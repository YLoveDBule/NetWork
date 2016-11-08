#ifndef __C__ENCRYPT__H__
#define __C__ENCRYPT__H__

#include "MiniCrypt.h"

class  CEncryptDes
{
public:
	CEncryptDes();
	enum encMethod
	{
		ENCDEC_NONE,
		ENCDEC_DES,
		ENCDEC_RC5
	};
	void random_key_des(ZES_cblock *ret);
	void set_key_des(const_ZES_cblock *key);
	void set_key_rc5(const unsigned char *data,int nLen,int rounds);
	//enc:1为加密,0:为解密
	int encdec(void *data,unsigned long nLen,bool enc);

	void setEncMethod(encMethod method);
	encMethod getEncMethod() const;

private:
	void ZES_random_key(ZES_cblock *ret);
	void ZES_set_key(const_ZES_cblock *key,ZES_key_schedule *schedule);
	void ZES_encrypt1(ZES_LONG *data,ZES_key_schedule *ks,int enc);

	void RC5_32_set_key(RC5_32_KEY *key,int len,const unsigned char *data,int rounds);
	void RC5_32_encrypt(RC5_32_INT *d,RC5_32_KEY *key);
	void RC5_32_decrypt(RC5_32_INT *d,RC5_32_KEY *key);

	int encdec_des(unsigned char *data,unsigned long nLen,bool enc);
	int encdec_rc5(unsigned char *data,unsigned long nLen,bool enc);

	ZES_key_schedule key_des;
	RC5_32_KEY key_rc5;
	bool haveKey_des;
	bool haveKey_rc5;

	encMethod method;

	CMiniCryptDes des;
};

#endif // !__C__ENCRYPT__H__
