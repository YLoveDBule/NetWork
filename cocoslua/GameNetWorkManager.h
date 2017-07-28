#pragma once
#ifndef __GAMENETWORKMANAGER__
#define __GAMENETWORKMANAGER__
#include "TCPSocket.h"
#include "Define.h"
#include "GameNetWork.h"
#include <map>
#include <stack>
#include "LongLiveBase.h"

class NetMsgAdpoter
{
public:
	virtual bool houseNetMsg(SOCKET wSocketID, unsigned short maincmd, unsigned short subcmd, void *pData, unsigned short wDataSize) = 0;
	virtual bool houseProcMsg(SOCKET wSocketID, unsigned short maincmd, unsigned short subcmd, void *pData, unsigned short wDataSize) = 0;
	virtual bool pushServerAskResult(const int recorder, SOCKET sid) = 0;
	virtual bool onConnectShutdown(unsigned short sockid, unsigned char shutReason) = 0;
};

class GameNetWorkManager : public LongLiveComponent
{
public:
	virtual ~GameNetWorkManager();
	static GameNetWorkManager&getInstance();
	virtual void start();
	virtual void clear();
	virtual void end();
	virtual void registerMsgAdopter(NetMsgAdpoter *adopter);
	virtual void askToSubserver(const int recorder, const char*ip, const int port);
	virtual void askToSubserverResult();
	virtual void pushServerAskResult(const int recorder, const SOCKET socketId);
	virtual void addNewNet(GameNetWork *value);
	virtual void removeNet(const SOCKET value);
	virtual void collectIdleNW();
	virtual bool getValidNetwork(GameNetWork **value);
	virtual bool getIdleNetWork(GameNetWork* value);
	virtual void closeConnect(const int sid);
	virtual void closeAllConnect();
	virtual void beginSendMsg(const SOCKET socketId);
	virtual void pushNum(const SOCKET socketId, void* data, const unsigned int size);
	virtual void pushLString(const SOCKET socketId, const char* data, const int size);
	virtual void pushLstringShortSize(const SOCKET socketId, const char* data, const short size);
	virtual void endSendMsg(const SOCKET socketId);
	virtual bool sendMessage(const SOCKET socketId, unsigned short wMainCmdID,  unsigned short wSubCmdID,  unsigned int& sendSize);
	virtual bool sendMessage(const SOCKET socketId, unsigned short wMainCmdID, unsigned short wSubCmdID, void *pData,unsigned int& sendSize);
	virtual bool houseNetMessage(unsigned short wSocketID, TCP_Command command, void *pData, unsigned short wDataSize);
	virtual bool OnEventTCPSocketShut(unsigned short wSocketID, unsigned char cbShutReason);
	virtual void update(float dt = 0.0f) override;
private:
	GameNetWorkManager();
	void updateNetWorkPool();
private:
	bool m_isValid;
	bool m_isInited;
	char * m_macAddress;
	NetMsgAdpoter * m_msgAdopter;
	typedef std::map<SOCKET, GameNetWork *> GameNetMap;
	GameNetMap m_gameNetWorkPool;
	typedef std::stack<GameNetWork *> GameNetWorkStack;
	GameNetWorkStack m_gameNetWorkStack;
	std::map<int, int > m_recorderSID;
};

#endif