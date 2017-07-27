/*
Title: JSMessager
Des: handle msg transfer between js and app.
*/

#pragma once
#ifndef __JSMESSAGER__
#define __JSMESSAGER__
#include "GameNetWorkManager.h"
#include "cocos2d.h"


class PacketMsgToJs
{
public:
	SOCKET m_socketID;
	TCP_Command m_tcpcommand;
	char m_dataBuffer[IPC_PACKET];
	unsigned short m_dataSize;

	PacketMsgToJs() :
		m_dataSize(0)
	{
		memset(m_dataBuffer, 0, IPC_PACKET);
	}
};

class NetStatusMsg
{
public:
	unsigned short m_ID;
	unsigned char m_Re;
	NetStatusMsg() :
		m_ID(0),
		m_Re(0)
	{

	}
};

class JSMessager : public NetMsgAdpoter 
{
public:
	static JSMessager *s_jsMessager;
	static JSMessager *getInstance();
	JSMessager();
	virtual ~JSMessager();
	virtual bool houseNetMsg(SOCKET wSocketID, unsigned short maincmd, unsigned short subcmd, void *pData, unsigned short wDataSize) override;
	virtual bool houseProcMsg(SOCKET wSocketID, unsigned short maincmd, unsigned short subcmd, void *pData, unsigned short wDataSize) override;
	virtual bool pushServerAskResult(const int recorder, SOCKET sid) override;
	virtual bool onConnectShutdown(unsigned short sockid, unsigned char shutReason) override;
	virtual bool sendToJs(SOCKET wSocketID, unsigned short wMainCmd, unsigned short wSubCmd, void *pData, unsigned short wDataSize);
	virtual void dispatchNetMsg();
	virtual bool dispatchNetStatusMsg();
	virtual void update(float dt);
	virtual void closeConnet(int sid);
	virtual void closeAllConnect();
	virtual void beginSendMsg(SOCKET value);
	virtual void pushNum(SOCKET socketID, void *pData, unsigned int size);
	virtual void pushLString(SOCKET socket, const char* pData, int size);
	virtual void pushLStringShortSize(SOCKET socket, const char* pData, short size);
	virtual void endSendMsg(SOCKET socket);
	virtual bool sendMessage(const SOCKET socketId, unsigned short wMainCmdID, unsigned short wSubCmdID);
	virtual bool sendMessage(const SOCKET socketId, unsigned short wMainCmdID, unsigned short wSubCmdID, void *pData, unsigned int& sendSize);
	virtual void constructMsgToJs(PacketMsgToJs &pmjs, SOCKET wSocketID, unsigned short wMainCmdID, unsigned short wSubCmd, void* pData, unsigned short wDataSize);
	virtual void ask2Subserver(int recorder, const char *ip, int port);
private:
	NimeiQueue<PacketMsgToJs> m_packetMsgToJSList;
	NimeiQueue<NetStatusMsg> m_netStatusMsgList;
};


#endif