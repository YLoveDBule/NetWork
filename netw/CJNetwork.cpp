#include <thread>
#include <functional>
#include <mutex>

#include "CJNetwork.h"
#include "CJNetworkManager.h"



#pragma pack(1)



CJNetwork::CJNetwork() : TS(nullptr)
{
}

CJNetwork::~CJNetwork()
{
	if (TS) {
		TS->closeSock();
		delete TS;
		TS = nullptr;
	}
}

bool 
CJNetwork::init()
{
	return ((TS || (TS = new TCPSocket())) &&
		TS->init()) &&
		(TS->registerSink(this));
}

void 
CJNetwork::update()
{
	if (TS && TS->isActive())
		TS->update();
}

void 
CJNetwork::multistart()
{
	std::thread t(&CJNetwork::multiupdate, this);
	if (t.joinable())
		t.detach();
}

void 
CJNetwork::multiupdate()
{
	while (TS && TS->isActive())
		TS->multiupdate();
}

void 
CJNetwork::registMgr(CJNetworkManager* mgr)
{
	NWmgr = mgr;
}

bool 
CJNetwork::connect(unsigned long ip, int port)
{
	return TS && TS->connect(ip, port);
}

bool 
CJNetwork::connect(const char* serverip, int port)
{
	return TS && TS->connect(serverip, port);
}

void 
CJNetwork::close()
{
	if (TS) {
		TS->closeSock();
	}
}

void 
CJNetwork::beginSendMsg()
{
	if (TS) TS->beginSendMsg();
}

void 
CJNetwork::pushNum(void* data, NumbericSize size)
{
	if (TS) TS->pushNum(data, size);
}

void 
CJNetwork::pushLString(char* data, int size)
{
	if (TS) TS->pushLString(data, size);
}

void 
CJNetwork::pushLString(char* data, short size)
{
	if (TS) TS->pushLString(data, size);
}

void 
CJNetwork::endSendMsg()
{
	if (TS) TS->endSendMsg();
}

bool 
CJNetwork::sendMessage(unsigned short wMainCmdID,
	unsigned short wSubCmdID)
{
	return TS && TS->sendMessage(wMainCmdID, wSubCmdID);
}

bool 
CJNetwork::sendMessage(unsigned short wMainCmdID, unsigned short wSubCmdID,
	void *pData, unsigned short wDataSize)
{
	return TS && TS->sendMessage(wMainCmdID, wSubCmdID, pData, wDataSize);
}

bool 
CJNetwork::sendMsg(unsigned short wMainCmdID,
	unsigned short wSubCmdID)
{
	return TS && TS->sendMsg(wMainCmdID, wSubCmdID);
}

bool 
CJNetwork::sendMsg(unsigned short wMainCmdID,
	unsigned short wSubCmdID,
	void *pData,
	unsigned short wDataSize)
{
	return TS && TS->sendMsg(wMainCmdID, wSubCmdID, pData, wDataSize);
}

SOCKET 
CJNetwork::getSocket() const
{
	return TS ? TS->getSocket() : INVALID_SOCKET;
}

bool 
CJNetwork::isActive()
{
	return TS && TS->isActive();
}

bool 
CJNetwork::OnEventTCPSocketLink(unsigned short wSocketID, int nErrorCode)
{
	return true;
}

bool 
CJNetwork::OnEventTCPSocketShut(unsigned short wSocketID,
	unsigned char cbShutReason)
{
	CJNetworkMgr->OnEventTCPSocketShut(wSocketID, cbShutReason);

	return true;
}

bool 
CJNetwork::OnEventTCPSocketRead(unsigned short wSocketID,
	TCP_Command Command,
	void *pData,
	unsigned short wDataSize)
{
	bool result = CJNetworkMgr->houseNetMsg(wSocketID,
		Command,
		pData,
		wDataSize);

	return result;
}