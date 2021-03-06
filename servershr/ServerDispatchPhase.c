/*
Copyright (c) 2017, Massachusetts Institute of Technology All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

Redistributions of source code must retain the above copyright notice, this
list of conditions and the following disclaimer.

Redistributions in binary form must reproduce the above copyright notice, this
list of conditions and the following disclaimer in the documentation and/or
other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
/*------------------------------------------------------------------------------

                Name:   SERVER$DISPATCH_PHASE

                Type:   C function

                Author:	TOM FREDIAN

                Date:   21-APR-1992

                Purpose: Dispatch a phase

------------------------------------------------------------------------------

        Call sequence:

int SERVER$DISPATCH_PHASE(int efn, DispatchTable *table, struct descriptor
*phasenam, char *noact, int *sync, void (*output_rtn)(), struct descriptor
*monitor)

------------------------------------------------------------------------------
   Copyright (c) 1992
   Property of Massachusetts Institute of Technology, Cambridge MA 02139.
   This program cannot be copied or distributed in any form for non-MIT
   use without specific written approval of MIT Plasma Fusion Center
   Management.
---------------------------------------------------------------------------

        Description:

------------------------------------------------------------------------------*/

#include <mdsplus/mdsconfig.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#include <STATICdef.h>
#include <condition.h>
#include <mdsdescrip.h>
#include <ipdesc.h>
#include <servershr.h>
#include <ncidef.h>
#include <libroutines.h>
#include <strroutines.h>
#include <treeshr.h>
#include <mdsshr.h>
#include <servershr.h>
#include <mds_stdarg.h>
#include <tdishr_messages.h>
#include "servershrp.h"

extern int TdiCompletionOf();
extern int TdiExecute();
extern int TdiErrorlogsOf();
extern int TdiGetLong();
extern int ProgLoc;

void SendMonitor(int mode, int idx);

STATIC_ROUTINE void Dispatch(int idx);
STATIC_ROUTINE void DoSendMonitor(int mode, int idx);
STATIC_ROUTINE void ActionDone(int idx);
STATIC_ROUTINE void DoActionDone(int idx);
STATIC_ROUTINE void Before(int idx);
STATIC_ROUTINE void SetActionRanges(int phase, int *first_c, int *last_c);
STATIC_ROUTINE void AbortRange(int s, int e);
STATIC_ROUTINE void SetGroup(int sync, int first_g, int *last_g);
STATIC_ROUTINE int NoOutstandingActions(int s, int e);
STATIC_ROUTINE void RecordStatus(int s, int e);
STATIC_ROUTINE void Dispatch(int i);
STATIC_ROUTINE void WaitForActions(int conditionals, int first_g, int last_g,
                                   int first_c, int last_c);

// STATIC_CONSTANT const int zero = 0;

typedef struct _complete
{
  struct _complete *next;
  int idx;
} Complete;

STATIC_THREADSAFE Complete *CompletedQueueHead = NULL;
STATIC_THREADSAFE Complete *CompletedQueueTail = NULL;

typedef struct _send_monitor
{
  struct _send_monitor *next;
  int idx;
  int mode;
} SendMonitorInfo;

STATIC_THREADSAFE SendMonitorInfo *SendMonitorQueueHead = NULL;
STATIC_THREADSAFE SendMonitorInfo *SendMonitorQueueTail = NULL;

STATIC_THREADSAFE DispatchTable *table;
STATIC_THREADSAFE int noact;
STATIC_THREADSAFE void (*Output)();
STATIC_THREADSAFE int MonitorOn = 0;
STATIC_THREADSAFE char *Monitor = 0;
STATIC_THREADSAFE int first_s;
STATIC_THREADSAFE int last_s;

static int AbortInProgress;
static pthread_mutex_t abortinprogress_mutex = PTHREAD_MUTEX_INITIALIZER;
static inline int setAbortInProgress(int val_in)
{
  int val_out;
  pthread_mutex_lock(&abortinprogress_mutex);
  val_out = AbortInProgress;
  AbortInProgress = val_in;
  pthread_mutex_unlock(&abortinprogress_mutex);
  return val_out;
}
static inline int isAbortInProgress()
{
  int val;
  pthread_mutex_lock(&abortinprogress_mutex);
  val = AbortInProgress;
  pthread_mutex_unlock(&abortinprogress_mutex);
  return val;
}

STATIC_THREADSAFE Condition JobWaitC = CONDITION_INITIALIZER;
STATIC_THREADSAFE Condition SendMonitorC = CONDITION_INITIALIZER;
STATIC_THREADSAFE Condition CompletedC = CONDITION_INITIALIZER;

static pthread_mutex_t send_monitor_queue_mutex = PTHREAD_MUTEX_INITIALIZER;
#define MONITOR_QUEUE_LOCK                       \
  pthread_mutex_lock(&send_monitor_queue_mutex); \
  pthread_cleanup_push((void *)pthread_mutex_unlock, &send_monitor_queue_mutex)
#define MONITOR_QUEUE_UNLOCK pthread_cleanup_pop(1)

STATIC_THREADSAFE pthread_mutex_t completed_queue_mutex =
    PTHREAD_MUTEX_INITIALIZER;

STATIC_THREADSAFE pthread_rwlock_t table_lock = PTHREAD_RWLOCK_INITIALIZER;
#define WRLOCK_TABLE pthread_rwlock_wrlock(&table_lock)
#define RDLOCK_TABLE pthread_rwlock_rdlock(&table_lock)
#define UNLOCK_TABLE pthread_rwlock_unlock(&table_lock)

#define ACTION_LOCK(idx) table->actions[idx].lock
#define DEBUG_LOCK( \
    idx, info, typ, \
    b) // fprintf(stderr,"%d: %-10s  %s[%d,%d]%c\n",idx,#info,#typ,ACTION_LOCK(idx).__data.__readers,ACTION_LOCK(idx).__data.__writers,b)
#define XLOCK_ACTION(idx, info, typ)               \
  do                                               \
  {                                                \
    DEBUG_LOCK(idx, info, typ, '?');               \
    pthread_rwlock_##typ##lock(&ACTION_LOCK(idx)); \
    DEBUG_LOCK(idx, info, typ, '!');               \
  } while (0)
#define WRLOCK_ACTION(idx, info) XLOCK_ACTION(idx, info, wr)
#define RDLOCK_ACTION(idx, info) XLOCK_ACTION(idx, info, rd)
#define UNLOCK_ACTION(idx, info) XLOCK_ACTION(idx, info, un)

//#define DEBUG
#define PRINT_ACTIONS                                                    \
  {                                                                      \
    int sti;                                                             \
    for (sti = 0; sti < table->num; sti++)                               \
    {                                                                    \
      RDLOCK_ACTION(sti, debug);                                         \
      fprintf(stderr, "Action(%d): %s, p=%d(%d), %d, %d\n", sti,         \
              table->actions[sti].path, table->actions[sti].phase,       \
              table->actions[sti].condition != NULL,                     \
              table->actions[sti].dispatched, table->actions[sti].done); \
      UNLOCK_ACTION(sti, debug);                                         \
    }                                                                    \
  }
//"
STATIC_ROUTINE char *Server(char *out, char *srv)
{
  int i;
  for (i = 0; i < 32; i++)
    out[i] = srv[i] == ' ' ? 0 : srv[i];
  out[i] = 0;
  return out;
}

void SendMonitor(int mode, int idx)
{
  if (MonitorOn)
  {
    char tree[13];
    char *cptr;
    int i;
    RDLOCK_TABLE;
    if (table)
    { // if no table prevent seg fault
      for (i = 0, cptr = table->tree; i < 12; i++)
        if (cptr[i] == (char)32)
          break;
        else
          tree[i] = cptr[i];
      tree[i] = 0;
      ActionInfo *actions = table->actions;
      RDLOCK_ACTION(idx, sm);
      int on = actions[idx].on;
      char server[33];
      for (i = 0, cptr = actions[idx].server; i < 32 && (cptr[i] != ' '); i++)
        server[i] = cptr[i];
      server[i] = 0;
      MonitorOn = ServerSendMonitor(Monitor, tree, table->shot,
                                    actions[idx].phase, actions[idx].nid, on,
                                    mode, server, actions[idx].status);
      UNLOCK_ACTION(idx, sm);
    }
    UNLOCK_TABLE;
  }
}

STATIC_ROUTINE void ActionDone(int idx)
{
  int i;
  char logmsg[1024];
  if (idx >= 0)
  {
    RDLOCK_TABLE;
    if (table)
    { // if no table prevent seg fault
      RDLOCK_ACTION(idx, ad);
      ActionInfo *actions = table->actions;
      if (actions[idx].event)
        MDSEvent(actions[idx].event, sizeof(int), (char *)&table->shot);
      DoSendMonitor(MonitorDone, idx);
      if (Output)
      {
        char now[32];
        Now32(now);
        if (IS_OK(actions[idx].status))
          sprintf(logmsg, "%s, Action %s completed", now, actions[idx].path);
        else
        {
          char *emsg = MdsGetMsg(actions[idx].status);
          sprintf(logmsg, "%s, Action %s failed, %s", now, actions[idx].path,
                  emsg);
        }
        (*Output)(logmsg);
      }
      if (!isAbortInProgress())
      {
        EMPTYXD(xd);
        char expression[60];
        struct descriptor expression_d = {0, DTYPE_T, CLASS_S, 0};
        expression_d.pointer = expression;
        expression_d.length = sprintf(expression, "PUBLIC _ACTION_%08X = %d",
                                      actions[idx].nid, actions[idx].status);
        TdiExecute(&expression_d, &xd MDS_END_ARG);
        MdsFree1Dx(&xd, NULL);
        for (i = 0; i < actions[idx].num_references; i++)
        {
          int dstat;
          int doit;
          int cidx = actions[idx].referenced_by[i];
          RDLOCK_ACTION(cidx, adl);
          if (!actions[cidx].done)
          {
            if (IS_OK(dstat = TdiGetLong(actions[cidx].condition, &doit)))
            {
              UNLOCK_ACTION(cidx, ad_ftt);
              if (doit)
                Dispatch(cidx);
              else
              {
                WRLOCK_ACTION(cidx, ad_ftte);
                actions[cidx].status = ServerNOT_DISPATCHED;
                UNLOCK_ACTION(cidx, ad_ftte);
                DoActionDone(cidx);
              }
            }
            else if (dstat != TdiUNKNOWN_VAR)
            {
              UNLOCK_ACTION(cidx, ad_fte);
              WRLOCK_ACTION(cidx, ad_fte);
              actions[cidx].status = ServerINVALID_DEPENDENCY;
              UNLOCK_ACTION(cidx, ad_fte);
              DoActionDone(cidx);
            }
          }
          else
            UNLOCK_ACTION(cidx, ad_fe);
        }
      }
      UNLOCK_ACTION(idx, ad);
      WRLOCK_ACTION(idx, ad);
      {
        actions[idx].done = 1;
        actions[idx].recorded = 0;
      }
      UNLOCK_ACTION(idx, ad);
    }
    UNLOCK_TABLE;
  }
  CONDITION_SET(&JobWaitC);
}

STATIC_ROUTINE void Before(int idx)
{
  RDLOCK_TABLE;
  if (table)
  { // if no table prevent seg fault
    WRLOCK_ACTION(idx, b);
    {
      ActionInfo *actions = table->actions;
      char logmsg[1024];
      actions[idx].doing = 1;
      DoSendMonitor(MonitorDoing, idx);
      if (Output)
      {
        char server[33];
        char now[32];
        Now32(now);
        sprintf(logmsg, "%s, %s is beginning action %s", now,
                Server(server, actions[idx].server), actions[idx].path);
        (*Output)(logmsg);
      }
    }
    UNLOCK_ACTION(idx, b);
  }
  UNLOCK_TABLE;
}

/* FIND_NEXT_ACTION(START,END,CONDITION)
 * look-up of actions to find first action between START and END-1 meeting
 * CONDITION if successful, i.e. i<END, action[i] will have RDLOCK acquired
 * FIND_NEXT_ACTION_END(END)
 * UNLOCKs action[i] if FIND_NEXT_ACTION left if locked, i.e. CONDITION was met,
 * i<END.
 */
#define FIND_NEXT_ACTION(START, END, CONDITION, info) \
  i = START;                                          \
  for (; i < END; i++)                                \
  {                                                   \
    RDLOCK_ACTION(i, fna_##info);                     \
    if (CONDITION)                                    \
      break;                                          \
    UNLOCK_ACTION(i, fna_##info);                     \
  } // leaves actions[i] locked if i < table->num
#define FIND_NEXT_ACTION_END(END, info) \
  if (i < END)                          \
    UNLOCK_ACTION(i, fnae_##info);

STATIC_ROUTINE void SetActionRanges(int phase, int *first_c, int *last_c)
{
  int i;
  RDLOCK_TABLE;
  if (table)
  { // if no table prevent seg fault
    ActionInfo *actions = table->actions;
#ifdef DEBUG
    PRINT_ACTIONS;
#endif
    FIND_NEXT_ACTION(0, table->num, actions[i].phase == phase, sar)
    if (i < table->num)
    {
      if (actions[i].condition)
      {
        UNLOCK_ACTION(i, sar1);
        *first_c = i;
        FIND_NEXT_ACTION(*first_c + 1, table->num,
                         (actions[i].phase != phase) || !actions[i].condition,
                         sar1);
        *last_c = i;
      }
      else
        *first_c = *last_c = table->num;
      if (i < table->num && actions[i].phase == phase)
      {
        UNLOCK_ACTION(i, sar2);
        first_s = i;
        FIND_NEXT_ACTION(first_s + 1, table->num, actions[i].phase != phase,
                         sar2);
        last_s = i;
      }
      else
        first_s = last_s = table->num;
      FIND_NEXT_ACTION_END(table->num, sar);
    }
    else
    {
      *first_c = first_s = table->num;
      *last_c = last_s = table->num;
    }
  }
  else
  {
    *first_c = first_s = 0;
    *last_c = last_s = 0;
  }
  UNLOCK_TABLE;
#ifdef DEBUG
  fprintf(stderr, "ActionRange: %d,%d,%d,%d\n", *first_c, *last_c, first_s,
          last_s);
#endif
}

STATIC_ROUTINE void AbortRange(int s, int e)
{
  int i;
  RDLOCK_TABLE;
  {
    ActionInfo *actions = table->actions;
    for (i = s; i < e; i++)
    {
      RDLOCK_ACTION(i, ar);
      if (actions[i].dispatched && !actions[i].done)
      {
        char server[33];
        int one = 1;
        ServerAbortServer(Server(server, actions[i].server), one);
      }
      UNLOCK_ACTION(i, ar);
    }
  }
  UNLOCK_TABLE;
}

STATIC_ROUTINE void SetGroup(int sync, int first_g, int *last_g)
{
  if (first_g == last_s)
  {
    *last_g = last_s;
    return;
  }
  int i;
  int group;
  RDLOCK_TABLE;
  if (table)
  { // if no table prevent seg fault
    ActionInfo *actions = table->actions;
    sync = (sync < 1) ? 1 : sync;
    RDLOCK_ACTION(first_g, sg);
    group = actions[first_g].sequence / sync;
    UNLOCK_ACTION(first_g, sg);
    FIND_NEXT_ACTION(first_g + 1, last_s, (actions[i].sequence / sync) != group,
                     sg);
    FIND_NEXT_ACTION_END(last_s, sg);
  }
  else
  {
    i = first_g - 1;
  }
  UNLOCK_TABLE;
  *last_g = i;
}

STATIC_ROUTINE int NoOutstandingActions(const int s, const int e)
{
  int i;
  ActionInfo *actions = table->actions;
  FIND_NEXT_ACTION(s, e, actions[i].dispatched && !actions[i].done, noa);
  FIND_NEXT_ACTION_END(e, noa);
#ifdef DEBUG
  fprintf(stderr, "%d -> %d==%d\n", s, i, e);
#endif
  return i >= e;
}

STATIC_ROUTINE void RecordStatus(int s, int e)
{
  int i;
  RDLOCK_TABLE;
  if (table)
  { // if no table prevent seg fault
    ActionInfo *actions = table->actions;
    for (i = s; i < e; i++)
    {
      WRLOCK_ACTION(i, rs);
      if (actions[i].done && !actions[i].recorded)
      {
        NCI_ITM setnci[] = {{sizeof(actions[0].status), NciSTATUS, 0, 0},
                            {0, NciEND_OF_LIST, 0, 0}};
        setnci[0].pointer = (unsigned char *)&actions[i].status;
        TreeSetNci(actions[i].nid, setnci);
        actions[i].recorded = 1;
        if (IS_NOT_OK(actions[i].status) && !table->failed_essential)
        {
          int flags;
          NCI_ITM getnci[] = {{sizeof(flags), NciGET_FLAGS, 0, 0},
                              {0, NciEND_OF_LIST, 0, 0}};
          getnci[0].pointer = &flags;
          TreeGetNci(actions[i].nid, getnci);
          table->failed_essential = (flags & NciM_ESSENTIAL) != 0;
        }
      }
      UNLOCK_ACTION(i, rs);
    }
  }
  UNLOCK_TABLE;
}

STATIC_ROUTINE void WaitForActions(int all, int first_g, int last_g,
                                   int first_c, int last_c)
{
  int c_status = C_OK;
  _CONDITION_LOCK(&JobWaitC);
  struct timespec tp;
  clock_gettime(CLOCK_REALTIME, &tp);
  int g, c = 1;
  while ((c_status == ETIMEDOUT || c_status == C_OK) && !isAbortInProgress() &&
         (g = !NoOutstandingActions(first_g, last_g) ||
              (all && (c = !NoOutstandingActions(first_c, last_c)))))
  {
#ifdef DEBUG
    fprintf(stderr, "%lu: %d, %d\n", (long unsigned int)tp.tv_sec, g, c);
    PRINT_ACTIONS;
#endif
    tp.tv_sec++;
    c_status = pthread_cond_timedwait(&JobWaitC.cond, &JobWaitC.mutex, &tp);
#if defined(_DECTHREADS_) && (_DECTHREADS_ == 1)
    if (c_status == -1 && errno == 11)
      c_status = ETIMEDOUT;
#endif
  }
  _CONDITION_UNLOCK(&JobWaitC);
}

STATIC_ROUTINE char *DetailProc(int full)
{
  char *msg;
  RDLOCK_TABLE;
  if (table)
  { // if no table prevent seg fault
    msg = NULL;
    int i;
    int doing;
    char msg1[1024];
    unsigned int msglen = 4096;
    ActionInfo *actions = table->actions;
    for (doing = 1; doing > (full ? -1 : 0); doing--)
    {
      for (i = 0; i < table->num; i++)
      {
        RDLOCK_ACTION(i, dp);
        if (actions[i].dispatched && !actions[i].done &&
            ((int)actions[i].doing == doing))
        {
          char server[33];
          sprintf(msg1, "	%s %s %s for shot %d\n", actions[i].path,
                  actions[i].doing ? "in progress on" : "dispatched to",
                  Server(server, actions[i].server), table->shot);
          UNLOCK_ACTION(i, dp_t);
          if (!msg)
            msg = strcpy((char *)malloc(msglen), "\nWaiting on:\n");
          if (msglen < (strlen(msg) + strlen(msg1) + 1))
          {
            char *oldmsg = msg;
            msglen += 4096;
            msg = strcpy((char *)malloc(msglen), oldmsg);
            free(oldmsg);
          }
          strcat(msg, msg1);
        }
        else
          UNLOCK_ACTION(i, dp_e);
      }
    }
  }
  else
  {
    msg = NULL;
  }
  UNLOCK_TABLE;
  return msg;
}

static inline void setMonitor(const char *monitor)
{
  MONITOR_QUEUE_LOCK;
  if (monitor)
  {
    MonitorOn = B_TRUE;
    free(Monitor);
    Monitor = strdup(monitor);
  }
  else
  {
    free(Monitor);
    Monitor = NULL;
    MonitorOn = B_FALSE;
  }
  MONITOR_QUEUE_UNLOCK;
}

EXPORT int ServerDispatchPhase(int *id __attribute__((unused)), void *vtable,
                               char *phasenam, char noact_in, int sync,
                               void (*output_rtn)(), const char *monitor)
{
  WRLOCK_TABLE;
  {
    table = vtable;
    if (table)
      table->failed_essential = B_FALSE;
  }
  UNLOCK_TABLE;
  if (!vtable)
    return TreeSUCCESS;
  int i;
  int phase;
  int first_g, last_g = 0, first_c, last_c;
  DESCRIPTOR_LONG(phase_d, 0);
  STATIC_CONSTANT DESCRIPTOR(phase_lookup, "PHASE_NUMBER_LOOKUP($)");
  struct descriptor phasenam_d = {0, DTYPE_T, CLASS_S, 0};
  phase_d.pointer = (char *)&phase;
  Output = output_rtn;
  noact = noact_in;
  setAbortInProgress(0);
  phasenam_d.length = strlen(phasenam);
  phasenam_d.pointer = phasenam;
  ProgLoc = 6005;
  int status = TdiExecute(&phase_lookup, &phasenam_d, &phase_d MDS_END_ARG);
  ProgLoc = 6006;
  if (STATUS_OK && (phase > 0))
  {
    setMonitor(monitor);
    ProgLoc = 6007;
    SetActionRanges(phase, &first_c, &last_c);
    ProgLoc = 6008;
    ServerSetDetailProc(DetailProc);
    ProgLoc = 6009;
    first_g = first_s;
    while (!isAbortInProgress() && (first_g < last_s))
    {
      ProgLoc = 6010;
      SetGroup(sync, first_g, &last_g);
      ProgLoc = 6011;
      for (i = first_g; i < last_g; i++)
        Dispatch(i);
      ProgLoc = 6012;
      WaitForActions(0, first_g, last_g, first_c, last_c);
      first_g = last_g;
    }
    ProgLoc = 6013;
    if (setAbortInProgress(0))
    {
      AbortRange(first_c, last_c);
      AbortRange(first_s, last_s);
      status = ServerABORT;
    }
    ProgLoc = 6014;
    WaitForActions(1, first_g, last_g, first_c, last_c);
    ProgLoc = 6015;
    setAbortInProgress(1);
    RDLOCK_TABLE;
    if (table)
    {
      ActionInfo *actions = table->actions;
      for (i = first_c; i < last_c; i++)
      {
        RDLOCK_ACTION(i, sdp);
        if (!actions[i].done)
        {
          UNLOCK_ACTION(i, sdpw);
          WRLOCK_ACTION(i, sdpw);
          actions[i].status = ServerCANT_HAPPEN;
          DoActionDone(i);
        }
        UNLOCK_ACTION(i, sdp);
      }
    }
    UNLOCK_TABLE;
    ProgLoc = 6015;
    WaitForActions(1, first_g, last_g, first_c, last_c);
    ProgLoc = 6016;
    if (!noact)
      RecordStatus(first_c, last_c);
    ProgLoc = 6017;
    if (!noact)
      RecordStatus(first_s, last_s);
    ProgLoc = 6018;
  }
  ProgLoc = 6019;
  return status;
}

EXPORT int ServerFailedEssential(void *vtable, int reset)
{
  // returns B_FALSE if no table
  int failed;
  WRLOCK_TABLE;
  if (vtable)
  {
    DispatchTable *table = (DispatchTable *)vtable;
    failed = table->failed_essential;
    if (reset)
      table->failed_essential = B_FALSE;
  }
  else
  {
    failed = B_FALSE;
  }
  UNLOCK_TABLE;
  return failed;
}

STATIC_ROUTINE void Dispatch(int i)
{
  RDLOCK_TABLE;
  if (table)
  {
    int status;
    char logmsg[1024];
    char server[33];
    WRLOCK_ACTION(i, d);
    {
      ActionInfo *actions = table->actions;
      actions[i].done = 0;
      actions[i].doing = 0;
      actions[i].dispatched = 0;
      if (Output)
      {
        char now[32];
        Now32(now);
        sprintf(logmsg, "%s, Dispatching node %s to %s", now, actions[i].path,
                Server(server, actions[i].server));
        (*Output)(logmsg);
      }
      // ProgLoc = 7001;
      DoSendMonitor(MonitorDispatched, i);
      // ProgLoc = 7002;
      if (noact)
      {
        actions[i].dispatched = 1;
        actions[i].status = status = 1;
        DoActionDone(i);
      }
      else
      {
        UNLOCK_ACTION(i, d_w);
        status = ServerDispatchAction(
            0, Server(server, actions[i].server), table->tree, table->shot,
            actions[i].nid, DoActionDone, i + (char *)0, &actions[i].status,
            &actions[i].lock, &actions[i].netid, Before);
        WRLOCK_ACTION(i, d_w);
        // ProgLoc = 7003;
        if (STATUS_OK)
          actions[i].dispatched = 1;
      }
      // ProgLoc = 7004;
      if (STATUS_NOT_OK)
      {
        actions[i].status = status;
        DoActionDone(i);
      }
      // ProgLoc = 7005;
    }
    UNLOCK_ACTION(i, d);
  }
  UNLOCK_TABLE;
}

STATIC_ROUTINE void WakeCompletedActionQueue() { CONDITION_SET(&CompletedC); }

STATIC_ROUTINE void WaitForActionDoneQueue()
{
  CONDITION_WAIT_1SEC(&CompletedC);
}

STATIC_ROUTINE void QueueCompletedAction(int i)
{
  Complete *c = malloc(sizeof(Complete));
  c->idx = i;
  c->next = 0;
  pthread_mutex_lock(&completed_queue_mutex);
  if (CompletedQueueTail)
    CompletedQueueTail->next = c;
  else
    CompletedQueueHead = c;
  CompletedQueueTail = c;
  pthread_mutex_unlock(&completed_queue_mutex);
  WakeCompletedActionQueue();
}

STATIC_ROUTINE int DequeueCompletedAction(int *i)
{
  int doneAction = -1;
  while (doneAction == -1)
  {
    pthread_mutex_lock(&completed_queue_mutex);
    if (CompletedQueueHead)
    {
      Complete *c = CompletedQueueHead;
      doneAction = CompletedQueueHead->idx;
      CompletedQueueHead = CompletedQueueHead->next;
      if (!CompletedQueueHead)
        CompletedQueueTail = 0;
      free(c);
      pthread_mutex_unlock(&completed_queue_mutex);
    }
    else
    {
      CONDITION_RESET(&CompletedC); // reset list state
      pthread_mutex_unlock(&completed_queue_mutex);
      WaitForActionDoneQueue();
    }
  }
  *i = doneAction;
  return B_TRUE;
}

STATIC_THREADSAFE Condition ActionDoneRunningC = CONDITION_INITIALIZER;

STATIC_ROUTINE void ActionDoneExit() { CONDITION_RESET(&ActionDoneRunningC); }

STATIC_ROUTINE void ActionDoneThread()
{
  int i;
  pthread_cleanup_push(ActionDoneExit, 0);
  CONDITION_SET(&ActionDoneRunningC);
  while (DequeueCompletedAction(&i))
    ActionDone(i);
  pthread_cleanup_pop(1);
  pthread_exit(NULL);
}

STATIC_ROUTINE void DoActionDone(int i)
{
  INIT_STATUS;
  pthread_t thread;
  QueueCompletedAction(i); /***** must be done before starting thread ****/
  CONDITION_START_THREAD(&ActionDoneRunningC, thread, , ActionDoneThread, NULL);
  if (STATUS_NOT_OK)
    perror("DoActionDone: pthread creation failed");
}

STATIC_THREADSAFE Condition SendMonitorRunningC = CONDITION_INITIALIZER;

STATIC_ROUTINE void WakeSendMonitorQueue() { CONDITION_SET(&SendMonitorC); }

STATIC_ROUTINE void WaitForSendMonitorQueue()
{
  CONDITION_WAIT_1SEC(&SendMonitorC);
}

STATIC_ROUTINE void QueueSendMonitor(int mode, int i)
{
  SendMonitorInfo *c = malloc(sizeof(SendMonitorInfo));
  c->idx = i;
  c->mode = mode;
  c->next = 0;
  MONITOR_QUEUE_LOCK;
  if (SendMonitorQueueTail)
    SendMonitorQueueTail->next = c;
  else
    SendMonitorQueueHead = c;
  SendMonitorQueueTail = c;
  MONITOR_QUEUE_UNLOCK;
  WakeSendMonitorQueue();
}

STATIC_ROUTINE int DequeueSendMonitor(int *mode_out, int *i)
{
  int idx;
  int mode;
  do
  {
    int release;
    MONITOR_QUEUE_LOCK;
    release = 1;
    if (SendMonitorQueueHead)
    {
      SendMonitorInfo *c = SendMonitorQueueHead;
      idx = SendMonitorQueueHead->idx;
      mode = SendMonitorQueueHead->mode;
      SendMonitorQueueHead = SendMonitorQueueHead->next;
      if (!SendMonitorQueueHead)
        SendMonitorQueueTail = NULL;
      free(c);
    }
    else
    {
      pthread_mutex_unlock(&send_monitor_queue_mutex);
      release = 0;
      idx = -1;
      mode = 0;
      WaitForSendMonitorQueue();
    }
    pthread_cleanup_pop(release);
  } while (idx == -1);
  *i = idx;
  *mode_out = mode;
  return B_TRUE;
}

STATIC_ROUTINE void SendMonitorExit() { CONDITION_RESET(&SendMonitorRunningC); }

STATIC_ROUTINE void SendMonitorThread()
{
  int i;
  int mode;
  pthread_cleanup_push(SendMonitorExit, NULL);
  CONDITION_SET(&SendMonitorRunningC);
  while (DequeueSendMonitor(&mode, &i))
    SendMonitor(mode, i);
  pthread_cleanup_pop(1);
  pthread_exit(NULL);
}

STATIC_ROUTINE void DoSendMonitor(int mode, int idx)
{
  INIT_STATUS;
  pthread_t thread;
  QueueSendMonitor(mode, idx); /***** must be done before starting thread ****/
  CONDITION_START_THREAD(&SendMonitorRunningC, thread, , SendMonitorThread,
                         NULL);
  if (STATUS_NOT_OK)
    perror("DoSendMonitor: pthread creation failed");
}

void serverDisarmDispatchTable(void *vtable)
{
  WRLOCK_TABLE;
  if (table == vtable)
    table = NULL; // disarm table
  UNLOCK_TABLE;
}

static inline void serverFreeDispatchTable(void *vtable)
{
  ActionInfo *actions = ((DispatchTable *)vtable)->actions;
  int num_actions = ((DispatchTable *)vtable)->num;
  int i;
  for (i = 0; i < num_actions; i++)
  {
    free(actions[i].referenced_by);
    if (actions[i].path)
      TreeFree(actions[i].path);
    free(actions[i].event);
    if (actions[i].condition)
    {
      MdsFree1Dx((struct descriptor_xd *)actions[i].condition, 0);
      free(actions[i].condition);
    }
  }
  free(vtable);
}

EXPORT int ServerFreeDispatchTable(void *vtable)
{
  // should be called during CloseTopTree
  if (vtable)
  {
    serverDisarmDispatchTable(vtable);
    serverFreeDispatchTable(vtable);
  }
  return MDSplusSUCCESS;
}
