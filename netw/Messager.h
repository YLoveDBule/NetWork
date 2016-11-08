/*
Title: Messager
Des: handle msg transfer between lua and app.
*/


#pragma once

#include "cocos2d.h"
#include "scripting/lua-bindings/manual/CCLuaEngine.h"
#include "Packet.h"
#include "CJNetworkManager.h"

#if CUR_DEVICE_TYPE == DEVICE_TYPE_PC
#include "ProcessModuleManager.h"
#endif

USING_NS_CC;

class Messager : public NetMsgAdpoter, public LongLiveComponent
{
public:
	static Messager* getInstance();
	~Messager();

	virtual bool houseNetMsg(SOCKET wSocketID,
		unsigned short maincmd,
		unsigned short subcmd,
		void *pData,
		unsigned short wDataSize);

	virtual void update(float dt = 0.0f);

	virtual bool onConnectShutdown(unsigned short sockid,
		unsigned char shutReason);

public:
	bool send2Lua(SOCKET,
		unsigned short maincmd,
		unsigned short subcmd,
		void *pdata,
		unsigned short size);

	bool sendSelfUsrID(size_t id);

	int getSelfUsrID();

	virtual bool pushServerAskResult(int recorder, SOCKET sid);

	void ask2Subserver(int recorder, const char* ip, int port);

	void closeConnect(int sid);

	void suicide();

	void closeAllConnect();

	void beginSendMsg(SOCKET socket);

	void pushNum(SOCKET socket, void* data, unsigned int size);

	void pushLString(SOCKET socket, char* data, int size);

	void pushLStringShortSize(SOCKET socket, char* data, short size);

	void endSendMsg(SOCKET socket);

	bool sendMessage(SOCKET socket,
		unsigned short maincmd,
		unsigned short subcmd);

	bool sendMessage(SOCKET socket,
		unsigned short maincmd,
		unsigned short subcmd,
		void *pData,
		unsigned short wDataSize);

	void addFreeMainCMDForSub(int);

public:
	void minimizeWindow();

	void setIsSubProcess(bool value);

	bool isSubProcess() const;

private:
	Messager();
	Messager(const Messager&);

private:
	static Messager* Instance;

private:
	bool IsForPC = false;
	
	bool IsSubProcess = false;

	lua_State* LuaStatePointer = nullptr;

	unsigned int SingleBuffSendSize = 0;

	std::vector<PacketMsg2Lua*> PacketMsg2LuaList;

	char SingleBuff4Send[IPC_PACKET] = {0};

};

#define YourMessager Messager::getInstance()