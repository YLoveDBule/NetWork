#ifndef WH_ENCRYPT_HEAD_FILE
#define WH_ENCRYPT_HEAD_FILE

#pragma once

//#include "ServiceCoreHead.h"
#include "Define.h"
#include "platform/CCPlatformDefine.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#include <windows.h>
#else

#endif

//////////////////////////////////////////////////////////////////////////////////

//宏定义
#define XOR_TIMES					8									//加密倍数
#define MAX_SOURCE_LEN				64									//最大长度
#define MAX_ENCRYPT_LEN				(MAX_SOURCE_LEN*XOR_TIMES)			//最大长度

//////////////////////////////////////////////////////////////////////////////////

//加密组件
class CWHEncrypt
{
	//函数定义
public:
	//构造函数
	CWHEncrypt();
	//析构函数
	virtual ~CWHEncrypt();

	//加密函数
public:
	//生成密文
	static bool MD5Encrypt(char* pszSourceData, char szMD5Result[LEN_MD5]);

	//加密函数
public:
	//生成密文
	static bool XorEncrypt(const wchar_t* pszSourceData, wchar_t* pszEncrypData, unsigned short wMaxCount);
	//解开密文
	static bool XorCrevasse(const wchar_t* pszEncrypData, wchar_t* pszSourceData, unsigned short wMaxCount);

	//加密函数
public:
	//生成密文
	static bool MapEncrypt(const wchar_t* pszSourceData, wchar_t* pszEncrypData, unsigned short wMaxCount);
	//解开密文
	static bool MapCrevasse(const wchar_t* pszEncrypData, wchar_t* pszSourceData, unsigned short wMaxCount);
};

//////////////////////////////////////////////////////////////////////////////////

#endif