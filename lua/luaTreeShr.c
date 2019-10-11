#include "luaTreeShr.h"
#include <lualib.h>
#include <lauxlib.h>
#include <stdio.h>
#include <string.h>
#include <treeshrload.h>

void checkResult(lua_State * L, int result);

void push_Descriptor(lua_State * L, mdsdsc_t * dsc);

int luaopen_Tree(lua_State * L);
void push_Tree(lua_State * L, void * dbid, const char * tree, int shot);
static int lua_Tree__tostring(lua_State * L);
static int lua_Tree_open(lua_State * L);
static int lua_Tree_edit(lua_State * L);
static int lua_Tree_close(lua_State * L);
static int lua_Tree_write(lua_State * L);
static int lua_Tree_getCurrent(lua_State * L);
static int lua_Tree_setCurrent(lua_State * L);
static int lua_Tree_getDatafileSize(lua_State * L);
static int lua_Tree_getNode(lua_State * L);

int luaopen_TreeNode(lua_State * L);
void push_TreeNode(lua_State * L, void * dbid, int nid);
static int lua_TreeNode__tostring(lua_State * L);
static int lua_TreeNode_getRecord(lua_State * L);
static int lua_TreeNode_isOn(lua_State * L);
static int lua_TreeNode_turnOn(lua_State * L);
static int lua_TreeNode_turnOff(lua_State * L);

int luaopen_Dimension(lua_State * L);
void push_Dimension(lua_State * L);
static int lua_Dimension__tostring(lua_State * L);
static int lua_Dimension_new(lua_State * L);

int luaopen_Signal(lua_State * L);
void push_Signal(lua_State * L, mdsdsc_t * data, mdsdsc_t * raw, mdsdsc_t ** dimensions);
static int lua_Signal__tostring(lua_State * L);
static int lua_Signal_new(lua_State * L);

void load_TreeShr(lua_State * L)
{
    if ((LoadTreeShr() & 1) == 0) {
        luaL_error(L, "Failed to load TreeShr");
    }

    luaopen_Tree(L);
    luaopen_TreeNode(L);
    luaopen_Signal(L);
}

void free_TreeShr(lua_State * L)
{
    FreeTreeShr();
}

void checkResult(lua_State * L, int result)
{
    if ((result & 1) == 0) {
        switch (result) {
        case 265388218:
            luaL_error(L, "Data corrupted, cannot read record.");
            break;
        case 265388258:
            luaL_error(L, "No data available for this node.");
            break;
        case 265392114:
            luaL_error(L, "Cannot locate tree files on paths from 'treename'_path or default_tree_path environment variables.");
            break;
        case 265392034:
            luaL_error(L, "Operation not successful.");
            break;
        case 265392138:
            luaL_error(L, "No current shot number set for this tree.");
            break;
        case 265392146:
            luaL_error(L, "Error opening tree file read-write.");
            break;
        case 265392154:
            luaL_error(L, "Error opening tree file read-only.");
            break;
        case 265392162:
            luaL_error(L, "Error creating new tree file.");
            break;
        default:
            luaL_error(L, "MDSplus Error: %d", result);
        }
    }
}

void push_Descriptor(lua_State * L, mdsdsc_t * dsc)
{
    if (dsc->class == CLASS_A) {
        mdsdsc_a_t * a = (mdsdsc_a_t *)dsc;
        unsigned count = a->arsize / a->length;

#       define PUSH_INTEGER_ARRAY(T)                                        \
            do {                                                            \
                T * data = (T *)a->pointer;                                 \
                lua_createtable(L, count, 0);                               \
                for (unsigned i = 0; i < count; ++i) {                      \
                    lua_pushinteger(L, data[i]);                            \
                    lua_rawseti(L, -2, i + 1);                              \
                }                                                           \
            } while (0)

#       define PUSH_NUMBER_ARRAY(T)                                         \
            do {                                                            \
                T * data = (T *)a->pointer;                                 \
                lua_createtable(L, count, 0);                               \
                for (unsigned i = 0; i < count; ++i) {                      \
                    lua_pushnumber(L, data[i]);                             \
                    lua_rawseti(L, -2, i + 1);                              \
                }                                                           \
            } while (0)

        switch (a->dtype) {
        case DTYPE_B:  PUSH_INTEGER_ARRAY(int8_t); break;
        case DTYPE_BU: PUSH_INTEGER_ARRAY(uint8_t); break;
        case DTYPE_W:  PUSH_INTEGER_ARRAY(int16_t); break;
        case DTYPE_WU: PUSH_INTEGER_ARRAY(uint16_t); break;
        case DTYPE_L:  PUSH_INTEGER_ARRAY(int32_t); break;
        case DTYPE_LU: PUSH_INTEGER_ARRAY(uint32_t); break;
        case DTYPE_Q:  PUSH_INTEGER_ARRAY(int64_t); break;
        case DTYPE_QU: PUSH_INTEGER_ARRAY(uint64_t); break;
        case DTYPE_F:  PUSH_NUMBER_ARRAY(float); break;
        case DTYPE_D:  PUSH_NUMBER_ARRAY(double); break;
        default:
            luaL_error(L, "class A, dtype %d not supported", dsc->dtype);
        }

#       undef PUSH_NUMBER
#       undef PUSH_INTEGER

    }
    else if (dsc->class == CLASS_S) {
        mdsdsc_s_t * s = (mdsdsc_s_t *)dsc;

#       define PUSH_INTEGER(T)                                          \
            do {                                                        \
                T data = (T)*s->pointer;                                \
                lua_pushinteger(L, data);                               \
            } while (0)
        
#       define PUSH_NUMBER(T)                                           \
            do {                                                        \
                T data = (T)*s->pointer;                                \
                lua_pushnumber(L, data);                                \
            } while (0)

        switch (s->dtype) {
        case DTYPE_B:  PUSH_INTEGER(int8_t); break;
        case DTYPE_BU: PUSH_INTEGER(uint8_t); break;
        case DTYPE_W:  PUSH_INTEGER(int16_t); break;
        case DTYPE_WU: PUSH_INTEGER(uint16_t); break;
        case DTYPE_L:  PUSH_INTEGER(int32_t); break;
        case DTYPE_LU: PUSH_INTEGER(uint32_t); break;
        case DTYPE_Q:  PUSH_INTEGER(int64_t); break;
        case DTYPE_QU: PUSH_INTEGER(uint64_t); break;
        case DTYPE_F:  PUSH_NUMBER(float); break;
        case DTYPE_D:  PUSH_NUMBER(double); break;
        case DTYPE_T:
            if (s->length == 0) {
                lua_pushstring(L, "");
            }
            else {
                lua_pushlstring(L, (char *)s->pointer, s->length);
            }
            break;
        default:
            luaL_error(L, "class S, dtype %d not supported", dsc->dtype);
        }

#       undef PUSH_NUMBER
#       undef PUSH_INTEGER

    }
    else if (dsc->class == CLASS_R) {
        mdsdsc_r_t * r = (mdsdsc_r_t *)dsc;
        switch (r->dtype) {
        case DTYPE_SIGNAL:
            {
                mds_signal_t * data = (mds_signal_t *)dsc;
                if (data) {
                    push_Signal(L, data->data, data->raw, data->dimensions);
                }
            }
            break;
        default:
            luaL_error(L, "class R, dtype %d not supported", dsc->dtype);
        }
    }
    else {
        luaL_error(L, "class %d not supported", dsc->class);
    }
}

int luaopen_Tree(lua_State * L)
{
    if (luaL_newmetatable(L, "Tree")) {
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        static const luaL_Reg methods[] = {
            { "__tostring",         lua_Tree__tostring },
            { "__gc",               lua_Tree_close },
            { "close",              lua_Tree_close },
            { "write",              lua_Tree_write },
            { "getCurrent",         lua_Tree_getCurrent },
            { "setCurrent",         lua_Tree_setCurrent },
            { "getDatafileSize",    lua_Tree_getDatafileSize },
            { "getNode",            lua_Tree_getNode },
            { NULL, NULL }
        };

        luaL_setfuncs(L, methods, 0);
        lua_pop(L, 1);
    }

    static const luaL_Reg functions[] = {
        { "open", lua_Tree_open },
        { "edit", lua_Tree_edit },
        { NULL, NULL }
    };

    lua_pushstring(L, "Tree");
    luaL_newlib(L, functions);
    lua_settable(L, -3);

    return 1;
}

void push_Tree(lua_State * L, void * dbid, const char * tree, int shot) 
{
    lua_createtable(L, 0, 3);

    lua_pushinteger(L, (lua_Integer)dbid);
    lua_setfield(L, -2, "dbid");

    lua_pushstring(L, tree);
    lua_setfield(L, -2, "tree");

    lua_pushinteger(L, shot);
    lua_setfield(L, -2, "shot");

    luaL_getmetatable(L, "Tree");
    lua_setmetatable(L, -2);
}

static int lua_Tree__tostring(lua_State * L)
{
    luaL_checktype(L, 1, LUA_TTABLE);

    lua_getfield(L, 1, "tree");
    lua_getfield(L, 1, "shot");

    char const * tree = luaL_checkstring(L, -2);
    int shot = luaL_checkinteger(L, -1);

    lua_pop(L, 1);

    lua_pushfstring(L, "Tree(\"%s\", %d, \"%s\")", tree, shot, "NORMAL");

    return 1;
}

static int lua_Tree_open(lua_State * L)
{
    char const * tree = luaL_checkstring(L, 1);
    int shot          = luaL_optinteger(L, 2, 0);
    int flags         = luaL_optinteger(L, 3, 0);

    void * dbid = NULL;
    int result = _TreeOpen(&dbid, tree, shot, flags);
    checkResult(L, result);

    push_Tree(L, dbid, tree, shot);
    
    lua_pushinteger(L, result);

    return 2;
}

static int lua_Tree_edit(lua_State * L)
{
    char const * tree = luaL_checkstring(L, 1);
    int shot          = luaL_optinteger(L, 2, 0);

    void * dbid = NULL;
    int result = _TreeOpenEdit(&dbid, tree, shot);
    checkResult(L, result);

    push_Tree(L, dbid, tree, shot);
    
    lua_pushinteger(L, result);

    return 2;
}

static int lua_Tree_close(lua_State * L)
{
    luaL_checktype(L, 1, LUA_TTABLE);

    lua_getfield(L, 1, "tree");
    char const * tree = luaL_checkstring(L, -1);

    lua_getfield(L, 1, "shot");
    int shot = luaL_checkinteger(L, -1);

    lua_getfield(L, 1, "dbid");
    void * dbid = (void *)luaL_checkinteger(L, -1);

    int result = _TreeClose(&dbid, tree, shot);
    checkResult(L, result);

    lua_pop(L, 3);
    
    lua_pushinteger(L, result);

    return 1;
}

static int lua_Tree_write(lua_State * L)
{
    luaL_checktype(L, 1, LUA_TTABLE);

    lua_getfield(L, 1, "tree");
    char const * tree = luaL_checkstring(L, -1);

    lua_getfield(L, 1, "shot");
    int shot = luaL_checkinteger(L, -1);

    lua_getfield(L, 1, "dbid");
    void * dbid = (void *)luaL_checkinteger(L, -1);

    int result = _TreeWriteTree(&dbid, tree, shot);
    checkResult(L, result);

    lua_pop(L, 3);
    
    lua_pushinteger(L, result);

    return 1;
}

static int lua_Tree_getCurrent(lua_State * L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    
    lua_getfield(L, 1, "tree");
    char const * tree = luaL_checkstring(L, -1);

    int shot = TreeGetCurrentShotId(tree);
    
    lua_pop(L, 1);

    lua_pushinteger(L, shot);

    return 1;
}

static int lua_Tree_setCurrent(lua_State * L)
{
    int shot = luaL_checkinteger(L, 2);

    luaL_checktype(L, 1, LUA_TTABLE);
    
    lua_getfield(L, 1, "tree");
    char const * tree = luaL_checkstring(L, -1);

    int result = TreeSetCurrentShotId(tree, shot);
    checkResult(L, result);

    lua_pop(L, 1);
    
    lua_pushinteger(L, result);

    return 1;
}

static int lua_Tree_getDatafileSize(lua_State * L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    
    lua_getfield(L, 1, "dbid");
    void * dbid = (void *)luaL_checkinteger(L, -1);

    lua_pop(L, 1);

    lua_pushinteger(L, _TreeGetDatafileSize(dbid));

    return 1;
}

static int lua_Tree_getNode(lua_State * L)
{
    const char * path = luaL_checkstring(L, 2);

    luaL_checktype(L, 1, LUA_TTABLE);

    lua_getfield(L, 1, "dbid");
    void * dbid = (void *)luaL_checkinteger(L, -1);

    int nid = 0;
    int result = _TreeFindNode(dbid, path, &nid);
    checkResult(L, result);

    lua_pop(L, 1);

    push_TreeNode(L, dbid, nid);

    lua_pushinteger(L, result);

    return 2;
}

int luaopen_TreeNode(lua_State * L)
{
    if (luaL_newmetatable(L, "TreeNode")) {
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        static const luaL_Reg metamethods[] = {
            { "__tostring",     lua_TreeNode__tostring },
            { "getRecord",      lua_TreeNode_getRecord },
            { "isOn",           lua_TreeNode_isOn },
            { "turnOn",         lua_TreeNode_turnOn },
            { "turnOff",        lua_TreeNode_turnOff },
            { NULL, NULL }
        };

        luaL_setfuncs(L, metamethods, 0);
        lua_pop(L, 1);
    }

    static const luaL_Reg methods[] = {
        { NULL, NULL }
    };

    lua_pushstring(L, "TreeNode");
    luaL_newlib(L, methods);
    lua_settable(L, -3);

    return 1;
}

void push_TreeNode(lua_State * L, void * dbid, int nid)
{
    lua_createtable(L, 0, 2);

    lua_pushinteger(L, (lua_Integer)dbid);
    lua_setfield(L, -2, "dbid");

    lua_pushinteger(L, nid);
    lua_setfield(L, -2, "nid");

    luaL_getmetatable(L, "TreeNode");
    lua_setmetatable(L, -2);
}

static int lua_TreeNode__tostring(lua_State * L)
{
    luaL_checktype(L, 1, LUA_TTABLE);

    lua_getfield(L, 1, "nid");
    int nid = luaL_checkinteger(L, -1);

    lua_getfield(L, 1, "dbid");
    void * dbid = (void *)luaL_checkinteger(L, -1);

    char * path = _TreeGetPath(dbid, nid);

    lua_pushfstring(L, "TreeNode(\"%s\")", path);

    TreeFree(path);

    return 1;
}

static int lua_TreeNode_getRecord(lua_State * L)
{
    luaL_checktype(L, 1, LUA_TTABLE);

    lua_getfield(L, 1, "nid");
    int nid = luaL_checkinteger(L, -1);

    lua_getfield(L, 1, "dbid");
    void * dbid = (void *)luaL_checkinteger(L, -1);

    lua_pop(L, 2);

    mdsdsc_xd_t xd;
    xd.dtype = 24;
    xd.class = 192;
    xd.pointer = NULL;
    int result = _TreeGetRecord(dbid, nid, &xd);
    checkResult(L, result);

    if (xd.pointer) {
        push_Descriptor(L, xd.pointer);
    }

    //MdsFree1Dx(xd, NULL);

    lua_pushinteger(L, result);

    return 2;
}

static int lua_TreeNode_isOn(lua_State * L)
{
    luaL_checktype(L, 1, LUA_TTABLE);

    lua_getfield(L, 1, "nid");
    lua_getfield(L, 1, "dbid");
    
    int nid = luaL_checkinteger(L, -2);
    void * dbid = (void *)luaL_checkinteger(L, -1);

    int on = _TreeIsOn(dbid, nid);

    lua_pop(L, 2);

    lua_pushboolean(L, on);

    return 1;
}

static int lua_TreeNode_turnOn(lua_State * L)
{
    luaL_checktype(L, 1, LUA_TTABLE);

    lua_getfield(L, 1, "nid");
    lua_getfield(L, 1, "dbid");
    
    int nid = luaL_checkinteger(L, -2);
    void * dbid = (void *)luaL_checkinteger(L, -1);

    _TreeTurnOn(dbid, nid);

    lua_pop(L, 2);

    return 0;
}

static int lua_TreeNode_turnOff(lua_State * L)
{
    luaL_checktype(L, 1, LUA_TTABLE);

    lua_getfield(L, 1, "nid");
    lua_getfield(L, 1, "dbid");
    
    int nid = luaL_checkinteger(L, -2);
    void * dbid = (void *)luaL_checkinteger(L, -1);

    _TreeTurnOff(dbid, nid);

    return 0;
}

int luaopen_Dimension(lua_State * L)
{
    if (luaL_newmetatable(L, "Dimension")) {
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        static const luaL_Reg metamethods[] = {
            { "__tostring",     lua_Dimension__tostring },
            { NULL, NULL }
        };

        luaL_setfuncs(L, metamethods, 0);
        lua_pop(L, 1);
    }

    static const luaL_Reg methods[] = {
        { "new", lua_Dimension_new },
        { NULL, NULL }
    };

    lua_pushstring(L, "Dimension");
    luaL_newlib(L, methods);
    lua_settable(L, -3);

    return 1;
}

void push_Dimension(lua_State * L) 
{
    lua_createtable(L, 0, 0);

    luaL_getmetatable(L, "Dimension");
    lua_setmetatable(L, -2);
}

static int lua_Dimension__tostring(lua_State * L)
{
    luaL_checktype(L, 1, LUA_TTABLE);

    lua_pushfstring(L, "Dimension()");

    return 1;
}

static int lua_Dimension_new(lua_State * L)
{
    push_Dimension(L);

    return 1;
}

int luaopen_Signal(lua_State * L)
{
    if (luaL_newmetatable(L, "Signal")) {
        lua_pushvalue(L, -1);
        lua_setfield(L, -2, "__index");

        static const luaL_Reg metamethods[] = {
            { "__tostring",     lua_Signal__tostring },
            { NULL, NULL }
        };

        luaL_setfuncs(L, metamethods, 0);
        lua_pop(L, 1);
    }

    static const luaL_Reg methods[] = {
        { "new", lua_Signal_new },
        { NULL, NULL }
    };

    lua_pushstring(L, "Signal");
    luaL_newlib(L, methods);
    lua_settable(L, -3);

    return 1;
}

void push_Signal(lua_State * L, mdsdsc_t * data, mdsdsc_t * raw, mdsdsc_t ** dimensions)
{
    lua_createtable(L, 0, 3);

    if (data) {
        push_Descriptor(L, data);
        lua_setfield(L, -2, "data");
    }

    if (raw) {
        push_Descriptor(L, raw);
        lua_setfield(L, -2, "raw");
    }

    if (dimensions) {
        push_Descriptor(L, dimensions[0]);
        lua_setfield(L, -2, "dimension");
    }

    luaL_getmetatable(L, "Signal");
    lua_setmetatable(L, -2);
}

static int lua_Signal__tostring(lua_State * L)
{
    luaL_checktype(L, 1, LUA_TTABLE);

    lua_pushfstring(L, "Signal()");

    return 1;
}

static int lua_Signal_new(lua_State * L)
{
    push_Signal(L, NULL, NULL, NULL);

    return 1;
}
