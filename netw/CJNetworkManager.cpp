#include <sstream>
#include <thread>
#include <functional>
#include <mutex>
#include <assert.h>
#include <iconv.h>

#include "CJNetworkManager.h"
#include "WHEncrypt.h"
#include "md5.h"



#if CJ_TARGET_PLATFORM == CJ_PLATFORM_WIN32

#include <Windows.h>
#include <Iphlpapi.h>

#pragma comment(lib, "iphlpapi.lib")
#else

#endif

#if (CJ_TARGET_PLATFORM == CJ_PLATFORM_MAC || \
	CJ_TARGET_PLATFORM == CJ_PLATFORM_IOS)
#include "ObjectHelper.h"
#endif



#pragma pack(1)



CJNetworkManager* CJNetworkManager::Instance = nullptr;

CJNetworkManager::~CJNetworkManager()
{
	IsValid = false;

	clear();

	if (Instance) {
		delete Instance;
		Instance = nullptr;
	}

	end();
}

CJNetworkManager* CJNetworkManager::getInstance()
{
	if (!Instance) {
		Instance = new CJNetworkManager();
		//Instance->start();
	}

	return Instance;
}

void 
CJNetworkManager::start()
{
	if (!IsInited) {
		IsValid = true;

		std::thread t(&CJNetworkManager::multiUpdate, this);
		if (t.joinable()) {
			t.detach();
		}
		IsInited = true;
	}
}

void 
CJNetworkManager::update(float dt)
{
	if (IsValid) {
		collectIdleNW();

		sendMsgs2Lua();
	}
}

void 
CJNetworkManager::multiUpdate()
{
	while (IsValid) {
		collectIdleNW();

		sendMsgs2Lua();
	}
}

void 
CJNetworkManager::end()
{
	IsValid = false;
}

void
CJNetworkManager::registerMsgAdopter(NetMsgAdpoter* adopter)
{
	MsgAdopter = adopter;
}

void 
CJNetworkManager::ask2Subserver(int recorder, const char* ip, int port)
{
	CJNetwork* netw = nullptr;
	if (strlen(ip) && getValidNetw(&netw)) {
		if (netw && netw->init() && netw->connect(ip, port)) {
			addNewNetw(netw);
			RecorderSID.insert(make_pair(recorder, netw->getSocket()));
			netw->multistart();

			ask2subserverResult();
		}
		else {
			// raise the warning
		}
	}
	else {
		assert(false && "$failed to create the socket!");
	}
}

void 
CJNetworkManager::ask2subserverResult()
{
	for (auto itor = RecorderSID.begin();
		itor != RecorderSID.end();
		itor++) {
		pushServerAskResult(itor->first, itor->second);
	}

	RecorderSID.clear();
}

void 
CJNetworkManager::closeConnect(int sid)
{
	if (CJNetworkPool.find(sid) != CJNetworkPool.end()) {
		CJNetworkPool[sid]->close();
		CJNetworkIdleStack.push(CJNetworkPool[sid]);
		CJNetworkPool.erase(sid);
	}
}

void 
CJNetworkManager::closeAllConnect()
{
	while (!CJNetworkPool.empty()) {
		auto itor = CJNetworkPool.begin();
		if (itor != CJNetworkPool.end() && itor->second) {
			CJNetworkPool[itor->first]->close();
			CJNetworkPool.erase(itor);
		}
	}

	CJNetworkPool.clear();
}

void 
CJNetworkManager::beginSendMsg(SOCKET socket)
{
	if (CJNetworkPool[socket]) CJNetworkPool[socket]->beginSendMsg();
}

void 
CJNetworkManager::pushNum(SOCKET socket, void* data, NumbericSize size)
{
	if (CJNetworkPool[socket]) CJNetworkPool[socket]->pushNum(data, size);
}

void 
CJNetworkManager::pushLString(SOCKET socket, char* data, int size)
{
	if (CJNetworkPool[socket]) CJNetworkPool[socket]->pushLString(data, size);
}

void 
CJNetworkManager::pushLStringShortSize(SOCKET socket,
	char* data,
	short size)
{
	if (CJNetworkPool[socket]) CJNetworkPool[socket]->pushLString(data, size);
}

void 
CJNetworkManager::endSendMsg(SOCKET socket)
{
	if (CJNetworkPool[socket]) CJNetworkPool[socket]->endSendMsg();
}

bool 
CJNetworkManager::sendMessage(SOCKET socket,
	unsigned short wMainCmdID,
	unsigned short wSubCmdID)
{
	bool result = false;
	if (CJNetworkPool[socket])
		result = CJNetworkPool[socket]->sendMessage(wMainCmdID, wSubCmdID);
	return result;
}

bool 
CJNetworkManager::sendMessage(SOCKET socket,
	unsigned short wMainCmdID,
	unsigned short wSubCmdID,
	void *pData,
	unsigned short wDataSize)
{
	bool result = false;
	if (CJNetworkPool[socket]) {
		result = CJNetworkPool[socket]->sendMessage(wMainCmdID,
			wSubCmdID,
			pData,
			wDataSize);
	}
	return result;
}

//const char* 
//CJNetworkManager::getUSRGUID()
//{
//	if (MacAddress != nullptr) return MacAddress;
//#if CJ_TARGET_PLATFORM == CJ_PLATFORM_WIN32
//	PIP_ADAPTER_INFO AdapterInfo;
//	DWORD dwBufLen = sizeof(AdapterInfo);
//	MacAddress = (char*)malloc(17);
//
//	AdapterInfo = (IP_ADAPTER_INFO *)malloc(sizeof(IP_ADAPTER_INFO));
//	if (AdapterInfo == nullptr) {
//		printf("Error allocating memory needed to call GetAdaptersinfo\n");
//	}
//
//	if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == ERROR_BUFFER_OVERFLOW) {
//
//		AdapterInfo = (IP_ADAPTER_INFO *)malloc(dwBufLen);
//		if (AdapterInfo == nullptr) {
//			printf("Error allocating memory needed to call GetAdaptersinfo\n");
//		}
//	}
//
//	if (GetAdaptersInfo(AdapterInfo, &dwBufLen) == NO_ERROR) {
//		PIP_ADAPTER_INFO pAdapterInfo = AdapterInfo;
//		do {
//			sprintf(MacAddress, "%02X:%02X:%02X:%02X:%02X:%02X",
//				pAdapterInfo->Address[0], pAdapterInfo->Address[1],
//				pAdapterInfo->Address[2], pAdapterInfo->Address[3],
//				pAdapterInfo->Address[4], pAdapterInfo->Address[5]);
//			return MacAddress;
//
//			printf("\n");
//			pAdapterInfo = pAdapterInfo->Next;
//		} while (pAdapterInfo);
//	}
//	free(AdapterInfo);
//#elif (CJ_TARGET_PLATFORM == CJ_PLATFORM_MAC || \
//	CJ_TARGET_PLATFORM == CJ_PLATFORM_IOS)
//
//	return getIOSUUID().c_str();
//#endif
//
//	return nullptr;
//}
//
//size_t 
//CJNetworkManager::convertstr(const char *src_charset,
//	const char *des_charset,
//	char *srcbuff,
//	size_t srclen,
//	char *desbuff,
//	size_t deslen)
//{
//	iconv_t cd;
//
//	if ((cd = iconv_open(des_charset, src_charset)) == (iconv_t)(-1)) {
//		return -1;
//	}
//
//#if CJ_TARGET_PLATFORM == CJ_PLATFORM_WIN32
//	int rc = iconv(cd, (const char**)&srcbuff, &srclen, &desbuff, &deslen);
//#else
//	int rc = iconv(cd, &srcbuff, &srclen, &desbuff, &deslen);
//#endif
//	if (rc == -1) {
//
//	}
//
//	iconv_close(cd);
//
//	return 1;
//}

CJNetworkManager::CJNetworkManager() {
	IsValid = true;
	clear();
}

void 
CJNetworkManager::clear()
{
	for (auto itor = CJNetworkPool.begin();
		itor != CJNetworkPool.end();
		itor++) {
		if (itor->second) {
			itor->second->close();
			delete itor->second;
			itor->second = nullptr;
		}
	}

	PacketMsg2LuaList.clear();
	RecorderSID.clear();
	CJNetworkPool.clear();

	while (!CJNetworkIdleStack.empty()) {
		if (CJNetworkIdleStack.top()) {
			delete CJNetworkIdleStack.top();
			CJNetworkIdleStack.top() = nullptr;
			CJNetworkIdleStack.pop();
		}
	}

	CJNetworkStack empty;
	CJNetworkIdleStack.swap(empty);
}

bool 
CJNetworkManager::getValidNetw(CJNetwork** nw)
{
	bool result = false;

	//if (CJNetworkIdleStack.size()) {
	//	*nw = CJNetworkIdleStack.top();
	//	CJNetworkIdleStack.pop();
	//	result = true;
	//}

	result = getIdleNetw(nw);

	if (!result) {
		result = ((*nw = new CJNetwork()) != nullptr);
	}

	return result;
}

bool 
CJNetworkManager::getIdleNetw(CJNetwork** nw)
{
	bool result = false;
	if (CJNetworkIdleStack.size()) {
		*nw = CJNetworkIdleStack.top();
		CJNetworkIdleStack.pop();
		result = true;
	}

	return result;
}

void 
CJNetworkManager::addNewNetw(CJNetwork* value)
{
	if (CJNetworkPool.find(value->getSocket()) == CJNetworkPool.end()) {
		CJNetworkPool.insert(make_pair(value->getSocket(), value));
	}
}

void 
CJNetworkManager::removeNetw(SOCKET value)
{
	CJNetworkPool.erase(value);
}

void 
CJNetworkManager::collectIdleNW()
{
	//std::lock_guard<std::mutex> guard(Msg2Lua_mutex); // TODO

	if (CJNetworkPool.size() > 0) {
		auto itor = CJNetworkPool.begin();

		while (itor != CJNetworkPool.end()) {
			if (itor->second && !itor->second->isActive()) {
				CJNetworkIdleStack.push(itor->second);
				itor = CJNetworkPool.erase(itor);
			}
			else {
				itor++;
			}
		}
	}
}

void 
CJNetworkManager::pushServerAskResult(int recorder, SOCKET sid)
{
	if (MsgAdopter) MsgAdopter->pushServerAskResult(recorder, sid);
}

bool 
CJNetworkManager::houseNetMsg(unsigned short wSocketID,
	TCP_Command Command,
	void *pData,
	unsigned short wDataSize)
{
	std::lock_guard<std::mutex> guard(Msg2Lua_mutex);

	//PacketMsg2Lua* pml = new PacketMsg2Lua;
	//pml->Sockid = wSocketID;
	//pml->Comm = Command;
	//pml->DataBuff = new char[wDataSize];
	//memcpy(pml->DataBuff, (char*)pData, wDataSize);
	//pml->DataSize = wDataSize;

	//std::lock_guard<std::mutex> guard(Msg2Lua_mutex);
	//PacketMsg2LuaList.push_back(pml);

	if (MsgAdopter) {
		return MsgAdopter->houseNetMsg(wSocketID, 
			Command.wMainCmdID, 
			Command.wSubCmdID, 
			pData, 
			wDataSize);
	}

	return true;
}

bool 
CJNetworkManager::OnEventTCPSocketShut(unsigned short wSocketID,
	unsigned char cbShutReason)
{
	if (MsgAdopter) {
		std::lock_guard<std::mutex> guard(Msg2Lua_mutex);
		return MsgAdopter->onConnectShutdown(wSocketID, cbShutReason);
	}

	return true;
}

bool 
CJNetworkManager::sendMsgs2Lua()
{
	//std::lock_guard<std::mutex> guard(Msg2Lua_mutex);

	//if (MsgAdopter) {

	//	for (std::vector<PacketMsg2Lua*>::size_type n = 0;
	//		n < PacketMsg2LuaList.size();)
	//	{
	//		if (n < PacketMsg2LuaList.size() && PacketMsg2LuaList[n]) {
	//			//std::lock_guard<std::mutex> guard(Msg2Lua_mutex);

	//			MsgAdopter->houseNetMsg(PacketMsg2LuaList[n]->Sockid,
	//				PacketMsg2LuaList[n]->Comm.wMainCmdID,
	//				PacketMsg2LuaList[n]->Comm.wSubCmdID,
	//				(char*)PacketMsg2LuaList[n]->DataBuff,
	//				PacketMsg2LuaList[n]->DataSize);

	//			PacketMsg2LuaList[n]->clear();
	//			delete PacketMsg2LuaList[n];

	//			PacketMsg2LuaList.erase(PacketMsg2LuaList.begin() + n);
	//		}
	//		else
	//			n++;
	//	}

	//	return true;
	//}

	return false;
}
