#pragma  once
#ifndef __FMUTILS__
#define __FMUTILS__

#include "cocos2d.h"
class FmUtils
{
public:
	FmUtils();
	virtual ~FmUtils();
	static FmUtils* s_FmUtils;
	static FmUtils* getInstance();
	int convertstr(const char *src_charset,const char *des_charset,char *srcbuff,size_t srclen,char *desbuff,size_t deslen);
	int code_convert(const char *from_charset, const char *to_charset, char *inbuf,
		size_t inlen, char *outbuf, size_t outlen);

	/* UTF-8 to wchar  */
	int u2w(char *inbuf, size_t inlen, char *outbuf, size_t outlen);

	/* UTF-8 to gbk  */
	int u2g(char *inbuf, size_t inlen, char *outbuf, size_t outlen);

	/* wchar to UTF-8 */
	int w2u(char *inbuf, size_t inlen, char *outbuf, size_t outlen);

	/* wchar to gbk */
	int w2g(char *inbuf, size_t inlen, char *outbuf, size_t outlen);

	/* gbk to UTF-8 */
	int g2u(char *inbuf, size_t inlen, char *outbuf, size_t outlen);

	/* gbk to wchar */
	int g2w(char *inbuf, size_t inlen, char *outbuf, size_t outlen);
	bool ToServerString(char *inbuf, size_t inlen, char *outbuf, size_t outlen);
	bool ToClientString(char *inbuf, size_t inlen, char *outbuf, size_t outlen);

};

#endif