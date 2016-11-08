#ifndef _CJNETWORKMANAGER_H_
#define _CJNETWORKMANAGER_H_

#pragma once

#include "CJNetwork.h"
#include <vector>
#include <stack>
#include <map>



using namespace std;

class NetMsgAdpoter
{
public:
	virtual bool houseNetMsg(SOCKET wSocketID,
		unsigned short maincmd,
		unsigned short subcmd,
		void *pData,
		unsigned short wDataSize) = NULL;

	virtual bool pushServerAskResult(int recorder, SOCKET sid) = NULL;
};

typedef struct PacketMsg2Lua
{
	SOCKET Sockid;
	TCP_Command Comm;
	char* DataBuff = nullptr;
	unsigned short DataSize;

	void clear() {
		if (DataBuff) {
			delete[] DataBuff;
			DataBuff = nullptr;
		}
	}
} PacketMsg2Lua;

class __declspec(dllexport) CJNetworkManager
{
	friend class CJNetwork;

public:
	~CJNetworkManager();

	static CJNetworkManager* getInstance();

	void start();

	void update(float dt);

	void multiUpdate();

	void end();

	void registerMsgAdopter(NetMsgAdpoter*);

public:
	void ask2Subserver(int, const char*, int);

	void ask2subserverResult();

	void closeConnect(int sid);

	void closeAllConnect();

	void beginSendMsg(SOCKET socket);

	void pushNum(SOCKET socket, void* data, NumbericSize size);

	void pushLString(SOCKET socket, char* data, int size);

	void pushLStringShortSize(SOCKET socket, char* data, short size);

	void endSendMsg(SOCKET socket);

	bool sendMessage(SOCKET socket,
		unsigned short wMainCmdID,
		unsigned short wSubCmdID);

	bool sendMessage(SOCKET socket,
		unsigned short wMainCmdID,
		unsigned short wSubCmdID,
		void *pData, 
		unsigned short wDataSize);

	//const char* getUSRGUID();

public:
	//size_t convertstr(const char *src_charset,
	//	const char *des_charset,
	//	char *srcbuff,
	//	size_t srclen,
	//	char *desbuff,
	//	size_t deslen);

	bool houseNetMsg(unsigned short wSocketID,
		TCP_Command Command,
		void *pData,
		unsigned short wDataSize);

protected:
	std::mutex Msg2Lua_mutex;

private:
	CJNetworkManager();

	void clear();

	bool getValidNetw(CJNetwork**);

	bool getIdleNetw(CJNetwork**);

	void addNewNetw(CJNetwork*);

	void removeNetw(SOCKET value);

	void collectIdleNW();

	void pushServerAskResult(int recorder, SOCKET sid);

	bool sendMsgs2Lua();

private:
	static CJNetworkManager* Instance;

private:
	bool IsValid = false;

	bool IsInited = false;

	char* MacAddress = nullptr;

	NetMsgAdpoter* MsgAdopter = nullptr;

	typedef std::map<SOCKET, CJNetwork*> CJNetworkmap;

	CJNetworkmap CJNetworkPool;

	typedef std::stack<CJNetwork*> CJNetworkStack;

	CJNetworkStack CJNetworkIdleStack;

	std::map<int, int> RecorderSID;

	std::vector<PacketMsg2Lua*> PacketMsg2LuaList;
};

#define  CJNetworkMgr CJNetworkManager::getInstance()

#endif