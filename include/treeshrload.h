#pragma once

#include <dlfcn.h>
#include <mdstypes.h>
#include <mdsdescrip.h>

#define TreeBLOCKID 0x3ade68b1

extern int treeshr_errno;
extern int TREE_BLOCKID;

#ifndef DBIDEF_H
  struct dbi_itm;
#endif

#ifndef NCIDEF_H
  struct nci_itm;
#endif

void *  (*TreeSavePrivateCtx)(void* dbid);
void *  (*TreeRestorePrivateCtx)(void* pctx);
char *  (*TreeAbsPath)(char const *in);
char *  (*_TreeAbsPath)(void *dbid, char const *in);
int     (*TreeAddConglom)(char const *path, char const *congtype, int *nid);
int     (*_TreeAddConglom)(void *dbid, char const *path, char const *congtype, int *nid);
int     (*TreeAddNode)(char const *name, int *nid_ret, char usage);
int     (*_TreeAddNode)(void *dbid, char const *name, int *nid_ret, char usage);
int     (*TreeAddTag)(int nid, char const *tagnam);
int     (*_TreeAddTag)(void *dbid, int nid, char const *tagnam);
int     (*TreeCleanDatafile)(char *tree, int shot);
int     (*_TreeCleanDatafile)(void **dbid, char *tree, int shot);
int     (*TreeClose)(char const *tree, int shot);
int     (*_TreeClose)(void **dbid, char const *tree, int shot);
int     (*TreeCompressDatafile)(char *tree, int shot);
int     (*_TreeCompressDatafile)(void **dbid, char *tree, int shot);
int     (*TreeCreatePulseFile)(int shot, int numnids, int *nids);
int     (*_TreeCreatePulseFile)(void *dbid, int shot, int numnids, int *nids);
int     (*TreeCreateTreeFiles)(char *tree, int shot, int source_shot);
void ** (*TreeCtx)();
void *  (*TreeDbid)();
void *  (*_TreeDbid)(void **dbid);
void    (*TreeDeleteNodeExecute)(void);
void    (*_TreeDeleteNodeExecute)(void *dbid);
int     (*TreeDeleteNodeGetNid)(int *nid);
int     (*_TreeDeleteNodeGetNid)(void *dbid, int *nid);
int     (*TreeDeleteNodeInitialize)(int nid, int *count, int reset);
int     (*_TreeDeleteNodeInitialize)(void *dbid, int nid, int *count, int reset);
int     (*TreeDeletePulseFile)(int shotid, int all_versions);
int     (*_TreeDeletePulseFile)(void *dbid, int shotid, int all_versions);
int     (*TreeDoMethod)(mdsdsc_t *nid_dsc, mdsdsc_t *method_ptr, ...);
int     (*_TreeDoMethod)(void *dbid, mdsdsc_t *nid_dsc, mdsdsc_t *method_ptr, ...);
int     (*TreeDoMethodA)(mdsdsc_t *nid_dsc, mdsdsc_t *method_ptr, int nargs_in, mdsdsc_t **arglist_in, mdsdsc_xd_t *out_ptr);
int     (*_TreeDoMethodA)(void *dbid, mdsdsc_t *nid_dsc, mdsdsc_t *method_ptr, int nargs_in, mdsdsc_t **arglist_in, mdsdsc_xd_t *out_ptr);
int     (*TreeDoMethod_HANDLER)(int *sig_args, int *mech_args);
int     (*TreeEditing)();
int     (*_TreeEditing)(void *dbid);
int     (*TreeEndConglomerate)();
int     (*_TreeEndConglomerate)(void *dbid);
int     (*TreeFindNode)(char const *path, int *nid);
int     (*_TreeFindNode)(void *dbid, char const *path, int *nid);
int     (*TreeFindNodeEnd)(void **ctx);
int     (*_TreeFindNodeEnd)(void *dbid, void **ctx);
char *  (*TreeFindNodeTags)(int nid, void **ctx);
char *  (*_TreeFindNodeTags)(void *dbid, int nid, void **ctx);
int     (*TreeFindNodeWild)(char const *path, int *nid, void **ctx, int usage_mask);
int     (*_TreeFindNodeWild)(void *dbid, char const *path, int *nid, void **ctx, int usage_mask);
void    (*TreeFindTagEnd)(void **ctx);
char *  (*TreeFindTagWild)(char *wild, int *nidout, void **search_ctx);
char *  (*_TreeFindTagWild)(void *dbid, char *wild, int *nidout, void **ctx);
int     (*TreeFlushOff)(int nid);
int     (*_TreeFlushOff)(void *dbid, int nid);
int     (*TreeFlushReset)(int nid);
int     (*_TreeFlushReset)(void *dbid, int nid);
void    (*TreeFree)(void *);
void    (*TreeFreeDbid)(void *);
int     (*_TreeNewDbid)(void **dbid);
int     (*TreeGetDbi)(struct dbi_itm *itmlst);
int     (*_TreeGetDbi)(void *dbid, struct dbi_itm *itmlst);
int     (*TreeGetNci)(int nid, struct nci_itm *itmlst);
int     (*_TreeGetNci)(void *dbid, int nid, struct nci_itm *itmlst);
int     (*TreeGetDefaultNid)(int *nid);
int     (*_TreeGetDefaultNid)(void *dbid, int *nid);
char *  (*TreeGetMinimumPath)(int *def_nid, int nid);
char *  (*_TreeGetMinimumPath)(void *dbid, int *def_nid, int nid);
char *  (*TreeGetPath)(int nid);
char *  (*_TreeGetPath)(void *dbid, int nid);
int     (*TreeGetRecord)(int nid, struct descriptor_xd *dsc_ptr);
int     (*_TreeGetRecord)(void *dbid, int nid, struct descriptor_xd *dsc_ptr);
int     (*TreeGetStackSize)();
int     (*_TreeGetStackSize)(void *dbid);
int     (*TreeGetViewDate)(int64_t * date);
int     (*TreeIsOn)(int nid);
int     (*_TreeIsOn)(void *dbid, int nid);
int     (*TreeIsOpen)();
int     (*_TreeIsOpen)(void *dbid);
int     (*TreeLink)(struct descriptor *intree, struct descriptor *outtree);
int     (*TreeMarkIncludes)();
int     (*TreeNodePresent)(int *nid);
int     (*TreeOpen)(char const *tree, int shot, int flags);
int     (*_TreeOpen)(void **dbid, char const *tree, int shot, int flags);
int     (*TreeOpenEdit)(char const *tree, int shot);
int     (*_TreeOpenEdit)(void **dbid, char const *tree, int shot);
int     (*TreeOpenNew)(char const *tree, int shot);
int     (*_TreeOpenNew)(void **dbid, char const *tree, int shot);
int     (*TreePutRecord)(int nid, struct descriptor *descriptor_ptr, int utility_update);
int     (*_TreePutRecord)(void *dbid, int nid, struct descriptor *descriptor, int utility_update);
int     (*TreeQuitTree)(char const *tree, int shot);
int     (*_TreeQuitTree)(void **dbid, char const *tree, int shot);
int     (*TreeRemoveNodesTags)(int nid);
int     (*_TreeRemoveNodesTags)(void *dbid, int nid);
int     (*TreeRemoveTag)(char const *tagname);
int     (*_TreeRemoveTag)(void *dbid, char const *tagname);
int     (*TreeRenameNode)(int nid, char const *newname);
int     (*_TreeRenameNode)(void *dbid, int nid, char const *newname);
void    (*TreeRestoreContext)(void *ctx);
void    (*_TreeRestoreContext)(void **dbid, void *ctx);
int     (*TreeRundownTree)(struct descriptor *treenam, int *shotid, int single_tree, int force_exit);
void *  (*TreeSaveContext)();
void *  (*_TreeSaveContext)(void *dbid);
int     (*TreeSetDbi)(struct dbi_itm *itmlst);
int     (*_TreeSetDbi)(void *dbid, struct dbi_itm *itmlst);
int     (*TreeSetNci)(int nid, struct nci_itm *itmlst);
int     (*_TreeSetNci)(void *dbid, int nid, struct nci_itm *itmlst);
int     (*TreeSetDefault)(char *path, int *nid);
int     (*_TreeSetDefault)(void *dbid, char *path, int *nid);
int     (*TreeSetDefaultNid)(int nid);
int     (*_TreeSetDefaultNid)(void *dbid, int nid);
int     (*TreeSetNoSubtree)(int nid);
int     (*_TreeSetNoSubtree)(void *dbid, int nid);
int     (*TreeSetStackSize)(int new_size);
int     (*_TreeSetStackSize)(void **dbid, int new_size);
int     (*TreeSetSubtree)(int nid);
int     (*_TreeSetSubtree)(void *dbid, int nid);
int     (*TreeSetUsage)(int nid, unsigned char usage);
int     (*_TreeSetUsage)(void *dbid, int nid, unsigned char usage);
int     (*TreeStartConglomerate)(int numnodes);
int     (*_TreeStartConglomerate)(void *dbid, int numnodes);
void *  (*TreeSwitchDbid)(void *dbid);
int     (*TreeTurnOff)(int nid);
int     (*_TreeTurnOff)(void *dbid, int nid);
int     (*TreeTurnOn)(int nid);
int     (*_TreeTurnOn)(void *dbid, int nid);
int     (*TreeUsePrivateCtx)(int onoff);
int     (*TreeUsingPrivateCtx)();
int     (*TreeVerify)();
int     (*_TreeVerify)(void *dbid);
int     (*TreeWait)();
int     (*TreeWriteTree)(char const *tree, int shot);
int     (*_TreeWriteTree)(void **dbid, char const *tree, int shot);
int     (*TreeBeginSegment)(int nid, struct descriptor *start, struct descriptor *end, struct descriptor *dim, struct descriptor_a *initialData, int idx);
int     (*_TreeBeginSegment)(void *dbid, int nid, struct descriptor *start, struct descriptor *end, struct descriptor *dim, struct descriptor_a *initialData, int idx);
int     (*TreeMakeSegment)(int nid, struct descriptor *start, struct descriptor *end, struct descriptor *dim, struct descriptor_a *initialData, int idx, int filled);
int     (*_TreeMakeSegment)(void *dbid, int nid, struct descriptor *start, struct descriptor *end, struct descriptor *dim, struct descriptor_a *initialData, int idx, int filled);
int     (*TreePutSegment)(int nid, const int rowidx, struct descriptor_a *data);
int     (*_TreePutSegment)(void *dbid, int nid, const int rowidx, struct descriptor_a *data);
int     (*TreeSetRowsFilled)(int nid, int rows_filled);
int     (*_TreeSetRowsFilled)(void *dbid, int nid, int rows_filled);
int     (*TreeUpdateSegment)(int nid, struct descriptor *start, struct descriptor *end, struct descriptor *dim, int idx);
int     (*_TreeUpdateSegment)(void *dbid, int nid, struct descriptor *start, struct descriptor *end, struct descriptor *dim, int idx);
int     (*TreeBeginTimestampedSegment)(int nid, struct descriptor_a *initialValue, int idx);
int     (*_TreeBeginTimestampedSegment)(void *dbid, int nid, struct descriptor_a *initialValue, int idx);
int     (*TreePutTimestampedSegment)(int nid, int64_t * timestamp, struct descriptor_a *rowdata);
int     (*_TreePutTimestampedSegment)(void *dbid, int nid, int64_t * timestamp, struct descriptor_a *rowdata);
int     (*TreeMakeTimestampedSegment)(int nid, int64_t * timestamp, struct descriptor_a *rowdata, int idx, int filled);
int     (*_TreeMakeTimestampedSegment)(void *dbid, int nid, int64_t * timestamp, struct descriptor_a *rowdata, int idx, int filled);
int     (*TreePutRow)(int nid, int bufsize, int64_t * timestamp, struct descriptor_a *rowdata);
int     (*_TreePutRow)(void *dbid, int nid, int bufsize, int64_t * timestamp, struct descriptor_a *rowdata);
int     (*TreeSetTimeContext)(struct descriptor *start, struct descriptor *end, struct descriptor *delta);
int     (*_TreeSetTimeContext)(void *dbid, struct descriptor *start, struct descriptor *end, struct descriptor *delta);
int     (*TreeGetTimeContext)(struct descriptor_xd *start, struct descriptor_xd *end, struct descriptor_xd *delta);
int     (*_TreeGetTimeContext)(void *dbid, struct descriptor_xd *start, struct descriptor_xd *end, struct descriptor_xd *delta);
int     (*TreeGetNumSegments)(int nid, int *num);
int     (*_TreeGetNumSegments)(void *dbid, int nid, int *num);
int     (*TreeGetSegmentLimits)(int nid, int segidx, struct descriptor_xd *start, struct descriptor_xd *end);
int     (*_TreeGetSegmentLimits)(void *dbid, int nid, int segidx, struct descriptor_xd *start, struct descriptor_xd *end);
int     (*TreeGetSegment)(int nid, int segidx, struct descriptor_xd *data, struct descriptor_xd *dim);
int     (*_TreeGetSegment)(void *dbid, int nid, int segidx, struct descriptor_xd *data, struct descriptor_xd *dim);
int     (*TreeGetSegments)(int nid, struct descriptor *start, struct descriptor *end, struct descriptor_xd *retSegments);
int     (*_TreeGetSegments)(void *dbid, int nid, struct descriptor *start, struct descriptor *end, struct descriptor_xd *retSegments);
int     (*TreeGetSegmentInfo)(int nid, int idx, char *dtype, char *dimct, int *dims, int *next_row);
int     (*_TreeGetSegmentInfo)(void *dbid, int nid, int idx, char *dtype, char *dimct, int *dims, int *next_row);
int     (*TreeGetSegments)(int nid, struct descriptor *start, struct descriptor *end, struct descriptor_xd *out);
int     (*_TreeGetSegments)(void *dbid, int nid, struct descriptor *start, struct descriptor *end, struct descriptor_xd *out);
int     (*TreeGetSegmentTimes)(int nid, int *numsegs, int64_t ** times);
int     (*_TreeGetSegmentTimes)(void *dbid, int nid, int *numsegs, int64_t ** times);
int     (*TreeGetSegmentTimesXd)(int nid, int *numsegs, struct descriptor_xd *start_list, struct descriptor_xd *end_list);
int     (*_TreeGetSegmentTimesXd)(void *dbid, int nid, int *numsegs, struct descriptor_xd *start_list, struct descriptor_xd *end_list);
int     (*TreeGetXNci)(int nid, const char *xnciname, struct descriptor_xd *value);
int     (*_TreeGetXNci)(void *dbid, int nid, const char *xnciname, struct descriptor_xd *value);
int     (*TreeSetXNci)(int nid, const char *xnciname, struct descriptor *value);
int     (*_TreeSetXNci)(void *dbid, int nid, const char *xnciname, struct descriptor *value);
int     (*TreeSetViewDate)(int64_t * date);
int     (*_TreeSetViewDate)(void *dbid, int64_t * date);
int     (*TreeSetCurrentShotId)(char const *experiment, int shot);
int     (*TreeGetCurrentShotId)(char const *experiment);
int     (*TreeSetDbiItm)(int code, int value);
int64_t (*TreeGetDatafileSize)();
int64_t (*_TreeGetDatafileSize)(void *dbid);
int     (*TreeFindNodeTagsDsc)(int nid_in, void **ctx_ptr, struct descriptor *tag);
int     (*TreeFindTagWildDsc)(char *wild, int *nidout, void **ctx_inout, struct descriptor_xd *name);
int     (*_TreeGetSegmentScale)(void *dbid, int nid, struct descriptor_xd *value);
int     (*TreeGetSegmentScale)(int nid, struct descriptor_xd *value);
int     (*_TreeSetSegmentScale)(void *dbid, int nid, struct descriptor *value);
int     (*TreeSetSegmentScale)(int nid, struct descriptor *value);
int     (*_TreeExecute)(void *dbid, ...);
int     (*_TreeEvaluate)(void *dbid, ...);
int     (*_TreeDecompile)(void *dbid, ...);
int     (*_TreeCompile)(void *dbid, ...);
void *  (*TreeCtxPush)(void** ctx);
void    (*TreeCtxPop)(void* ps);
void *  (*TreeDbidPush)(void* dbid);
void *  (*TreeDbidPop)(void* ps);

static void * _TreeShr = NULL;

static int LoadTreeShr() {
    _TreeShr = dlopen("libTreeShr.so", RTLD_LAZY);
    if (!_TreeShr) {
        return 0;
    }

    TreeSavePrivateCtx              = dlsym(_TreeShr, "TreeSavePrivateCtx");
    TreeRestorePrivateCtx           = dlsym(_TreeShr, "TreeRestorePrivateCtx");
    TreeAbsPath                     = dlsym(_TreeShr, "TreeAbsPath");
    _TreeAbsPath                    = dlsym(_TreeShr, "_TreeAbsPath");
    TreeAddConglom                  = dlsym(_TreeShr, "TreeAddConglom");
    _TreeAddConglom                 = dlsym(_TreeShr, "_TreeAddConglom");
    TreeAddNode                     = dlsym(_TreeShr, "TreeAddNode");
    _TreeAddNode                    = dlsym(_TreeShr, "_TreeAddNode");
    TreeAddTag                      = dlsym(_TreeShr, "TreeAddTag");
    _TreeAddTag                     = dlsym(_TreeShr, "_TreeAddTag");
    TreeCleanDatafile               = dlsym(_TreeShr, "TreeCleanDatafile");
    _TreeCleanDatafile              = dlsym(_TreeShr, "_TreeCleanDatafile");
    TreeClose                       = dlsym(_TreeShr, "TreeClose");
    _TreeClose                      = dlsym(_TreeShr, "_TreeClose");
    TreeCompressDatafile            = dlsym(_TreeShr, "TreeCompressDatafile");
    _TreeCompressDatafile           = dlsym(_TreeShr, "_TreeCompressDatafile");
    TreeCreatePulseFile             = dlsym(_TreeShr, "TreeCreatePulseFile");
    _TreeCreatePulseFile            = dlsym(_TreeShr, "_TreeCreatePulseFile");
    TreeCreateTreeFiles             = dlsym(_TreeShr, "TreeCreateTreeFiles");
    TreeCtx                         = dlsym(_TreeShr, "TreeCtx");
    TreeDbid                        = dlsym(_TreeShr, "TreeDbid");
    _TreeDbid                       = dlsym(_TreeShr, "_TreeDbid");
    TreeDeleteNodeExecute           = dlsym(_TreeShr, "TreeDeleteNodeExecute");
    _TreeDeleteNodeExecute          = dlsym(_TreeShr, "_TreeDeleteNodeExecute");
    TreeDeleteNodeGetNid            = dlsym(_TreeShr, "TreeDeleteNodeGetNid");
    _TreeDeleteNodeGetNid           = dlsym(_TreeShr, "_TreeDeleteNodeGetNid");
    TreeDeleteNodeInitialize        = dlsym(_TreeShr, "TreeDeleteNodeInitialize");
    _TreeDeleteNodeInitialize       = dlsym(_TreeShr, "_TreeDeleteNodeInitialize");
    TreeDeletePulseFile             = dlsym(_TreeShr, "TreeDeletePulseFile");
    _TreeDeletePulseFile            = dlsym(_TreeShr, "_TreeDeletePulseFile");
    TreeDoMethod                    = dlsym(_TreeShr, "TreeDoMethod");
    _TreeDoMethod                   = dlsym(_TreeShr, "_TreeDoMethod");
    TreeDoMethodA                   = dlsym(_TreeShr, "TreeDoMethodA");
    _TreeDoMethodA                  = dlsym(_TreeShr, "_TreeDoMethodA");
    TreeDoMethod_HANDLER            = dlsym(_TreeShr, "TreeDoMethod_HANDLER");
    TreeEditing                     = dlsym(_TreeShr, "TreeEditing");
    _TreeEditing                    = dlsym(_TreeShr, "_TreeEditing");
    TreeEndConglomerate             = dlsym(_TreeShr, "TreeEndConglomerate");
    _TreeEndConglomerate            = dlsym(_TreeShr, "_TreeEndConglomerate");
    TreeFindNode                    = dlsym(_TreeShr, "TreeFindNode");
    _TreeFindNode                   = dlsym(_TreeShr, "_TreeFindNode");
    TreeFindNodeEnd                 = dlsym(_TreeShr, "TreeFindNodeEnd");
    _TreeFindNodeEnd                = dlsym(_TreeShr, "_TreeFindNodeEnd");
    TreeFindNodeTags                = dlsym(_TreeShr, "TreeFindNodeTags");
    _TreeFindNodeTags               = dlsym(_TreeShr, "_TreeFindNodeTags");
    TreeFindNodeWild                = dlsym(_TreeShr, "TreeFindNodeWild");
    _TreeFindNodeWild               = dlsym(_TreeShr, "_TreeFindNodeWild");
    TreeFindTagEnd                  = dlsym(_TreeShr, "TreeFindTagEnd");
    TreeFindTagWild                 = dlsym(_TreeShr, "TreeFindTagWild");
    _TreeFindTagWild                = dlsym(_TreeShr, "_TreeFindTagWild");
    TreeFlushOff                    = dlsym(_TreeShr, "TreeFlushOff");
    _TreeFlushOff                   = dlsym(_TreeShr, "_TreeFlushOff");
    TreeFlushReset                  = dlsym(_TreeShr, "TreeFlushReset");
    _TreeFlushReset                 = dlsym(_TreeShr, "_TreeFlushReset");
    TreeFree                        = dlsym(_TreeShr, "TreeFree");
    TreeFreeDbid                    = dlsym(_TreeShr, "TreeFreeDbid");
    _TreeNewDbid                    = dlsym(_TreeShr, "_TreeNewDbid");
    TreeGetDbi                      = dlsym(_TreeShr, "TreeGetDbi");
    _TreeGetDbi                     = dlsym(_TreeShr, "_TreeGetDbi");
    TreeGetNci                      = dlsym(_TreeShr, "TreeGetNci");
    _TreeGetNci                     = dlsym(_TreeShr, "_TreeGetNci");
    TreeGetDefaultNid               = dlsym(_TreeShr, "TreeGetDefaultNid");
    _TreeGetDefaultNid              = dlsym(_TreeShr, "_TreeGetDefaultNid");
    TreeGetMinimumPath              = dlsym(_TreeShr, "TreeGetMinimumPath");
    _TreeGetMinimumPath             = dlsym(_TreeShr, "_TreeGetMinimumPath");
    TreeGetPath                     = dlsym(_TreeShr, "TreeGetPath");
    _TreeGetPath                    = dlsym(_TreeShr, "_TreeGetPath");
    TreeGetRecord                   = dlsym(_TreeShr, "TreeGetRecord");
    _TreeGetRecord                  = dlsym(_TreeShr, "_TreeGetRecord");
    TreeGetStackSize                = dlsym(_TreeShr, "TreeGetStackSize");
    _TreeGetStackSize               = dlsym(_TreeShr, "_TreeGetStackSize");
    TreeGetViewDate                 = dlsym(_TreeShr, "TreeGetViewDate");
    TreeIsOn                        = dlsym(_TreeShr, "TreeIsOn");
    _TreeIsOn                       = dlsym(_TreeShr, "_TreeIsOn");
    TreeIsOpen                      = dlsym(_TreeShr, "TreeIsOpen");
    _TreeIsOpen                     = dlsym(_TreeShr, "_TreeIsOpen");
    TreeLink                        = dlsym(_TreeShr, "TreeLink");
    TreeMarkIncludes                = dlsym(_TreeShr, "TreeMarkIncludes");
    TreeNodePresent                 = dlsym(_TreeShr, "TreeNodePresent");
    TreeOpen                        = dlsym(_TreeShr, "TreeOpen");
    _TreeOpen                       = dlsym(_TreeShr, "_TreeOpen");
    TreeOpenEdit                    = dlsym(_TreeShr, "TreeOpenEdit");
    _TreeOpenEdit                   = dlsym(_TreeShr, "_TreeOpenEdit");
    TreeOpenNew                     = dlsym(_TreeShr, "TreeOpenNew");
    _TreeOpenNew                    = dlsym(_TreeShr, "_TreeOpenNew");
    TreePutRecord                   = dlsym(_TreeShr, "TreePutRecord");
    _TreePutRecord                  = dlsym(_TreeShr, "_TreePutRecord");
    TreeQuitTree                    = dlsym(_TreeShr, "TreeQuitTree");
    _TreeQuitTree                   = dlsym(_TreeShr, "_TreeQuitTree");
    TreeRemoveNodesTags             = dlsym(_TreeShr, "TreeRemoveNodesTags");
    _TreeRemoveNodesTags            = dlsym(_TreeShr, "_TreeRemoveNodesTags");
    TreeRemoveTag                   = dlsym(_TreeShr, "TreeRemoveTag");
    _TreeRemoveTag                  = dlsym(_TreeShr, "_TreeRemoveTag");
    TreeRenameNode                  = dlsym(_TreeShr, "TreeRenameNode");
    _TreeRenameNode                 = dlsym(_TreeShr, "_TreeRenameNode");
    TreeRestoreContext              = dlsym(_TreeShr, "TreeRestoreContext");
    _TreeRestoreContext             = dlsym(_TreeShr, "_TreeRestoreContext");
    TreeRundownTree                 = dlsym(_TreeShr, "TreeRundownTree");
    TreeSaveContext                 = dlsym(_TreeShr, "TreeSaveContext");
    _TreeSaveContext                = dlsym(_TreeShr, "_TreeSaveContext");
    TreeSetDbi                      = dlsym(_TreeShr, "TreeSetDbi");
    _TreeSetDbi                     = dlsym(_TreeShr, "_TreeSetDbi");
    TreeSetNci                      = dlsym(_TreeShr, "TreeSetNci");
    _TreeSetNci                     = dlsym(_TreeShr, "_TreeSetNci");
    TreeSetDefault                  = dlsym(_TreeShr, "TreeSetDefault");
    _TreeSetDefault                 = dlsym(_TreeShr, "_TreeSetDefault");
    TreeSetDefaultNid               = dlsym(_TreeShr, "TreeSetDefaultNid");
    _TreeSetDefaultNid              = dlsym(_TreeShr, "_TreeSetDefaultNid");
    TreeSetNoSubtree                = dlsym(_TreeShr, "TreeSetNoSubtree");
    _TreeSetNoSubtree               = dlsym(_TreeShr, "_TreeSetNoSubtree");
    TreeSetStackSize                = dlsym(_TreeShr, "TreeSetStackSize");
    _TreeSetStackSize               = dlsym(_TreeShr, "_TreeSetStackSize");
    TreeSetSubtree                  = dlsym(_TreeShr, "TreeSetSubtree");
    _TreeSetSubtree                 = dlsym(_TreeShr, "_TreeSetSubtree");
    TreeSetUsage                    = dlsym(_TreeShr, "TreeSetUsage");
    _TreeSetUsage                   = dlsym(_TreeShr, "_TreeSetUsage");
    TreeStartConglomerate           = dlsym(_TreeShr, "TreeStartConglomerate");
    _TreeStartConglomerate          = dlsym(_TreeShr, "_TreeStartConglomerate");
    TreeSwitchDbid                  = dlsym(_TreeShr, "TreeSwitchDbid");
    TreeTurnOff                     = dlsym(_TreeShr, "TreeTurnOff");
    _TreeTurnOff                    = dlsym(_TreeShr, "_TreeTurnOff");
    TreeTurnOn                      = dlsym(_TreeShr, "TreeTurnOn");
    _TreeTurnOn                     = dlsym(_TreeShr, "_TreeTurnOn");
    TreeUsePrivateCtx               = dlsym(_TreeShr, "TreeUsePrivateCtx");
    TreeUsingPrivateCtx             = dlsym(_TreeShr, "TreeUsingPrivateCtx");
    TreeVerify                      = dlsym(_TreeShr, "TreeVerify");
    _TreeVerify                     = dlsym(_TreeShr, "_TreeVerify");
    TreeWait                        = dlsym(_TreeShr, "TreeWait");
    TreeWriteTree                   = dlsym(_TreeShr, "TreeWriteTree");
    _TreeWriteTree                  = dlsym(_TreeShr, "_TreeWriteTree");
    TreeBeginSegment                = dlsym(_TreeShr, "TreeBeginSegment");
    _TreeBeginSegment               = dlsym(_TreeShr, "_TreeBeginSegment");
    TreeMakeSegment                 = dlsym(_TreeShr, "TreeMakeSegment");
    _TreeMakeSegment                = dlsym(_TreeShr, "_TreeMakeSegment");
    TreePutSegment                  = dlsym(_TreeShr, "TreePutSegment");
    _TreePutSegment                 = dlsym(_TreeShr, "_TreePutSegment");
    TreeSetRowsFilled               = dlsym(_TreeShr, "TreeSetRowsFilled");
    _TreeSetRowsFilled              = dlsym(_TreeShr, "_TreeSetRowsFilled");
    TreeUpdateSegment               = dlsym(_TreeShr, "TreeUpdateSegment");
    _TreeUpdateSegment              = dlsym(_TreeShr, "_TreeUpdateSegment");
    TreeBeginTimestampedSegment     = dlsym(_TreeShr, "TreeBeginTimestampedSegment");
    _TreeBeginTimestampedSegment    = dlsym(_TreeShr, "_TreeBeginTimestampedSegment");
    TreePutTimestampedSegment       = dlsym(_TreeShr, "TreePutTimestampedSegment");
    _TreePutTimestampedSegment      = dlsym(_TreeShr, "_TreePutTimestampedSegment");
    TreeMakeTimestampedSegment      = dlsym(_TreeShr, "TreeMakeTimestampedSegment");
    _TreeMakeTimestampedSegment     = dlsym(_TreeShr, "_TreeMakeTimestampedSegment");
    TreePutRow                      = dlsym(_TreeShr, "TreePutRow");
    _TreePutRow                     = dlsym(_TreeShr, "_TreePutRow");
    TreeSetTimeContext              = dlsym(_TreeShr, "TreeSetTimeContext");
    _TreeSetTimeContext             = dlsym(_TreeShr, "_TreeSetTimeContext");
    TreeGetTimeContext              = dlsym(_TreeShr, "TreeGetTimeContext");
    _TreeGetTimeContext             = dlsym(_TreeShr, "_TreeGetTimeContext");
    TreeGetNumSegments              = dlsym(_TreeShr, "TreeGetNumSegments");
    _TreeGetNumSegments             = dlsym(_TreeShr, "_TreeGetNumSegments");
    TreeGetSegmentLimits            = dlsym(_TreeShr, "TreeGetSegmentLimits");
    _TreeGetSegmentLimits           = dlsym(_TreeShr, "_TreeGetSegmentLimits");
    TreeGetSegment                  = dlsym(_TreeShr, "TreeGetSegment");
    _TreeGetSegment                 = dlsym(_TreeShr, "_TreeGetSegment");
    TreeGetSegments                 = dlsym(_TreeShr, "TreeGetSegments");
    _TreeGetSegments                = dlsym(_TreeShr, "_TreeGetSegments");
    TreeGetSegmentInfo              = dlsym(_TreeShr, "TreeGetSegmentInfo");
    _TreeGetSegmentInfo             = dlsym(_TreeShr, "_TreeGetSegmentInfo");
    TreeGetSegments                 = dlsym(_TreeShr, "TreeGetSegments");
    _TreeGetSegments                = dlsym(_TreeShr, "_TreeGetSegments");
    TreeGetSegmentTimes             = dlsym(_TreeShr, "TreeGetSegmentTimes");
    _TreeGetSegmentTimes            = dlsym(_TreeShr, "_TreeGetSegmentTimes");
    TreeGetSegmentTimesXd           = dlsym(_TreeShr, "TreeGetSegmentTimesXd");
    _TreeGetSegmentTimesXd          = dlsym(_TreeShr, "_TreeGetSegmentTimesXd");
    TreeGetXNci                     = dlsym(_TreeShr, "TreeGetXNci");
    _TreeGetXNci                    = dlsym(_TreeShr, "_TreeGetXNci");
    TreeSetXNci                     = dlsym(_TreeShr, "TreeSetXNci");
    _TreeSetXNci                    = dlsym(_TreeShr, "_TreeSetXNci");
    TreeSetViewDate                 = dlsym(_TreeShr, "TreeSetViewDate");
    _TreeSetViewDate                = dlsym(_TreeShr, "_TreeSetViewDate");
    TreeSetCurrentShotId            = dlsym(_TreeShr, "TreeSetCurrentShotId");
    TreeGetCurrentShotId            = dlsym(_TreeShr, "TreeGetCurrentShotId");
    TreeSetDbiItm                   = dlsym(_TreeShr, "TreeSetDbiItm");
    TreeGetDatafileSize             = dlsym(_TreeShr, "TreeGetDatafileSize");
    _TreeGetDatafileSize            = dlsym(_TreeShr, "_TreeGetDatafileSize");
    TreeFindNodeTagsDsc             = dlsym(_TreeShr, "TreeFindNodeTagsDsc");
    TreeFindTagWildDsc              = dlsym(_TreeShr, "TreeFindTagWildDsc");
    _TreeGetSegmentScale            = dlsym(_TreeShr, "_TreeGetSegmentScale");
    TreeGetSegmentScale             = dlsym(_TreeShr, "TreeGetSegmentScale");
    _TreeSetSegmentScale            = dlsym(_TreeShr, "_TreeSetSegmentScale");
    TreeSetSegmentScale             = dlsym(_TreeShr, "TreeSetSegmentScale");
    _TreeExecute                    = dlsym(_TreeShr, "_TreeExecute");
    _TreeEvaluate                   = dlsym(_TreeShr, "_TreeEvaluate");
    _TreeDecompile                  = dlsym(_TreeShr, "_TreeDecompile");
    _TreeCompile                    = dlsym(_TreeShr, "_TreeCompile");
    TreeCtxPush                     = dlsym(_TreeShr, "TreeCtxPush");
    TreeCtxPop                      = dlsym(_TreeShr, "TreeCtxPop");
    TreeDbidPush                    = dlsym(_TreeShr, "TreeDbidPush");
    TreeDbidPop                     = dlsym(_TreeShr, "TreeDbidPop");

    return 1;
}

static void FreeTreeShr() {
    dlclose(_TreeShr);
    _TreeShr = NULL;
}
