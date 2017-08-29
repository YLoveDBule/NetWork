#pragma once
#ifndef __GAMENETWORK__
#define __GAMENETWORK__

#include "TCPSocket.h"
#include "Define.h"
class GameNetWork : public ITCPSocketSink 
{
public:
	GameNetWork();
	virtual ~GameNetWork();
	virtual bool init();
	virtual void update();
	virtual void multiStart();
	virtual void multiEntry();
	virtual bool connectServer(const unsigned long ip, const int port);
	virtual bool connectServer(const std::string serverIp, const int serverPort);
	virtual void closeSock();
	virtual void beginSendMsg();
	virtual void pushNum(void * data, const unsigned int size);
	virtual void pushLString(const char* data, const int size);
	virtual void pushLString(const char* data, const short size);
	virtual void endSendMsg();
	virtual bool sendData(unsigned short wMainCmdID, unsigned short wSubCmdID, unsigned int& sendedSize);
	virtual bool sendData(unsigned short wMainCmdID, unsigned short wSubCmdID, void * pData, unsigned short wDataSize);
	virtual bool sendData();
	virtual SOCKET getSocket();
	virtual bool isActive();
	//连接事件
	virtual bool OnEventTCPSocketLink(unsigned short wSocketID, int nErrorCode) override;

	//关闭事件
	virtual bool OnEventTCPSocketShut(unsigned short wSocketID, unsigned char cbShutReason) override;

	//读取事件
	virtual bool OnEventTCPSocketRead(unsigned short wSocketID,TCP_Command Command, void *pData,unsigned short wDataSize) override;

private:
	TCPSocket * m_tcpSocket;
	long long       _animationInterval;
	long long  getCurMillTime();
	
};
#endif