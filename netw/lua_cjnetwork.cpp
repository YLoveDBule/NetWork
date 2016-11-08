#include <stdio.h>
#include <stdlib.h>
#include <sstream>
#include <string>

#include "platform/CCPlatformDefine.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include <iconv.h>
#endif

#include "lua_cjnetwork.h"
#include "CJNetworkManager.h"
#include "Messager.h"
#include "base/ccUTF8.h"
#include "WHEncrypt.h"
#include "md5.h"
#include "ConvertUTF/ConvertUTF.h"
#include "Utils/WebUtil.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#include "ProcessModuleManager.h"
#endif

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
	return WebUtility->convertstr("gb2312", "utf-8", sourcebuf, sourcelen, destbuf, destlen);
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
		return WebUtility->convertstr("utf-8", "gb2312", input, strlen(input), output, outlen);
	}

	return -1;
}

static int tolua_cjnetwork_ask2server(lua_State* tolua_S)
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
		int value = ((int)tolua_tonumber(tolua_S, 1, 0));
		char* ip = (char*)tolua_tostring(tolua_S, 2, 0);
		int port = (int)tolua_tonumber(tolua_S, 3, 0);

		YourMessager->ask2Subserver(value, ip, port);
	}
	return 0;
#ifndef TOLUA_RELEASE
	tolua_lerror :
				 tolua_error(tolua_S, "#ferror in function 'cjnetwork_ask2Subserver'.", &tolua_err);
				 return 0;
#endif
}

static int tolua_cjnetwork_closeConnect(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (!tolua_isnumber(tolua_S, 1, 0, &tolua_err))
		goto tolua_lerror;
	else
#endif
	{
		int value = ((int)tolua_tonumber(tolua_S, 1, 0));

		YourMessager->closeConnect(value);
	}
	return 0;
#ifndef TOLUA_RELEASE
	tolua_lerror :
				 tolua_error(tolua_S, "#ferror in function 'tolua_cjnetwork_closeConnect'.", &tolua_err);
				 return 0;
#endif
}

static int tolua_cjnetwork_beginSendMsg(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (!tolua_isnumber(tolua_S, 1, 0, &tolua_err))
		goto tolua_lerror;
	else
#endif
	{
		int value = ((int)tolua_tonumber(tolua_S, 1, 0));

		YourMessager->beginSendMsg(value);
	}
	return 0;
#ifndef TOLUA_RELEASE
	tolua_lerror :
				 tolua_error(tolua_S, "#ferror in function 'cjnetwork_beginSendMsg'.", &tolua_err);
				 return 0;
#endif
}

static int tolua_cjnetwork_pushNum(lua_State* tolua_S)
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

		YourMessager->pushNum(sid, &num, (NumbericSize)size);
	}
	return 0;
#ifndef TOLUA_RELEASE
	tolua_lerror :
				 tolua_error(tolua_S, "#ferror in function 'cjnetwork_pushNum'.", &tolua_err);
				 return 0;
#endif
}

static int tolua_cjnetwork_pushInt(lua_State* tolua_S)
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

		YourMessager->pushNum(sid, &num, (NumbericSize)size);
	}
	return 0;
#ifndef TOLUA_RELEASE
	tolua_lerror :
				 tolua_error(tolua_S, "#ferror in function 'cjnetwork_pushInt'.", &tolua_err);
				 return 0;
#endif
}

static int tolua_cjnetwork_pushInt64(lua_State* tolua_S)
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
		
		YourMessager->pushNum(sid, &num, (NumbericSize)size);
	}
	return 0;
#ifndef TOLUA_RELEASE
	tolua_lerror :
				 tolua_error(tolua_S, "#ferror in function 'cjnetwork_pushInt64'.", &tolua_err);
				 return 0;
#endif
}
static int tolua_cjnetwork_pushShort(lua_State* tolua_S)
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
		
		YourMessager->pushNum(sid, (void*)&num, (NumbericSize)size);
	}
	return 0;
#ifndef TOLUA_RELEASE
	tolua_lerror :
				 tolua_error(tolua_S, "#ferror in function 'cjnetwork_pushNum'.", &tolua_err);
				 return 0;
#endif
}

static int tolua_cjnetwork_pushLong(lua_State* tolua_S)
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
		
		YourMessager->pushNum(sid, (void*)&num, (NumbericSize)size);
	}
	return 0;
#ifndef TOLUA_RELEASE
	tolua_lerror :
				 tolua_error(tolua_S, "#ferror in function 'cjnetwork_pushNum'.", &tolua_err);
				 return 0;
#endif
}

static int tolua_cjnetwork_pushLString(lua_State* tolua_S)
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
		char* buff = ((char*)tolua_tostring(tolua_S, 2, 0));
		buff[strlen(buff) + 1] = '\0';

		YourMessager->pushLString(sid, buff, size);
	}
	return 0;
#ifndef TOLUA_RELEASE
	tolua_lerror :
				 tolua_error(tolua_S, "#ferror in function 'cjnetwork_pushLString'.", &tolua_err);
				 return 0;
#endif
}

static int tolua_cjnetwork_pushLStringShortSize(lua_State* tolua_S)
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

		YourMessager->pushLStringShortSize(sid, buff, size);
	}
	return 0;
#ifndef TOLUA_RELEASE
	tolua_lerror :
				 tolua_error(tolua_S, "#ferror in function 'cjnetwork_pushLStringShortSize'.", &tolua_err);
				 return 0;
#endif
}

static int tolua_cjnetwork_endSendMsg(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (!tolua_isnumber(tolua_S, 1, 0, &tolua_err))
		goto tolua_lerror;
	else
#endif
	{
		int sid = ((int)tolua_tonumber(tolua_S, 1, 0));

		YourMessager->endSendMsg(sid);
	}
	return 0;
#ifndef TOLUA_RELEASE
	tolua_lerror :
				 tolua_error(tolua_S, "#ferror in function 'cjnetwork_endSendMsg'.", &tolua_err);
				 return 0;
#endif
}

static int tolua_cjnetwork_sendMessage(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isnumber(tolua_S, 1, 0, &tolua_err) ||
		!tolua_isnumber(tolua_S, 2, 0, &tolua_err) ||
		!tolua_isnumber(tolua_S, 3, 0, &tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		int sid = ((int)tolua_tonumber(tolua_S, 1, 0));
		int mcmd = ((int)tolua_tonumber(tolua_S, 2, 0));
		int smcd = ((int)tolua_tonumber(tolua_S, 3, 0));

		YourMessager->sendMessage(sid, mcmd, smcd);
	}
	return 0;
#ifndef TOLUA_RELEASE
	tolua_lerror :
				 tolua_error(tolua_S, "#ferror in function 'cjnetwork_sendMessage'.", &tolua_err);
				 return 0;
#endif
}

static int tolua_cjnetwork_gettemptestdata(lua_State* tolua_S)
{
	typedef struct FSP {
		int x;
		int y;
	} FSP;

	typedef struct FCfg {
		int res;
		int speed;
		int npcount;
		FSP* point;
	} FCfg;

	typedef struct Topp {
		int fishcount;
		FCfg* fishes;
	} Topp;

	Topp *topp = new Topp();
	topp->fishcount = rand() % 10 + 5;

	if (topp->fishcount > 0)
	{
		topp->fishes = new FCfg[topp->fishcount];
		for (int j = 0; j < topp->fishcount; j++)
		{
			topp->fishes[j].res = rand() % 50;
			topp->fishes[j].speed = rand() % 10;
			topp->fishes[j].npcount = rand() % 30 + 10;
			if (topp->fishes[j].npcount > 0)
			{
				topp->fishes[j].point = new FSP[topp->fishes[j].npcount];
				for (int i = 0; i < topp->fishes[j].npcount; i++)
				{
					topp->fishes[j].point[i].x = rand() % 1000 - 500;
					topp->fishes[j].point[i].y = rand() % 600 - 300;
				}
			}
		}
	}

	const int buffsize = 128 * 1024;
	char buff[buffsize] = { 0 };
	int lenint = sizeof(int);
	int offset = 0;

	memcpy(buff + offset, (void*)&(topp->fishcount), lenint); offset += lenint;
	for (int i = 0; i < topp->fishcount; i++)
	{
		memcpy(buff + offset, (void*)&(topp->fishes[i].res), lenint); offset += lenint;

		memcpy(buff + offset, (void*)&(topp->fishes[i].speed), lenint); offset += lenint;

		memcpy(buff + offset, (void*)&(topp->fishes[i].npcount), lenint); offset += lenint;

		for (int j = 0; j < topp->fishes[i].npcount; j++)
		{
			memcpy(buff + offset, (void*)&topp->fishes[i].point[j].x, lenint); offset += lenint;
			memcpy(buff + offset, (void*)&topp->fishes[i].point[j].y, lenint); offset += lenint;
		}
	}

	lua_pushlstring(tolua_S, (char*)buff, offset);

	return 1;
}

static int tolua_cjnetwork_byte2point(lua_State* tolua_S)
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

		typedef struct npoint {
			int x;
			int y;
		} npoint;

		if (len == 8) {
			npoint* np = (npoint*)sid;
			int i = 0;
		}
	}
	return 1;
#ifndef TOLUA_RELEASE
	tolua_lerror :
				 tolua_error(tolua_S, "#ferror in function 'cjnetwork_byte2point'.", &tolua_err);
				 return 0;
#endif
}

static int tolua_cjnetwork_byte2int(lua_State* tolua_S)
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
			if (CN2Ttf8(sid, buff, 512) != -1) {
			//if (buff && strlen(buff) > 0) {
				int size = strlen(buff);
				lua_pushstring(tolua_S, buff);
			}
			else {
				lua_pushstring(tolua_S, sid);
			}
		}
		break;
		}

		return 1;
	}
#ifndef TOLUA_RELEASE
	tolua_lerror :
				 tolua_error(tolua_S, "#ferror in function 'cjnetwork_byte2int'.", &tolua_err);
				 return 0;
#endif
}

static int tolua_cjnetwork_byte2unsignedInt(lua_State* tolua_S)
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
				 tolua_error(tolua_S, "#ferror in function 'cjnetwork_byte2unsignedInt'.", &tolua_err);
				 return 0;
#endif
}

static int tolua_cjnetwork_byte2int64(lua_State* tolua_S)
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
	}
	return 1;
#ifndef TOLUA_RELEASE
	tolua_lerror :
				 tolua_error(tolua_S, "#ferror in function 'cjnetwork_byte2int64'.", &tolua_err);
				 return 0;
#endif
}

static int tolua_cjnetwork_byte2short(lua_State* tolua_S)
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
	}
	return 1;
#ifndef TOLUA_RELEASE
	tolua_lerror :
				 tolua_error(tolua_S, "#ferror in function 'cjnetwork_byte2short'.", &tolua_err);
				 return 0;
#endif
}

static int tolua_cjnetwork_byte2long(lua_State* tolua_S)
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
	}
	return 1;
#ifndef TOLUA_RELEASE
	tolua_lerror :
				 tolua_error(tolua_S, "#ferror in function 'cjnetwork_byte2long'.", &tolua_err);
				 return 0;
#endif
}

static int tolua_cjnetwork_byte2string(lua_State* tolua_S)
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
	}
	return 1;
#ifndef TOLUA_RELEASE
	tolua_lerror :
				 tolua_error(tolua_S, "#ferror in function 'cjnetwork_byte2string'.", &tolua_err);
				 return 0;
#endif
}

static int tolua_cjnetwork_cj_md5Encrypt(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (!tolua_isstring(tolua_S, 1, 0, &tolua_err))
		goto tolua_lerror;
	else
#endif
	{
		char* str = (char*)tolua_tostring(tolua_S, 1, 0);
		char szPassword[LEN_MD5];
		memset(szPassword, 0, LEN_MD5);
		CWHEncrypt::MD5Encrypt(str, szPassword);

		std::string stdstr = MD5(std::string(str)).toStr();

		lua_pushstring(tolua_S, stdstr.c_str());
	}
	return 1;
#ifndef TOLUA_RELEASE
	tolua_lerror :
				 tolua_error(tolua_S, "#ferror in function 'cjnetwork_cj_md5Encrypt'.", &tolua_err);
				 return 0;
#endif
}

static int tolua_cjnetwork_cj_getUsrGUID(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (false)
		goto tolua_lerror;
	else
#endif
	{
		const char* str = WebUtility->getUSRGUID();

		lua_pushstring(tolua_S, str);
	}
	return 1;
#ifndef TOLUA_RELEASE
	tolua_lerror :
				 tolua_error(tolua_S, "#ferror in function 'cjnetwork_cj_getUsrGUID'.", &tolua_err);
				 return 0;
#endif
}

static int tolua_cjnetwork_cj_getnetstr(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (!tolua_isstring(tolua_S, 1, 0, &tolua_err))
		goto tolua_lerror;
	else
#endif
	{
		char* str = (char*)tolua_tostring(tolua_S, 1, 0);
		char buff[512] = { 0 };
		if (utf82gb(str, buff, 512) != -1) {
		//if (buff && strlen(buff) > 0) {
			lua_pushstring(tolua_S, buff);
		}
		else {
			lua_pushstring(tolua_S, str);
		}
	}
	return 1;
#ifndef TOLUA_RELEASE
	tolua_lerror :
				 tolua_error(tolua_S, "#ferror in function 'cjnetwork_cj_getnetstr'.", &tolua_err);
				 return 0;
#endif
}

static int tolua_cj_testcreateproc(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isnumber(tolua_S, 1, 0, &tolua_err) ||
		!tolua_isstring(tolua_S, 2, 0, &tolua_err) ||
		!tolua_isstring(tolua_S, 3, 0, &tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		SOCKET sid = (SOCKET)tolua_tonumber(tolua_S, 1, 0);
		std::string scriptname((char*)tolua_tostring(tolua_S, 2, 0));
		std::string exename((char*)tolua_tostring(tolua_S, 3, 0));

		if (scriptname.find(".lua") == std::string::npos) {
			scriptname.append(".lua");
		}

		std::stringstream ss;
		ss << exename;
		ss << " ";
		ss << scriptname;
		ss << " ";
		ss << sid;

		ProcessModuleMgr->startNewProcess(sid, ss.str().c_str());
	}
	return 0;
#ifndef TOLUA_RELEASE
	tolua_lerror :
				 tolua_error(tolua_S, "#ferror in function 'cjnetwork_cj_getnetstr'.", &tolua_err);
				 return 0;
#endif
}

static int tolua_cj_creategameproc(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (
		!tolua_isnumber(tolua_S, 1, 0, &tolua_err) ||
		!tolua_isstring(tolua_S, 2, 0, &tolua_err) ||
		!tolua_isstring(tolua_S, 3, 0, &tolua_err)
		)
		goto tolua_lerror;
	else
#endif
	{
		SOCKET sid = (SOCKET)tolua_tonumber(tolua_S, 1, 0);
		std::string scriptname((char*)tolua_tostring(tolua_S, 2, 0));
		std::string exename((char*)tolua_tostring(tolua_S, 3, 0));

		if (scriptname.find(".lua") == std::string::npos) {
			scriptname.append(".lua");
		}

		std::stringstream ss;
		ss << exename;
		ss << " ";
		ss << scriptname;
		ss << " ";
		ss << sid;

		ProcessModuleMgr->startNewProcess(sid, ss.str().c_str());
	}
	return 0;
#ifndef TOLUA_RELEASE
	tolua_lerror :
				 tolua_error(tolua_S, "#ferror in function 'tolua_cj_creategameproc'.", &tolua_err);
				 return 0;
#endif
}

static int tolua_cj_is_windows(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (false)
		goto tolua_lerror;
	else                                                          
#endif
	{
#if CUR_DEVICE_TYPE == DEVICE_TYPE_PC
		lua_pushnumber(tolua_S, (lua_Number)0x01);
#else
		lua_pushnumber(tolua_S, (lua_Number)0x0);
#endif
	}
	return 1;
#ifndef TOLUA_RELEASE
	tolua_lerror :
				 tolua_error(tolua_S, "#ferror in function 'cjnetwork_cj_is_windows'.", &tolua_err);
				 return 0;
#endif
}

static int tolua_cj_isSubProcess(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (false)
		goto tolua_lerror;
	else
#endif
	{
#if CUR_DEVICE_TYPE == DEVICE_TYPE_PC
		bool isSub = ProcessModuleMgr->isSubProcess();
		int result = isSub ? 0x01 : 0x0;
		lua_pushboolean(tolua_S, (lua_Number)result);
#else
		lua_pushboolean(tolua_S, (lua_Number)0x0);
#endif
	}
	return 1;
#ifndef TOLUA_RELEASE
	tolua_lerror :
				 tolua_error(tolua_S, "#ferror in function 'cj_isSubProcess'.", &tolua_err);
				 return 0;
#endif
}

static int tolua_cj_getGameScript(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (false)
		goto tolua_lerror;
	else
#endif
	{
#if CUR_DEVICE_TYPE == DEVICE_TYPE_PC
		std::string gamescriptname;
		ProcessModuleMgr->getGameScriptName(gamescriptname);
		lua_pushstring(tolua_S, gamescriptname.c_str());
#else
		lua_pushstring(tolua_S, "");
#endif
	}
	return 1;
#ifndef TOLUA_RELEASE
	tolua_lerror :
				 tolua_error(tolua_S, "#ferror in function 'cj_getGameScript'.", &tolua_err);
				 return 0;
#endif
}

static int tolua_cj_getSelfSid(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (false)
		goto tolua_lerror;
	else
#endif
	{
#if CUR_DEVICE_TYPE == DEVICE_TYPE_PC
		SOCKET sid = ProcessModuleMgr->getSelfSid();
		lua_pushnumber(tolua_S, sid);
#else
		lua_pushnumber(tolua_S, INVALID_SOCKET);
#endif
	}
	return 1;
#ifndef TOLUA_RELEASE
	tolua_lerror :
				 tolua_error(tolua_S, "#ferror in function 'cj_getSelfSid'.", &tolua_err);
				 return 0;
#endif
}

static int tolua_cj_testpromsg(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (false)
		goto tolua_lerror;
	else
#endif
	{
#if CUR_DEVICE_TYPE == DEVICE_TYPE_PC
		long long testsh = (long long)ProcessModuleMgr->getSelfHwnd();
		ProcessModuleMgr->sendData(ProcessModuleMgr->getDefaultDesHwnd(), 0, 0,
			(void*)&testsh, sizeof(long long), IPC_DATA_IDENTIFY);
#else
		
#endif
	}
	return 0;
#ifndef TOLUA_RELEASE
	tolua_lerror :
				 tolua_error(tolua_S, "#ferror in function 'cj_testpromsg'.", &tolua_err);
				 return 0;
#endif
}

static int tolua_cj_subready(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (false)
		goto tolua_lerror;
	else
#endif
	{
#if CUR_DEVICE_TYPE == DEVICE_TYPE_PC
		long long testsh = (long long)ProcessModuleMgr->getSelfHwnd();
		ProcessModuleMgr->sendData(ProcessModuleMgr->getDefaultDesHwnd(), 0, 0,
			NULL, 0, IPC_DATA_SUBREADY);
#else

#endif
	}
	return 0;
#ifndef TOLUA_RELEASE
	tolua_lerror :
				 tolua_error(tolua_S, "#ferror in function 'cj_subready'.", &tolua_err);
				 return 0;
#endif
}

static int tolua_cj_registerSelfUsrID(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (!tolua_isnumber(tolua_S, 1, 0, &tolua_err))
		goto tolua_lerror;
	else
#endif
	{
		size_t value = ((size_t)tolua_tonumber(tolua_S, 1, 0));

		ProcessModuleMgr->registerSubUsrID(value);
	}
	return 0;
#ifndef TOLUA_RELEASE
	tolua_lerror :
				 tolua_error(tolua_S, "#ferror in function 'tolua_cjnetwork_closeConnect'.", &tolua_err);
				 return 0;
#endif
}

static int tolua_cj_addFreeCMDForSub(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (!tolua_isnumber(tolua_S, 1, 0, &tolua_err))
		goto tolua_lerror;
	else
#endif
	{
		int value = ((int)tolua_tonumber(tolua_S, 1, 0));

		YourMessager->addFreeMainCMDForSub(value);
	}
	return 0;
#ifndef TOLUA_RELEASE
	tolua_lerror :
				 tolua_error(tolua_S, "#ferror in function 'tolua_cj_addFreeCMDForSub'.", &tolua_err);
				 return 0;
#endif
}

static int tolua_cj_minimizeWindow(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (false)
		goto tolua_lerror;
	else
#endif
	{
		YourMessager->minimizeWindow();
	}
	return 0;
#ifndef TOLUA_RELEASE
	tolua_lerror :
				 tolua_error(tolua_S, "#ferror in function 'tolua_cj_minimizeWindow'.", &tolua_err);
				 return 0;
#endif
}

static int tolua_cj_openwebpage(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (!tolua_isstring(tolua_S, 1, 0, &tolua_err))
		goto tolua_lerror;
	else
#endif
	{
		char* value = (char*)tolua_tostring(tolua_S, 1, 0);

		if (value) {
			WebUtility->openwebpage(value);
		}
	}
	return 0;
#ifndef TOLUA_RELEASE
	tolua_lerror :
				 tolua_error(tolua_S, "#ferror in function 'tolua_cj_openwebpage'.", &tolua_err);
				 return 0;
#endif
}

static int tolua_cj_suicide(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
	tolua_Error tolua_err;
	if (false)
		goto tolua_lerror;
	else
#endif
	{
		YourMessager->suicide();
	}
	return 0;
#ifndef TOLUA_RELEASE
	tolua_lerror :
				 tolua_error(tolua_S, "#ferror in function 'tolua_cj_suicide'.", &tolua_err);
				 return 0;
#endif
}
//////////////////////////////////////////////////////////////////////////

int register_all_cjnetwork(lua_State* tolua_S)
{
	tolua_open(tolua_S);

	tolua_module(tolua_S, NULL, 0);
	tolua_beginmodule(tolua_S, NULL);

	tolua_function(tolua_S, "cj_ask2serverip", tolua_cjnetwork_ask2server);
	tolua_function(tolua_S, "cj_closeConnect", tolua_cjnetwork_closeConnect);
	tolua_function(tolua_S, "cj_beginSendMsg", tolua_cjnetwork_beginSendMsg);
	tolua_function(tolua_S, "cj_pushNum", tolua_cjnetwork_pushNum);

	tolua_function(tolua_S, "cj_pushInt", tolua_cjnetwork_pushInt);
	tolua_function(tolua_S, "cj_pushInt64", tolua_cjnetwork_pushInt64);
	tolua_function(tolua_S, "cj_pushShort", tolua_cjnetwork_pushShort);
	tolua_function(tolua_S, "cj_pushLong", tolua_cjnetwork_pushLong);

	tolua_function(tolua_S, "cj_pushLString", tolua_cjnetwork_pushLString);
	tolua_function(tolua_S, "cj_pushLStringShortSize", tolua_cjnetwork_pushLStringShortSize);
	tolua_function(tolua_S, "cj_endSendMsg", tolua_cjnetwork_endSendMsg);
	tolua_function(tolua_S, "cj_sendMessage", tolua_cjnetwork_sendMessage);

	tolua_function(tolua_S, "cj_gettemptestdata", tolua_cjnetwork_gettemptestdata); // for test
	tolua_function(tolua_S, "cj_byte2point", tolua_cjnetwork_byte2point); // for test
	tolua_function(tolua_S, "cj_byte2int", tolua_cjnetwork_byte2int);
	tolua_function(tolua_S, "cj_byte2unsignedInt", tolua_cjnetwork_byte2unsignedInt);
	//tolua_function(tolua_S, "cj_byte2int64", tolua_cjnetwork_byte2int64);
	tolua_function(tolua_S, "cj_byte2short", tolua_cjnetwork_byte2short);
	tolua_function(tolua_S, "cj_byte2long", tolua_cjnetwork_byte2long);
	//tolua_function(tolua_S, "cj_byte2string", tolua_cjnetwork_byte2string);

	tolua_function(tolua_S, "cj_md5Encrypt", tolua_cjnetwork_cj_md5Encrypt);
	tolua_function(tolua_S, "cj_getUsrGUID", tolua_cjnetwork_cj_getUsrGUID);
	tolua_function(tolua_S, "cj_getnetstr", tolua_cjnetwork_cj_getnetstr);
	tolua_function(tolua_S, "cj_testcreateproc", tolua_cj_testcreateproc);
	tolua_function(tolua_S, "cj_creategameproc", tolua_cj_creategameproc);

	////// process functions tolua
	tolua_function(tolua_S, "cj_is_windows", tolua_cj_is_windows);
	tolua_function(tolua_S, "cj_isSubProcess", tolua_cj_isSubProcess);
	tolua_function(tolua_S, "cj_getGameScript", tolua_cj_getGameScript);
	tolua_function(tolua_S, "cj_getSelfSid", tolua_cj_getSelfSid);

	tolua_function(tolua_S, "cj_testpromsg", tolua_cj_testpromsg);
	tolua_function(tolua_S, "cj_subready", tolua_cj_subready);
	tolua_function(tolua_S, "cj_registerSelfUsrID", tolua_cj_registerSelfUsrID);
	tolua_function(tolua_S, "cj_addFreeCMDForSub", tolua_cj_addFreeCMDForSub);

	tolua_function(tolua_S, "cj_minimizeWindow", tolua_cj_minimizeWindow);
	tolua_function(tolua_S, "cj_openwebpage", tolua_cj_openwebpage);
	tolua_function(tolua_S, "cj_suicide", tolua_cj_suicide);

	//tolua_function(tolua_S, "cj_testcreateproc", tolua_cj_testcreateproc);

	tolua_endmodule(tolua_S);

	return 1;
}
