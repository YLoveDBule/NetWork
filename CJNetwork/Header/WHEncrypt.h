#ifndef WH_ENCRYPT_HEAD_FILE
#define WH_ENCRYPT_HEAD_FILE

#pragma once

#include "Define.h"
#include "CJPlatform.h"

#if CJ_TARGET_PLATFORM == CJ_PLATFORM_WIN32
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
class __declspec(dllexport) CWHEncrypt
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
	static bool MD5Encrypt(const char* pszSourceData, char szMD5Result[LEN_MD5]);

	//���ܺ���
public:
	//��������
	//static bool XorEncrypt(const char* pszSourceData, char* pszEncrypData, WORD wMaxCount);
	//�⿪����
	//static bool XorCrevasse(const char* pszEncrypData, char* pszSourceData, WORD wMaxCount);

	//���ܺ���
public:
	//��������
	static bool MapEncrypt(const char* pszSourceData, char* pszEncrypData, unsigned short wMaxCount);
	//�⿪����
	static bool MapCrevasse(const char* pszEncrypData, char* pszSourceData, unsigned short wMaxCount);
};

//////////////////////////////////////////////////////////////////////////////////

#endif