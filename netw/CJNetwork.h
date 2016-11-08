#pragma once

#ifndef __CJNETWORK__CLASS
#define __CJNETWORK__CLASS

#include "TCPSocket.h"



class CJNetworkManager;

class CJNetwork : public ITCPSocketSink
{
public:
	CJNetwork();
	~CJNetwork();

	bool init();
	void update();

	void multistart();
	void multiupdate();

	void registMgr(CJNetworkManager*);

	bool connect(unsigned long ip, int port);
	bool connect(const char* serverip, int port);
	void close();

	void beginSendMsg();
	void pushNum(void* data, NumbericSize size);
	void pushLString(char* data, int size);
	void pushLString(char* data, short size);
	void endSendMsg();
	bool sendMessage(unsigned short wMainCmdID, unsigned short wSubCmdID);
	bool sendMessage(unsigned short wMainCmdID, unsigned short wSubCmdID,
		void *pData, unsigned short wDataSize);

	bool sendMsg(unsigned short wMainCmdID, unsigned short wSubCmdID);

	bool sendMsg(unsigned short wMainCmdID, unsigned short wSubCmdID,
		void *pData, unsigned short wDataSize);

	SOCKET getSocket() const;

	bool isActive();

	bool OnEventTCPSocketLink(unsigned short wSocketID,
		int nErrorCode);

	bool OnEventTCPSocketShut(unsigned short wSocketID,
		unsigned char cbShutReason);

	inline bool OnEventTCPSocketRead(unsigned short wSocketID,
		TCP_Command Command,
		void * pData = NULL,
		unsigned short wDataSize = 0);

private:
	TCPSocket* TS = nullptr;
	CJNetworkManager* NWmgr = nullptr;
};

#endif