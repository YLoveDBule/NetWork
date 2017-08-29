#pragma once 
#ifndef __LUA_NETWORK_MANUAL__
#define __LUA_NETWORK_MANUAL__
#ifdef __cplusplus
extern "C" {
#endif
#include "tolua++.h"
#ifdef __cplusplus
}
#endif

int register_all_luaNetWorkEx(lua_State* tolua_S);
#endif