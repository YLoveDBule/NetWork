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

//�궨��
#define XOR_TIMES					8									//���ܱ���
#define MAX_SOURCE_LEN				64									//��󳤶�
#define MAX_ENCRYPT_LEN				(MAX_SOURCE_LEN*XOR_TIMES)			//��󳤶�

//////////////////////////////////////////////////////////////////////////////////

//�������
class CWHEncrypt
{
	//��������
public:
	//���캯��
	CWHEncrypt();
	//��������
	virtual ~CWHEncrypt();

	//���ܺ���
public:
	//��������
	static bool MD5Encrypt(char* pszSourceData, char szMD5Result[LEN_MD5]);

	//���ܺ���
public:
	//��������
	static bool XorEncrypt(const wchar_t* pszSourceData, wchar_t* pszEncrypData, unsigned short wMaxCount);
	//�⿪����
	static bool XorCrevasse(const wchar_t* pszEncrypData, wchar_t* pszSourceData, unsigned short wMaxCount);

	//���ܺ���
public:
	//��������
	static bool MapEncrypt(const wchar_t* pszSourceData, wchar_t* pszEncrypData, unsigned short wMaxCount);
	//�⿪����
	static bool MapCrevasse(const wchar_t* pszEncrypData, wchar_t* pszSourceData, unsigned short wMaxCount);
};

//////////////////////////////////////////////////////////////////////////////////

#endif