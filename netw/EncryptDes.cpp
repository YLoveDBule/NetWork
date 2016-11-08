//#include "Stdafx.h"
#define _EPESDK_IMP_
//#include <stdio.h>
//#include <stdlib.h>
#include <string.h>

#include "EncryptDes.h"
#include <assert.h>

CEncryptDes::CEncryptDes()
{
	memset(&key_des,0,sizeof(key_des));
	memset(&key_rc5,0,sizeof(key_rc5));
	haveKey_des = false;
	haveKey_rc5 = false;
	method = ENCDEC_NONE;
}

void CEncryptDes::ZES_set_key(const_ZES_cblock *key,ZES_key_schedule *schedule)
{
	des.ZES_set_key(key,schedule);
}

void CEncryptDes::ZES_random_key(ZES_cblock *ret)
{
	des.ZES_random_key(ret);
}

void CEncryptDes::ZES_encrypt1(ZES_LONG *data,ZES_key_schedule *ks,int enc)
{
	des.ZES_encrypt1(data,ks,enc);
}

void CEncryptDes::RC5_32_set_key(RC5_32_KEY *key,int len,const unsigned char *data,int rounds)
{
	des.RC5_32_set_key(key,len,data,rounds);
}

void CEncryptDes::RC5_32_encrypt(unsigned long*d,RC5_32_KEY *key)
{
	des.RC5_32_encrypt(d,key);
}

void CEncryptDes::RC5_32_decrypt(unsigned long*d,RC5_32_KEY *key)
{
	des.RC5_32_decrypt(d,key);
}

int CEncryptDes::encdec_des(unsigned char *data,unsigned long nLen,bool enc)
{
	if ((0==data)||(!haveKey_des)) return -1;

	unsigned long offset = 0;
	while (offset<=nLen-8)
	{       
		des.ZES_encrypt1((ZES_LONG*)(data+offset),&key_des,enc);
		offset += 8;
	}


	return nLen-offset;
}

inline void mymemcpy(void* pDst, unsigned long dwDstSize, void* pScr, unsigned long dwCpSize )
{
	if(dwCpSize>dwDstSize)
	{
		//MessageBox(NULL,"内存操作将越界","错误",MB_ICONERROR);
		assert(false);
	}
	//memcpy_s(pDst,dwDstSize,pScr,dwCpSize);
	memcpy(pDst, pScr, dwCpSize);
}

int CEncryptDes::encdec_rc5(unsigned char *data,unsigned long nLen,bool enc)
{
	if ((0==data)||(!haveKey_rc5)) return -1;

	unsigned long offset = 0;
	while (offset<=nLen-8)
	{
		unsigned long d[2];
		//_MEMCPY_(d,data+offset,sizeof(d),sizeof(d));
		mymemcpy((void*)d,sizeof(d),data+offset,sizeof(d));
		if (enc)
			RC5_32_encrypt(d,&key_rc5);
		else
			RC5_32_decrypt(d,&key_rc5);
		//_MEMCPY_(data+offset,d,sizeof(d),sizeof(d));
		mymemcpy((void*)(data+offset),sizeof(d),d,sizeof(d));
		offset += sizeof(d);
	}

	return nLen-offset;
}

void CEncryptDes::random_key_des(ZES_cblock *ret)
{
	des.ZES_random_key(ret);
}

void CEncryptDes::set_key_des(const_ZES_cblock *key)
{
	ZES_set_key(key,&key_des);
	haveKey_des = true;
}

void CEncryptDes::set_key_rc5(const unsigned char *data,int nLen,int rounds)
{
	RC5_32_set_key(&key_rc5,nLen,data,rounds);
	haveKey_rc5 = true;
} 

int CEncryptDes::encdec(void *data,unsigned long nLen,bool enc)
{
	switch(method)
	{
	case ENCDEC_NONE:
		return -1;
	case ENCDEC_DES:
		return encdec_des((unsigned char*)data,nLen,enc);
	case ENCDEC_RC5:
		return encdec_rc5((unsigned char*)data,nLen,enc);
	}
	return -2;
}

void CEncryptDes::setEncMethod(encMethod m)
{
	method = m;
}

CEncryptDes::encMethod CEncryptDes::getEncMethod() const
{
	return method;
}
