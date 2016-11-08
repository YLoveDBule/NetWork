#include <string>
#include <assert.h>
#include <iconv.h>

#include "curl/curl.h"
#include "platform/CCPlatformDefine.h"
#include "WebUtil.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || \
	CC_TARGET_PLATFORM == CC_PLATFORM_WINRT 

#include <windows.h>
#include <Iphlpapi.h>

#pragma comment(lib, "iphlpapi.lib")
#else

#endif

#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC || \
	CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "ObjectHelper.h"
#endif




#pragma pack(1)

WebUtil* WebUtil::Instance = nullptr;

WebUtil* WebUtil::getInstance()
{
	if (Instance == nullptr) {
		Instance = new WebUtil;
	}

	return Instance;
}

WebUtil::WebUtil()
{

}

WebUtil::WebUtil(const WebUtil&)
{

}

WebUtil::~WebUtil()
{

}

const char* WebUtil::getUSRGUID()
{
	if (MacAddress != nullptr) return MacAddress;

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || \
	CC_TARGET_PLATFORM == CC_PLATFORM_WINRT 
	PIP_ADAPTER_INFO AdapterInfo;
	DWORD dwBufLen = sizeof(AdapterInfo);
	MacAddress = (char*)malloc(17);

	AdapterInfo = (IP_ADAPTER_INFO *)malloc(sizeof(IP_ADAPTER_INFO));
	if (AdapterInfo == nullptr) {
		printf("Error allocating memory needed to call GetAdaptersinfo\n");
	}

	if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == ERROR_BUFFER_OVERFLOW) {

		AdapterInfo = (IP_ADAPTER_INFO *)malloc(dwBufLen);
		if (AdapterInfo == nullptr) {
			printf("Error allocating memory needed to call GetAdaptersinfo\n");
		}
	}

	if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == NO_ERROR) {
		PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
		do {
			sprintf(MacAddress, "%02X:%02X:%02X:%02X:%02X:%02X",
				pAdapterInfo->Address[0], pAdapterInfo->Address[1],
				pAdapterInfo->Address[2], pAdapterInfo->Address[3],
				pAdapterInfo->Address[4], pAdapterInfo->Address[5]);
			return MacAddress;

			printf("\n");
			pAdapterInfo = pAdapterInfo->Next;
		} while (pAdapterInfo);
	}
	free(AdapterInfo);
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_MAC || \
	CC_TARGET_PLATFORM == CC_PLATFORM_IOS)

	return getIOSUUID().c_str();
#endif

	return nullptr;
}

int WebUtil::convertstr(const char *src_charset, const char *des_charset, char *srcbuff, size_t srclen, char *desbuff, size_t deslen)
{
	iconv_t cd;

	if ((cd = iconv_open(des_charset, src_charset)) == (iconv_t)(-1)) {
		return -1;
	}

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || \
	CC_TARGET_PLATFORM == CC_PLATFORM_WINRT 
	int rc = iconv(cd, (const char**)&srcbuff, &srclen, &desbuff, &deslen);
#else
	int rc = iconv(cd, &srcbuff, &srclen, &desbuff, &deslen);
#endif
	if (rc == -1) {

	}

	iconv_close(cd);

	return 1;
}

bool WebUtil::openwebpage(const std::string &url)
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || \
	CC_TARGET_PLATFORM == CC_PLATFORM_WINRT 
	if (!url.empty()) {
		ShellExecuteA(NULL, "open",
			url.c_str(), NULL, NULL, SW_SHOWNORMAL);
	}
#else

#endif

	return true;
}

//bool WebUtil::openwebpage(const std::string &url)
//{
//#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || \
//CC_TARGET_PLATFORM == CC_PLATFORM_WINRT 
//#else
//
//#endif
//	bool result = false;
//
//	/*CURL* curl = curl_easy_init();
//
//	if (curl) {
//		curl_easy_setopt(curl, CURLOPT_URL, "http://example.com");
//		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
//
//		CURLcode res = curl_easy_perform(curl);
//		result = true;
//		if (res != CURLE_OK) {
//			std::string err = curl_easy_strerror(res);
//			result = false;
//		}
//		curl_easy_cleanup(curl);
//	}*/
//
//	CURL *curl;
//	CURLcode res;
//
//	curl = curl_easy_init();
//	if (curl) {
//		curl_easy_setopt(curl, CURLOPT_URL, "http://www.example.com");
//		/* example.com is redirected, so we tell libcurl to follow redirection */
//		//curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
//
//		/* Perform the request, res will get the return code */
//		res = curl_easy_perform(curl);
//		/* Check for errors */
//		if (res != CURLE_OK)
//			fprintf(stderr, "curl_easy_perform() failed: %s\n",
//				curl_easy_strerror(res));
//
//		/* always cleanup */
//		curl_easy_cleanup(curl);
//	}
//
//	return result;
//}
