#include "lua_NetWork_manual.h"
#include "LUAMessager.h"
#include "FmUtils.h"
#include "DataTypeDefine.h"

#define LEN_MD5 33

#if defined(__GNUC__) && ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1)))
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#elif _MSC_VER >= 1400 //vs 2005 or higher
#pragma warning (push)
#pragma warning (disable: 4996)
#endif


#pragma pack(1)

int  gb2312toutf8(char *sourcebuf, size_t sourcelen, char *destbuf, size_t destlen)
{
	return FmUtils::getInstance()->convertstr("gb18030", "utf-8", sourcebuf, sourcelen, destbuf, destlen);
}

static int CN2Ttf8(char *input, char *output, size_t outlen)
{
	if (strlen(input) > 0) {
		return gb2312toutf8(input, strlen(input), output, outlen);
	}

	return -1;
}

static int utf82gb(char *input, char *output, size_t outlen)
{
	if (strlen(input) > 0) {
		return FmUtils::getInstance()->convertstr("utf-8", "gb18030", input, strlen(input), output, outlen);
	}

	return -1;
}

static int tolua_luaNetwork_pushInt64(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isnumber(tolua_S, 1, 0, &tolua_err) ||
		!tolua_isstring(tolua_S, 2, 0, &tolua_err) ||
		!tolua_isnumber(tolua_S, 3, 0, &tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		int sid = ((int)tolua_tonumber(tolua_S, 1, 0));
		long long num = ((long long)tolua_tonumber(tolua_S, 2, 0));
		int size = ((int)tolua_tonumber(tolua_S, 3, 0));

		if (size > 0) {
			LuaNetMessage::LUAMessager::getInstance()->pushNum(sid, &num, size);
		}

		return 0;
	}

#ifndef TOLUA_RELEASE
	tolua_lerror :
				 tolua_error(tolua_S, "#ferror in function 'tolua_luaNetwork_pushInt64'.", &tolua_err);
				 return 0;
#endif
}

static int tolua_luaNetwork_pushInt(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isnumber(tolua_S, 1, 0, &tolua_err) ||
		!tolua_isstring(tolua_S, 2, 0, &tolua_err) ||
		!tolua_isnumber(tolua_S, 3, 0, &tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		int sid = ((int)tolua_tonumber(tolua_S, 1, 0));
		int num = ((int)tolua_tonumber(tolua_S, 2, 0));
		int size = ((int)tolua_tonumber(tolua_S, 3, 0));

		if (size > 0) {
			LuaNetMessage::LUAMessager::getInstance()->pushNum(sid, &num, size);
		}

		return 0;
	}

#ifndef TOLUA_RELEASE
	tolua_lerror :
				 tolua_error(tolua_S, "#ferror in function 'tolua_luaNetwork_pushInt'.", &tolua_err);
				 return 0;
#endif
}

static int tolua_luaNetwork_pushShort(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isnumber(tolua_S, 1, 0, &tolua_err) ||
		!tolua_isstring(tolua_S, 2, 0, &tolua_err) ||
		!tolua_isnumber(tolua_S, 3, 0, &tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		int sid = ((int)tolua_tonumber(tolua_S, 1, 0));
		short num = ((short)tolua_tonumber(tolua_S, 2, 0));
		int size = ((int)tolua_tonumber(tolua_S, 3, 0));

		if (size > 0) {
			LuaNetMessage::LUAMessager::getInstance()->pushNum(sid, (void*)&num, size);
		}

		return 0;
	}

#ifndef TOLUA_RELEASE
	tolua_lerror :
				 tolua_error(tolua_S, "#ferror in function 'tolua_luaNetwork_pushShort'.", &tolua_err);
				 return 0;
#endif
}

static int tolua_luaNetwork_pushLong(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isnumber(tolua_S, 1, 0, &tolua_err) ||
		!tolua_isstring(tolua_S, 2, 0, &tolua_err) ||
		!tolua_isnumber(tolua_S, 3, 0, &tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		int sid = ((int)tolua_tonumber(tolua_S, 1, 0));
		unsigned long num = ((unsigned long)tolua_tonumber(tolua_S, 2, 0));
		int size = ((int)tolua_tonumber(tolua_S, 3, 0));

		if (size > 0) {
			LuaNetMessage::LUAMessager::getInstance()->pushNum(sid, (void*)&num, size);
		}

		return 0;
	}

#ifndef TOLUA_RELEASE
	tolua_lerror :
				 tolua_error(tolua_S, "#ferror in function 'tolua_luaNetwork_pushLong'.", &tolua_err);
				 return 0;
#endif
}

static int tolua_luaNetwork_pushLString(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isnumber(tolua_S, 1, 0, &tolua_err) ||
		!tolua_isstring(tolua_S, 2, 0, &tolua_err) ||
		!tolua_isnumber(tolua_S, 3, 0, &tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		int sid = ((int)tolua_tonumber(tolua_S, 1, 0));
		size_t size = ((size_t)tolua_tonumber(tolua_S, 3, 0));
		char* buff = ((char*)tolua_tostring(tolua_S, 2, 0));
		//const char* buff = (const char*)lua_tolstring(tolua_S, 2, &size);
		buff[strlen(buff) + 1] = '\0';

		LuaNetMessage::LUAMessager::getInstance()->pushLString(sid, buff, size);

		return 0;
	}

#ifndef TOLUA_RELEASE
	tolua_lerror :
				 tolua_error(tolua_S, "#ferror in function 'tolua_luaNetwork_pushLString'.", &tolua_err);
				 return 0;
#endif
}

static int tolua_luaNetwork_pushLStringShortSize(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isnumber(tolua_S, 1, 0, &tolua_err) ||
		!tolua_isstring(tolua_S, 2, 0, &tolua_err) ||
		!tolua_isnumber(tolua_S, 3, 0, &tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		int sid = ((int)tolua_tonumber(tolua_S, 1, 0));
		char* buff = ((char*)tolua_tostring(tolua_S, 2, 0));
		short size = ((short)tolua_tonumber(tolua_S, 3, 0));

		LuaNetMessage::LUAMessager::getInstance()->pushLStringShortSize(sid, buff, size);

		return 0;
	}

#ifndef TOLUA_RELEASE
	tolua_lerror :
				 tolua_error(tolua_S, "#ferror in function 'tolua_luaNetwork_pushLStringShortSize'.", &tolua_err);
				 return 0;
#endif
}

static int tolua_luaNetwork_byte2int(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isstring(tolua_S, 1, 0, &tolua_err) ||
		!tolua_isnumber(tolua_S, 2, 0, &tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		char* sid = ((char*)tolua_tostring(tolua_S, 1, 0));
		int len = (int)tolua_tonumber(tolua_S, 2, 0);

		switch (len)
		{
		case 1:
		{
			char value = *(char*)sid;
			lua_pushnumber(tolua_S, (lua_Number)value);
			break;
		}
		case 2:
		{
			short value = *(short*)sid;
			lua_pushnumber(tolua_S, (lua_Number)value);
			break;
		}
		case 4:
		{
			int value = *(int*)sid;
			lua_pushnumber(tolua_S, (lua_Number)value);
			break;
		}
		case 8:
		{
			long long value = *(long long*)sid;
			lua_pushnumber(tolua_S, (lua_Number)value);
			break;
		}
		default:
		{
			char buff[512] = { 0 };
			if (CN2Ttf8(sid, buff, len * 3) != -1) {
				//if (buff && strlen(buff) > 0) {
				int size = strlen(buff);
				lua_pushlstring(tolua_S, buff, size);
			}
			else {
				size_t size = strlen(sid);
				lua_pushlstring(tolua_S, sid, size);
			}
		}
		break;
		}

		return 1;
	}
#ifndef TOLUA_RELEASE
	tolua_lerror :
				 tolua_error(tolua_S, "#ferror in function 'tolua_luaNetwork_byte2int'.", &tolua_err);
				 return 0;
#endif
}

static int tolua_luaNetwork_byte2unsignedInt(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isstring(tolua_S, 1, 0, &tolua_err) ||
		!tolua_isnumber(tolua_S, 2, 0, &tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		char* sid = ((char*)tolua_tostring(tolua_S, 1, 0));
		int len = (int)tolua_tonumber(tolua_S, 2, 0);

		switch (len)
		{
		case 1:
		{
			unsigned char value = *(unsigned char*)sid;
			lua_pushnumber(tolua_S, (lua_Number)value);
			break;
		}
		case 2:
		{
			unsigned short value = *(unsigned short*)sid;
			lua_pushnumber(tolua_S, (lua_Number)value);
			break;
		}
		case 4:
		{
			unsigned int value = *(unsigned int*)sid;
			lua_pushnumber(tolua_S, (lua_Number)value);
		}
		case 8:
		{
			unsigned long long value = *(unsigned long long*)sid;
			lua_pushnumber(tolua_S, (lua_Number)value);
			break;
		}
		default:
		{
			unsigned int value = *(unsigned int*)sid;
			lua_pushnumber(tolua_S, (lua_Number)value);
		}
		break;
		}

		return 1;
	}
#ifndef TOLUA_RELEASE
	tolua_lerror :
				 tolua_error(tolua_S, "#ferror in function 'tolua_luaNetwork_byte2unsignedInt'.", &tolua_err);
				 return 0;
#endif
}

static int tolua_luaNetwork_byte2int64(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isstring(tolua_S, 1, 0, &tolua_err) ||
		!tolua_isnumber(tolua_S, 2, 0, &tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		char* sid = ((char*)tolua_tostring(tolua_S, 1, 0));
		int len = (int)tolua_tonumber(tolua_S, 2, 0);
		long long value = *(long long*)sid;

		lua_pushnumber(tolua_S, (lua_Number)value);

		return 1;
	}

#ifndef TOLUA_RELEASE
	tolua_lerror :
				 tolua_error(tolua_S, "#ferror in function 'tolua_luaNetwork_byte2int64'.", &tolua_err);
				 return 0;
#endif
}

static int tolua_luaNetwork_byte2short(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (!tolua_isstring(tolua_S, 1, 0, &tolua_err))
		goto tolua_lerror;
	else
#endif
	{
		char* sid = ((char*)tolua_tostring(tolua_S, 1, 0));
		short value = *(short*)((void*)sid);

		lua_pushnumber(tolua_S, (lua_Number)value);

		return 1;
	}

#ifndef TOLUA_RELEASE
	tolua_lerror :
				 tolua_error(tolua_S, "#ferror in function 'tolua_luaNetwork_byte2short'.", &tolua_err);
				 return 0;
#endif
}

static int tolua_luaNetwork_byte2long(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (!tolua_isstring(tolua_S, 1, 0, &tolua_err))
		goto tolua_lerror;
	else
#endif
	{
		char* sid = ((char*)tolua_tostring(tolua_S, 1, 0));
		long value = *(long*)((void*)sid);

		lua_pushnumber(tolua_S, (lua_Number)value);

		return 1;
	}

#ifndef TOLUA_RELEASE
	tolua_lerror :
				 tolua_error(tolua_S, "#ferror in function 'tolua_luaNetwork_byte2long'.", &tolua_err);
				 return 0;
#endif
}

static int tolua_luaNetwork_byte2string(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isstring(tolua_S, 1, 0, &tolua_err) ||
		!tolua_isnumber(tolua_S, 2, 0, &tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		char* str = (char*)tolua_tostring(tolua_S, 1, 0);
		int len = (int)tolua_tonumber(tolua_S, 2, 0);

		char buff[512] = { 0 };
		CN2Ttf8(str, buff, 512);
		if (buff) {
			lua_pushstring(tolua_S, buff);
		}
		else {
			lua_pushstring(tolua_S, str);
		}

		return 1;
	}

#ifndef TOLUA_RELEASE
	tolua_lerror :
				 tolua_error(tolua_S, "#ferror in function 'tolua_luaNetwork_byte2string'.", &tolua_err);
				 return 0;
#endif
}


static int tolua_luaNetwork_pushNum(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isnumber(tolua_S, 1, 0, &tolua_err) ||
		!tolua_isstring(tolua_S, 2, 0, &tolua_err) ||
		!tolua_isnumber(tolua_S, 3, 0, &tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		int sid = ((int)tolua_tonumber(tolua_S, 1, 0));
		int size = ((int)tolua_tonumber(tolua_S, 3, 0));
		//int num = ((int)tolua_tonumber(tolua_S, 2, 0));
		//int size = ((int)tolua_tonumber(tolua_S, 3, 0));
		switch (size)
		{
		case 1:
		{
			char value = (char)tolua_tonumber(tolua_S, 2, 0);
			LuaNetMessage::LUAMessager::getInstance()->pushNum(sid, &value, size);
			break;
		}
		case 2:
		{
			short value = (short)tolua_tonumber(tolua_S, 2, 0);
			LuaNetMessage::LUAMessager::getInstance()->pushNum(sid, &value, size);
			break;
		}
		case 4:
		{
			int value = (int)tolua_tonumber(tolua_S, 2, 0);
			LuaNetMessage::LUAMessager::getInstance()->pushNum(sid, &value, size);
			break;
		}
		case 8:
		{
			long long value = (long long)tolua_tonumber(tolua_S, 2, 0);
			LuaNetMessage::LUAMessager::getInstance()->pushNum(sid, &value, size);
			break;
		}
		default:
		{
			break;
		}
		}

		return 0;
	}

#ifndef TOLUA_RELEASE
	tolua_lerror :
				 tolua_error(tolua_S, "#ferror in function 'tolua_luaNetwork_pushNum'.", &tolua_err);
				 return 0;
#endif
}


static int tolua_luaNetwork_pushBoolen(lua_State * tolua_S)
{
	int arrgc = lua_gettop(tolua_S);
	if (arrgc == 3)
	{
		int sid = ((int)tolua_tonumber(tolua_S, 1, 0));
		BYTE cbValue = ((tolua_toboolean(tolua_S, 2,0) == 0) ? FALSE : TRUE);
		int size = ((int)tolua_tonumber(tolua_S, 3, 0));
		LuaNetMessage::LUAMessager::getInstance()->pushNum(sid, &cbValue, size);
	}
	return 0;
}

static int tolua_luaNetWork_pushByte(lua_State *tolua_S)
{
	int arrgc = lua_gettop(tolua_S);
	if (arrgc == 3)
	{
		int sid = ((int)tolua_tonumber(tolua_S, 1, 0));
		BYTE cbValue = (BYTE)lua_tointeger(tolua_S, 2);
		int size = ((int)tolua_tonumber(tolua_S, 3, 0));
		LuaNetMessage::LUAMessager::getInstance()->pushNum(sid, &cbValue, size);
	}
	return 0;
}

static int tolua_luaNetWork_pushWord(lua_State *tolua_S)
{
	int arrgc = lua_gettop(tolua_S);
	if (arrgc == 3)
	{
		int sid = ((int)tolua_tonumber(tolua_S, 1, 0));
		WORD cbValue = (WORD)lua_tointeger(tolua_S, 2);
		int size = ((int)tolua_tonumber(tolua_S, 3, 0));
		LuaNetMessage::LUAMessager::getInstance()->pushNum(sid, &cbValue, size);
	}
	return 0;
}

static int tolua_luaNetWork_pushShort(lua_State *tolua_S)
{
	int arrgc = lua_gettop(tolua_S);
	if (arrgc == 3)
	{
		int sid = ((int)tolua_tonumber(tolua_S, 1, 0));
		SHORT cbValue = (SHORT)lua_tointeger(tolua_S, 2);
		int size = ((int)tolua_tonumber(tolua_S, 3, 0));
		LuaNetMessage::LUAMessager::getInstance()->pushNum(sid, &cbValue, size);
	}
	return 0;
}

static int tolua_luaNetWork_pushInt_2(lua_State *tolua_S)
{
	int arrgc = lua_gettop(tolua_S);
	if (arrgc == 3)
	{
		int sid = ((int)tolua_tonumber(tolua_S, 1, 0));
		int cbValue = (int)lua_tointeger(tolua_S, 2);
		int size = ((int)tolua_tonumber(tolua_S, 3, 0));
		LuaNetMessage::LUAMessager::getInstance()->pushNum(sid, &cbValue, size);
	}
	return 0;
}

static int tolua_luaNetWork_pushDword(lua_State * tolua_S)
{
	int arrgc = lua_gettop(tolua_S);
	if (arrgc == 3)
	{
		int sid = ((int)tolua_tonumber(tolua_S, 1, 0));
		DWORD cbValue = (DWORD)lua_tointeger(tolua_S, 2);
		int size = ((int)tolua_tonumber(tolua_S, 3, 0));
		LuaNetMessage::LUAMessager::getInstance()->pushNum(sid, &cbValue, size);
	}
	return 0;
}

static int tolua_luaNetWork_pushFloat(lua_State *tolua_S)
{
	int arrgc = lua_gettop(tolua_S);
	if (arrgc == 3)
	{
		int sid = ((int)tolua_tonumber(tolua_S, 1, 0));
		float cbValue = (float)lua_tonumber(tolua_S, 2);
		int size = ((int)tolua_tonumber(tolua_S, 3, 0));
		LuaNetMessage::LUAMessager::getInstance()->pushNum(sid, &cbValue, size);
	}
	return 0;
}

static int tolua_luaNetWork_pushDouble(lua_State *tolua_S)
{
	int arrgc = lua_gettop(tolua_S);
	if (arrgc == 3)
	{
		int sid = ((int)tolua_tonumber(tolua_S, 1, 0));
		DOUBLE cbValue = (DOUBLE)lua_tonumber(tolua_S, 2);
		int size = ((int)tolua_tonumber(tolua_S, 3, 0));
		LuaNetMessage::LUAMessager::getInstance()->pushNum(sid, &cbValue, size);
	}
	return 0;
}

static int tolua_luaNetWork_pushScore(lua_State *tolua_S)
{
	int arrgc = lua_gettop(tolua_S);
	if (arrgc == 3)
	{
		int sid = ((int)tolua_tonumber(tolua_S, 1, 0));
		long long num = ((long long)tolua_tonumber(tolua_S, 2, 0));
		int size = ((int)tolua_tonumber(tolua_S, 3, 0));

		if (size > 0) {
			LuaNetMessage::LUAMessager::getInstance()->pushNum(sid, &num, size);
		}
	}
	return 0;
}

static int tolua_luaNetWork_pushString(lua_State *tolua_S)
{
	int arrgc = lua_gettop(tolua_S);
	if (arrgc == 3)
	{
		int sid = ((int)tolua_tonumber(tolua_S, 1, 0));
		size_t wDstLen = ((size_t)tolua_tonumber(tolua_S, 3, 0));
		const char* szData = ((char*)tolua_tostring(tolua_S, 2, 0));
		do
		{
			if (!EMPTY_CHAR(szData))
			{
				WORD wSrcLen = strlen(szData);
				TCHAR *szTmp = new TCHAR[wDstLen + 1];
				memset(szTmp, 0, (wDstLen + 1) * 2);
				char* szSrc = new char[wSrcLen + 1];
				memset(szSrc, 0, wSrcLen + 1);
				memcpy(szSrc, szData, wSrcLen);
				FmUtils::getInstance()->ToServerString(szSrc, wSrcLen, (char*)szTmp, wDstLen);
				LuaNetMessage::LUAMessager::getInstance()->pushLString(sid, (char*)szTmp, wDstLen);
				CC_SAFE_DELETE(szTmp);
				CC_SAFE_DELETE(szSrc);
			}
			else
			{
				LuaNetMessage::LUAMessager::getInstance()->pushLString(sid,nullptr, wDstLen);
			}

			//CCLOG("toLua_Cmd_Data_pushSTRING curLen:%d",cobj->GetCurrentIndex());
		} while (false);
	}
	return 0;
}



int register_all_luaNetWorkEx(lua_State* tolua_S)
{
	tolua_open(tolua_S);

	tolua_module(tolua_S, NULL, 0);
	tolua_beginmodule(tolua_S, NULL);
	tolua_function(tolua_S, "lua_pushStringShortSize", tolua_luaNetwork_pushLStringShortSize);
	tolua_function(tolua_S, "lua_pushLString", tolua_luaNetwork_pushLString);
	tolua_function(tolua_S, "lua_pushLong", tolua_luaNetwork_pushLong);
	tolua_function(tolua_S, "lua_pushint64", tolua_luaNetwork_pushInt64);
	tolua_function(tolua_S, "lua_pushint", tolua_luaNetwork_pushInt);
	tolua_function(tolua_S, "lua_pushnum", tolua_luaNetwork_pushNum);
	tolua_function(tolua_S, "lua_byte2int", tolua_luaNetwork_byte2int);
	tolua_function(tolua_S, "lua_byte2unsignedInt", tolua_luaNetwork_byte2unsignedInt);
	tolua_function(tolua_S, "lua_byte2short", tolua_luaNetwork_byte2short);
	tolua_function(tolua_S, "lua_byte2long", tolua_luaNetwork_byte2long);
	tolua_function(tolua_S, "lua_byte2string", tolua_luaNetwork_byte2string);
	//
	tolua_function(tolua_S, "lua_pushByte", tolua_luaNetWork_pushByte);
	tolua_function(tolua_S, "lua_pushBoolen", tolua_luaNetwork_pushBoolen);
	tolua_function(tolua_S, "lua_pushDword", tolua_luaNetWork_pushDword);
	tolua_function(tolua_S, "lua_pushWord", tolua_luaNetWork_pushWord);
	tolua_function(tolua_S, "lua_pushScore", tolua_luaNetWork_pushScore);
	tolua_function(tolua_S, "lua_pushDouble", tolua_luaNetWork_pushDouble);
	tolua_function(tolua_S, "lua_pushShort", tolua_luaNetWork_pushShort);
	tolua_function(tolua_S, "lua_pushInt", tolua_luaNetWork_pushInt_2);
	tolua_function(tolua_S, "lua_pushTChar", tolua_luaNetWork_pushString);
	tolua_function(tolua_S, "lua_pushFloat", tolua_luaNetWork_pushFloat);
	return 1;
}

