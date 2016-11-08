#pragma once



class WebUtil
{
public:
	~WebUtil();

	static WebUtil* getInstance();

	const char* getUSRGUID();

	int convertstr(const char *src_charset,
		const char *des_charset,
		char *srcbuff,
		size_t srclen,
		char *desbuff,
		size_t deslen);

	bool openwebpage(const std::string &);

private:
	WebUtil();
	WebUtil(const WebUtil&);

private:
	static WebUtil* Instance;

	char* MacAddress = nullptr;

};

#define WebUtility WebUtil::getInstance()