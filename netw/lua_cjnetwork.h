#pragma once

#ifndef COCOS2DX_SCRIPT_LUA_COCOS2DX_SUPPORT_LUA_CJNETWORK_H
#define COCOS2DX_SCRIPT_LUA_COCOS2DX_SUPPORT_LUA_CJNETWORK_H

#ifdef __cplusplus
extern "C" {
#endif
#include "tolua++.h"
#ifdef __cplusplus
}
#endif




TOLUA_API int register_all_cjnetwork(lua_State* tolua_S);

#endif 