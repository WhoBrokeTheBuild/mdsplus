#include "luaTreeShr.h"
#include <lualib.h>
#include <lauxlib.h>

int MDSplus__gc(lua_State * L)
{
    free_TreeShr(L);

    return 0;
}

int luaopen_MDSplus(lua_State * L)
{
    lua_newtable(L);

    luaL_newmetatable(L, "MDSplus");
    lua_pushvalue(L, -1);
    lua_setfield(L, -2, "__index");

    static const luaL_Reg methods[] = {
        { "__gc", MDSplus__gc },
        { NULL, NULL }
    };

    luaL_setfuncs(L, methods, 0);
    lua_setmetatable(L, -2);

    load_TreeShr(L);

    return 1;
}
