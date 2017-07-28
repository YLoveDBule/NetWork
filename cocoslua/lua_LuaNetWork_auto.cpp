#include "lua_LuaNetWork_auto.hpp"
#include "LUAMessager.h"
#include "scripting/lua-bindings/manual/tolua_fix.h"
#include "scripting/lua-bindings/manual/LuaBasicConversions.h"

int lua_NetWork_LUAMessager_onConnectShutdown(lua_State* tolua_S)
{
    int argc = 0;
    LuaNetMessage::LUAMessager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"luant.LUAMessager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaNetMessage::LUAMessager*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_NetWork_LUAMessager_onConnectShutdown'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 2) 
    {
        unsigned short arg0;
        uint16_t arg1;

        ok &= luaval_to_ushort(tolua_S, 2, &arg0, "luant.LUAMessager:onConnectShutdown");

        ok &= luaval_to_uint16(tolua_S, 3,&arg1, "luant.LUAMessager:onConnectShutdown");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_NetWork_LUAMessager_onConnectShutdown'", nullptr);
            return 0;
        }
        bool ret = cobj->onConnectShutdown(arg0, arg1);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "luant.LUAMessager:onConnectShutdown",argc, 2);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_NetWork_LUAMessager_onConnectShutdown'.",&tolua_err);
#endif

    return 0;
}
int lua_NetWork_LUAMessager_pushLString(lua_State* tolua_S)
{
    int argc = 0;
    LuaNetMessage::LUAMessager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"luant.LUAMessager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaNetMessage::LUAMessager*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_NetWork_LUAMessager_pushLString'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        int arg0;
        const char* arg1;
        int arg2;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "luant.LUAMessager:pushLString");

        std::string arg1_tmp; ok &= luaval_to_std_string(tolua_S, 3, &arg1_tmp, "luant.LUAMessager:pushLString"); arg1 = arg1_tmp.c_str();

        ok &= luaval_to_int32(tolua_S, 4,(int *)&arg2, "luant.LUAMessager:pushLString");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_NetWork_LUAMessager_pushLString'", nullptr);
            return 0;
        }
        cobj->pushLString(arg0, arg1, arg2);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "luant.LUAMessager:pushLString",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_NetWork_LUAMessager_pushLString'.",&tolua_err);
#endif

    return 0;
}
int lua_NetWork_LUAMessager_closeConnet(lua_State* tolua_S)
{
    int argc = 0;
    LuaNetMessage::LUAMessager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"luant.LUAMessager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaNetMessage::LUAMessager*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_NetWork_LUAMessager_closeConnet'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "luant.LUAMessager:closeConnet");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_NetWork_LUAMessager_closeConnet'", nullptr);
            return 0;
        }
        cobj->closeConnet(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "luant.LUAMessager:closeConnet",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_NetWork_LUAMessager_closeConnet'.",&tolua_err);
#endif

    return 0;
}
int lua_NetWork_LUAMessager_ask2Subserver(lua_State* tolua_S)
{
    int argc = 0;
    LuaNetMessage::LUAMessager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"luant.LUAMessager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaNetMessage::LUAMessager*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_NetWork_LUAMessager_ask2Subserver'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        int arg0;
        const char* arg1;
        int arg2;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "luant.LUAMessager:ask2Subserver");

        std::string arg1_tmp; ok &= luaval_to_std_string(tolua_S, 3, &arg1_tmp, "luant.LUAMessager:ask2Subserver"); arg1 = arg1_tmp.c_str();

        ok &= luaval_to_int32(tolua_S, 4,(int *)&arg2, "luant.LUAMessager:ask2Subserver");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_NetWork_LUAMessager_ask2Subserver'", nullptr);
            return 0;
        }
        cobj->ask2Subserver(arg0, arg1, arg2);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "luant.LUAMessager:ask2Subserver",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_NetWork_LUAMessager_ask2Subserver'.",&tolua_err);
#endif

    return 0;
}
int lua_NetWork_LUAMessager_dispatchNetMsg(lua_State* tolua_S)
{
    int argc = 0;
    LuaNetMessage::LUAMessager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"luant.LUAMessager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaNetMessage::LUAMessager*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_NetWork_LUAMessager_dispatchNetMsg'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_NetWork_LUAMessager_dispatchNetMsg'", nullptr);
            return 0;
        }
        cobj->dispatchNetMsg();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "luant.LUAMessager:dispatchNetMsg",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_NetWork_LUAMessager_dispatchNetMsg'.",&tolua_err);
#endif

    return 0;
}
int lua_NetWork_LUAMessager_endSendMsg(lua_State* tolua_S)
{
    int argc = 0;
    LuaNetMessage::LUAMessager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"luant.LUAMessager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaNetMessage::LUAMessager*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_NetWork_LUAMessager_endSendMsg'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "luant.LUAMessager:endSendMsg");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_NetWork_LUAMessager_endSendMsg'", nullptr);
            return 0;
        }
        cobj->endSendMsg(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "luant.LUAMessager:endSendMsg",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_NetWork_LUAMessager_endSendMsg'.",&tolua_err);
#endif

    return 0;
}
int lua_NetWork_LUAMessager_houseNetMsg(lua_State* tolua_S)
{
    int argc = 0;
    LuaNetMessage::LUAMessager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"luant.LUAMessager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaNetMessage::LUAMessager*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_NetWork_LUAMessager_houseNetMsg'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 5) 
    {
        int arg0;
        unsigned short arg1;
        unsigned short arg2;
        void* arg3;
        unsigned short arg4;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "luant.LUAMessager:houseNetMsg");

        ok &= luaval_to_ushort(tolua_S, 3, &arg1, "luant.LUAMessager:houseNetMsg");

        ok &= luaval_to_ushort(tolua_S, 4, &arg2, "luant.LUAMessager:houseNetMsg");

        #pragma warning NO CONVERSION TO NATIVE FOR void*
		ok = false;

        ok &= luaval_to_ushort(tolua_S, 6, &arg4, "luant.LUAMessager:houseNetMsg");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_NetWork_LUAMessager_houseNetMsg'", nullptr);
            return 0;
        }
        bool ret = cobj->houseNetMsg(arg0, arg1, arg2, arg3, arg4);
        tolua_pushboolean(tolua_S,(bool)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "luant.LUAMessager:houseNetMsg",argc, 5);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_NetWork_LUAMessager_houseNetMsg'.",&tolua_err);
#endif

    return 0;
}
int lua_NetWork_LUAMessager_beginSendMsg(lua_State* tolua_S)
{
    int argc = 0;
    LuaNetMessage::LUAMessager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"luant.LUAMessager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaNetMessage::LUAMessager*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_NetWork_LUAMessager_beginSendMsg'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 1) 
    {
        int arg0;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "luant.LUAMessager:beginSendMsg");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_NetWork_LUAMessager_beginSendMsg'", nullptr);
            return 0;
        }
        cobj->beginSendMsg(arg0);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "luant.LUAMessager:beginSendMsg",argc, 1);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_NetWork_LUAMessager_beginSendMsg'.",&tolua_err);
#endif

    return 0;
}
int lua_NetWork_LUAMessager_sendMessage(lua_State* tolua_S)
{
    int argc = 0;
    LuaNetMessage::LUAMessager* cobj = nullptr;
    bool ok  = true;
#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"luant.LUAMessager",0,&tolua_err)) goto tolua_lerror;
#endif
    cobj = (LuaNetMessage::LUAMessager*)tolua_tousertype(tolua_S,1,0);
#if COCOS2D_DEBUG >= 1
    if (!cobj)
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_NetWork_LUAMessager_sendMessage'", nullptr);
        return 0;
    }
#endif
    argc = lua_gettop(tolua_S)-1;
    do{
        if (argc == 5) {
            int arg0;
            ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "luant.LUAMessager:sendMessage");

            if (!ok) { break; }
            unsigned short arg1;
            ok &= luaval_to_ushort(tolua_S, 3, &arg1, "luant.LUAMessager:sendMessage");

            if (!ok) { break; }
            unsigned short arg2;
            ok &= luaval_to_ushort(tolua_S, 4, &arg2, "luant.LUAMessager:sendMessage");

            if (!ok) { break; }
            void* arg3;
            #pragma warning NO CONVERSION TO NATIVE FOR void*
		ok = false;

            if (!ok) { break; }
            unsigned int arg4;
            ok &= luaval_to_uint32(tolua_S, 6,&arg4, "luant.LUAMessager:sendMessage");

            if (!ok) { break; }
            bool ret = cobj->sendMessage(arg0, arg1, arg2, arg3, arg4);
            tolua_pushboolean(tolua_S,(bool)ret);
            return 1;
        }
    }while(0);
    ok  = true;
    do{
        if (argc == 3) {
            int arg0;
            ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "luant.LUAMessager:sendMessage");

            if (!ok) { break; }
            unsigned short arg1;
            ok &= luaval_to_ushort(tolua_S, 3, &arg1, "luant.LUAMessager:sendMessage");

            if (!ok) { break; }
            unsigned short arg2;
            ok &= luaval_to_ushort(tolua_S, 4, &arg2, "luant.LUAMessager:sendMessage");

            if (!ok) { break; }
            bool ret = cobj->sendMessage(arg0, arg1, arg2);
            tolua_pushboolean(tolua_S,(bool)ret);
            return 1;
        }
    }while(0);
    ok  = true;
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n",  "luant.LUAMessager:sendMessage",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_NetWork_LUAMessager_sendMessage'.",&tolua_err);
#endif

    return 0;
}
int lua_NetWork_LUAMessager_pushNum(lua_State* tolua_S)
{
    int argc = 0;
    LuaNetMessage::LUAMessager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"luant.LUAMessager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaNetMessage::LUAMessager*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_NetWork_LUAMessager_pushNum'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        int arg0;
        void* arg1;
        unsigned int arg2;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "luant.LUAMessager:pushNum");

        #pragma warning NO CONVERSION TO NATIVE FOR void*
		ok = false;

        ok &= luaval_to_uint32(tolua_S, 4,&arg2, "luant.LUAMessager:pushNum");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_NetWork_LUAMessager_pushNum'", nullptr);
            return 0;
        }
        cobj->pushNum(arg0, arg1, arg2);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "luant.LUAMessager:pushNum",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_NetWork_LUAMessager_pushNum'.",&tolua_err);
#endif

    return 0;
}
int lua_NetWork_LUAMessager_pushLStringShortSize(lua_State* tolua_S)
{
    int argc = 0;
    LuaNetMessage::LUAMessager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"luant.LUAMessager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaNetMessage::LUAMessager*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_NetWork_LUAMessager_pushLStringShortSize'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 3) 
    {
        int arg0;
        const char* arg1;
        int32_t arg2;

        ok &= luaval_to_int32(tolua_S, 2,(int *)&arg0, "luant.LUAMessager:pushLStringShortSize");

        std::string arg1_tmp; ok &= luaval_to_std_string(tolua_S, 3, &arg1_tmp, "luant.LUAMessager:pushLStringShortSize"); arg1 = arg1_tmp.c_str();

        ok &= luaval_to_int32(tolua_S, 4,&arg2, "luant.LUAMessager:pushLStringShortSize");
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_NetWork_LUAMessager_pushLStringShortSize'", nullptr);
            return 0;
        }
        cobj->pushLStringShortSize(arg0, arg1, arg2);
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "luant.LUAMessager:pushLStringShortSize",argc, 3);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_NetWork_LUAMessager_pushLStringShortSize'.",&tolua_err);
#endif

    return 0;
}
int lua_NetWork_LUAMessager_closeAllConnect(lua_State* tolua_S)
{
    int argc = 0;
    LuaNetMessage::LUAMessager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif


#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertype(tolua_S,1,"luant.LUAMessager",0,&tolua_err)) goto tolua_lerror;
#endif

    cobj = (LuaNetMessage::LUAMessager*)tolua_tousertype(tolua_S,1,0);

#if COCOS2D_DEBUG >= 1
    if (!cobj) 
    {
        tolua_error(tolua_S,"invalid 'cobj' in function 'lua_NetWork_LUAMessager_closeAllConnect'", nullptr);
        return 0;
    }
#endif

    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_NetWork_LUAMessager_closeAllConnect'", nullptr);
            return 0;
        }
        cobj->closeAllConnect();
        lua_settop(tolua_S, 1);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "luant.LUAMessager:closeAllConnect",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_NetWork_LUAMessager_closeAllConnect'.",&tolua_err);
#endif

    return 0;
}
int lua_NetWork_LUAMessager_getInstance(lua_State* tolua_S)
{
    int argc = 0;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif

#if COCOS2D_DEBUG >= 1
    if (!tolua_isusertable(tolua_S,1,"luant.LUAMessager",0,&tolua_err)) goto tolua_lerror;
#endif

    argc = lua_gettop(tolua_S) - 1;

    if (argc == 0)
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_NetWork_LUAMessager_getInstance'", nullptr);
            return 0;
        }
        LuaNetMessage::LUAMessager* ret = LuaNetMessage::LUAMessager::getInstance();
        object_to_luaval<LuaNetMessage::LUAMessager>(tolua_S, "luant.LUAMessager",(LuaNetMessage::LUAMessager*)ret);
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d\n ", "luant.LUAMessager:getInstance",argc, 0);
    return 0;
#if COCOS2D_DEBUG >= 1
    tolua_lerror:
    tolua_error(tolua_S,"#ferror in function 'lua_NetWork_LUAMessager_getInstance'.",&tolua_err);
#endif
    return 0;
}
int lua_NetWork_LUAMessager_constructor(lua_State* tolua_S)
{
    int argc = 0;
    LuaNetMessage::LUAMessager* cobj = nullptr;
    bool ok  = true;

#if COCOS2D_DEBUG >= 1
    tolua_Error tolua_err;
#endif



    argc = lua_gettop(tolua_S)-1;
    if (argc == 0) 
    {
        if(!ok)
        {
            tolua_error(tolua_S,"invalid arguments in function 'lua_NetWork_LUAMessager_constructor'", nullptr);
            return 0;
        }
        cobj = new LuaNetMessage::LUAMessager();
        tolua_pushusertype(tolua_S,(void*)cobj,"luant.LUAMessager");
        tolua_register_gc(tolua_S,lua_gettop(tolua_S));
        return 1;
    }
    luaL_error(tolua_S, "%s has wrong number of arguments: %d, was expecting %d \n", "luant.LUAMessager:LUAMessager",argc, 0);
    return 0;

#if COCOS2D_DEBUG >= 1
    tolua_error(tolua_S,"#ferror in function 'lua_NetWork_LUAMessager_constructor'.",&tolua_err);
#endif

    return 0;
}

static int lua_NetWork_LUAMessager_finalize(lua_State* tolua_S)
{
    printf("luabindings: finalizing LUA object (LUAMessager)");
    return 0;
}

int lua_register_NetWork_LUAMessager(lua_State* tolua_S)
{
    tolua_usertype(tolua_S,"luant.LUAMessager");
    tolua_cclass(tolua_S,"LUAMessager","luant.LUAMessager","",nullptr);

    tolua_beginmodule(tolua_S,"LUAMessager");
        tolua_function(tolua_S,"new",lua_NetWork_LUAMessager_constructor);
        tolua_function(tolua_S,"onConnectShutdown",lua_NetWork_LUAMessager_onConnectShutdown);
        tolua_function(tolua_S,"pushLString",lua_NetWork_LUAMessager_pushLString);
        tolua_function(tolua_S,"closeConnet",lua_NetWork_LUAMessager_closeConnet);
        tolua_function(tolua_S,"ask2Subserver",lua_NetWork_LUAMessager_ask2Subserver);
        tolua_function(tolua_S,"dispatchNetMsg",lua_NetWork_LUAMessager_dispatchNetMsg);
        tolua_function(tolua_S,"endSendMsg",lua_NetWork_LUAMessager_endSendMsg);
        tolua_function(tolua_S,"houseNetMsg",lua_NetWork_LUAMessager_houseNetMsg);
        tolua_function(tolua_S,"beginSendMsg",lua_NetWork_LUAMessager_beginSendMsg);
        tolua_function(tolua_S,"sendMessage",lua_NetWork_LUAMessager_sendMessage);
        tolua_function(tolua_S,"pushNum",lua_NetWork_LUAMessager_pushNum);
        tolua_function(tolua_S,"pushLStringShortSize",lua_NetWork_LUAMessager_pushLStringShortSize);
        tolua_function(tolua_S,"closeAllConnect",lua_NetWork_LUAMessager_closeAllConnect);
        tolua_function(tolua_S,"getInstance", lua_NetWork_LUAMessager_getInstance);
    tolua_endmodule(tolua_S);
    std::string typeName = typeid(LuaNetMessage::LUAMessager).name();
    g_luaType[typeName] = "luant.LUAMessager";
    g_typeCast["LUAMessager"] = "luant.LUAMessager";
    return 1;
}
TOLUA_API int register_all_NetWork(lua_State* tolua_S)
{
	tolua_open(tolua_S);
	
	tolua_module(tolua_S,"luant",0);
	tolua_beginmodule(tolua_S,"luant");

	lua_register_NetWork_LUAMessager(tolua_S);

	tolua_endmodule(tolua_S);
	return 1;
}

