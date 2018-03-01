#include "lua.h"
#include "lauxlib.h"
#include <dlfcn.h>

#include "realtalk.h"
#include "rtlua.h"

typedef rt_ref (*rt_ffi_func)(rt_state* rt, rt_ref args);

typedef void (*loader_func)(void*);



int lib_open(lua_State *L) {
    const char* Path    = lua_tostring(L, 1);
    int Flags           = lua_tonumber(L, 2);
    (void)Flags; // not yet implemented
    void* Handle = dlopen(Path, RTLD_NOW);
    lua_pushlightuserdata(L, Handle);
    return 1;
}

int lib_close(lua_State *L) {
    void* Handle = lua_touserdata(L, 1);
    dlclose(Handle);
    return 1;
}

int lib_sym(lua_State *L) {
    void* Handle       = lua_touserdata(L, 1);
    const char* Symbol = lua_tostring(L, 2);
    void* SymbolHandle = dlsym(Handle, Symbol);
    lua_pushlightuserdata(L, SymbolHandle);

    return 1;
}

int lib_call(lua_State *L) {

    void* Handle       = lua_touserdata(L, 1);
    const char* Symbol = lua_tostring(L, 2);

    rt_state* RT = rtlua_rt_from_lua(L, 3);
    rt_ref Arg   = rtlua_ref_from_lua(RT, L, 3);

    rt_ffi_func Function = (rt_ffi_func)dlsym(Handle, Symbol);
    if (Function == NULL) {
        return 0;
    }

    rt_ref Return = Function(RT, Arg);
    rtlua_push_ref(RT, L, Return);

    return 1;
}

int lib_call_loader(lua_State *L) {

    void* LoadToHandle = lua_touserdata(L, 1);
    const char* Symbol = lua_tostring(L, 2);
    void* LoadFromHandle = lua_touserdata(L, 3);

    loader_func Function = (loader_func)dlsym(LoadToHandle, Symbol);
    if (Function == NULL) {
        return 0;
    }

    Function(LoadFromHandle);

    return 0;
}

int luaopen_dylib(lua_State *L) {

    static const struct luaL_Reg lua_functions[] = {
        { "open", lib_open },
        { "close",  lib_close },
        { "sym",  lib_sym },
        { "call",  lib_call },
        { "call_loader",  lib_call_loader },
        { NULL, NULL },
    };

    luaL_newlib(L, lua_functions);

    return 1;
}
