#include "FmUtils.h"
#include <iconv.h>
#include "platform/CCPlatformDefine.h"

FmUtils * FmUtils::s_FmUtils = nullptr;

int FmUtils::convertstr(const char *src_charset, const char *des_charset, char *srcbuff, size_t srclen, char *desbuff, size_t deslen)
{
	iconv_t cd;

	if ((cd = iconv_open(des_charset, src_charset)) == (iconv_t)(-1)) {
		return -1;
	}

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || \
	CC_TARGET_PLATFORM == CC_PLATFORM_WINRT 
	int rc = iconv(cd, &srcbuff, &srclen, &desbuff, &deslen);
#else
	int rc = iconv(cd, &srcbuff, &srclen, &desbuff, &deslen);
#endif
	if (rc == -1) {

	}

	iconv_close(cd);

	return 1;
}

int FmUtils::code_convert(const char *from_charset, const char *to_charset, char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
	iconv_t cd;
	char **pin = &inbuf;
	char **pout = &outbuf;

	cd = iconv_open(to_charset, from_charset);
	if (cd == 0)
		return -1;
	memset(outbuf, 0, outlen);
	if (iconv(cd, pin, &inlen, pout, &outlen) == -1)
		return -1;
	iconv_close(cd);
	return 0;
}

int FmUtils::u2w(char * inbuf, size_t inlen, char * outbuf, size_t outlen)
{
	return code_convert("UTF-8", "UTF-16LE", inbuf, inlen, outbuf, outlen);
}

int FmUtils::u2g(char * inbuf, size_t inlen, char * outbuf, size_t outlen)
{
	return code_convert("UTF-8", "GBK", inbuf, inlen, outbuf, outlen);
}

int FmUtils::w2u(char * inbuf, size_t inlen, char * outbuf, size_t outlen)
{
	return code_convert("UTF-16LE", "UTF-8", inbuf, inlen, outbuf, outlen);
}

int FmUtils::w2g(char * inbuf, size_t inlen, char * outbuf, size_t outlen)
{
	return code_convert("UTF-16LE", "GBK", inbuf, inlen, outbuf, outlen);
}

int FmUtils::g2u(char * inbuf, size_t inlen, char * outbuf, size_t outlen)
{
	return code_convert("GBK", "UTF-8", inbuf, inlen, outbuf, outlen);
}

int FmUtils::g2w(char * inbuf, size_t inlen, char * outbuf, size_t outlen)
{
	return code_convert("GBK", "UTF-16LE", inbuf, inlen, outbuf, outlen);
}

bool FmUtils::ToServerString(char * inbuf, size_t inlen, char * outbuf, size_t outlen)
{

	return u2w(inbuf, inlen, outbuf, outlen) == 0;
}

bool FmUtils::ToClientString(char *inbuf, size_t inlen, char *outbuf, size_t outlen)
{
	return w2u(inbuf, inlen, outbuf, outlen) == 0;
}

//数组:utf8 -> unicode
unsigned short FmUtils::YYConverUtf8ToUnicodeWithArray(TCHAR destUnicode[], int tcharLength, const char srcUtf8[])
{
	if (srcUtf8 == nullptr)
	{
		return 0;
	}
	memset(destUnicode, 0, sizeof(TCHAR)*tcharLength);

	unsigned short wLen = strlen(srcUtf8);
	unsigned short wIndex = 0;
	unsigned short wCount = 0;
	for (unsigned short i = 0 ; i < wLen; ++i)
	{
		if (*(srcUtf8 + wIndex) != '\0')
		{
			// 得到utf8的字节数
			int word_byte = YYConverUtf8ToUnicode(destUnicode + i, srcUtf8 + wIndex);
			wIndex += word_byte;

			// 如果超过3个字节,直接干掉
			if (word_byte <= 3)
				wCount++;
			else
				i--;
		}
	}
	return wCount;
}

int FmUtils::YYConverUtf8ToUnicode(TCHAR *destUnicode, const char* srcUtf8)
{
	int i = 0;

	if ((0x80 & srcUtf8[0]) == 0) {
		// 1字节
		destUnicode[0] = srcUtf8[i++];
	}
	else if ((0xE0 & srcUtf8[0]) == 0xC0) {
		// 2字节
		destUnicode[0] = (0x1F & srcUtf8[i++]) << 6;
		destUnicode[0] |= (0x3F & srcUtf8[i++]);
	}
	else if ((0xF0 & srcUtf8[0]) == 0xE0) {
		// 3字节
		destUnicode[0] = (0x0F & srcUtf8[i++]) << 12;
		destUnicode[0] |= (0x3F & srcUtf8[i++]) << 6;
		destUnicode[0] |= (0x3F & srcUtf8[i++]);
	}
	else if ((0xF8 & srcUtf8[0]) == 0xF0) {
		// 4字节
		i = 4;
	}
	else if ((0xF8 & srcUtf8[0]) == 0xF0) {
		// 5字节
		i = 5;
	}
	else if ((0xF8 & srcUtf8[0]) == 0xF0) {
		// 6字节
		i = 6;
	}

	// 大于3字节的处理也没的鸟用.TCHAR不支持
	return i;
}

FmUtils *FmUtils::FmUtils::getInstance()
{
	if (s_FmUtils == nullptr)
	{
		s_FmUtils = new(std::nothrow) FmUtils;
	}
	return s_FmUtils;
}

FmUtils::FmUtils()
{

}

FmUtils::~FmUtils()
{

}

