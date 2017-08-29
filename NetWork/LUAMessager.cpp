#include "LUAMessager.h"
#include "scripting/lua-bindings/manual/CCLuaEngine.h"
#include "TCPCommon.h"
namespace LuaNetMessage {
	LUAMessager * LUAMessager::s_luaMessager = nullptr;
	LUAMessager *LUAMessager::getInstance()
	{
		if (s_luaMessager == nullptr)
		{
			s_luaMessager = new(std::nothrow) LUAMessager();
		}
		return s_luaMessager;
	}

	LUAMessager::~LUAMessager()
	{

	}

	bool LUAMessager::houseNetMsg(SOCKET wSocketID, unsigned short maincmd, unsigned short subcmd, void *pData, unsigned short wDataSize)
	{
		PacketMsgToLua jsMsg;
		constructMsgToLua(jsMsg, wSocketID, maincmd, subcmd, pData, wDataSize);
		m_packetMsgToJSList.push(jsMsg);
		return true;
	}

	bool LUAMessager::houseProcMsg(SOCKET wSocketID, unsigned short maincmd, unsigned short subcmd, void *pData, unsigned short wDataSize)
	{
		return true;
	}

	bool LUAMessager::pushServerAskResult(const int recorder, SOCKET sid)
	{
		initLuaState();
		if (m_luaState)
		{
			try
			{
				lua_getglobal(m_luaState, "Lua_Global_pushServerAskResult");
				lua_pushinteger(m_luaState, recorder);
				lua_pushinteger(m_luaState, sid);
				lua_pcall(m_luaState, 2, 0,0);
				return true;
			}
			catch (...)
			{
				return false;
			}
		}
		return false;
	}

	bool LUAMessager::onConnectShutdown(unsigned short sockid, unsigned char shutReason)
	{
		NetStatusMsg nsm;
		nsm.m_ID = sockid;
		nsm.m_Re = shutReason;
		m_netStatusMsgList.push(nsm);
		return true;
	}

	bool LUAMessager::sendToLua(SOCKET wSocketID, unsigned short wMainCmd, unsigned short wSubCmd, void *pData, unsigned short wDataSize)
	{
		initLuaState();
		if (m_luaState)
		{
			try
			{
				lua_getglobal(m_luaState, "Lua_Global_recvmsg");
				lua_pushinteger(m_luaState, wSocketID);
				lua_pushinteger(m_luaState, wMainCmd);
				lua_pushinteger(m_luaState, wSubCmd);
				lua_pushlstring(m_luaState, (char*)pData, wDataSize);
				lua_pushinteger(m_luaState, wDataSize);
				lua_pcall(m_luaState, 5, 0, NULL);
				return true;
			}
			catch (...)
			{
				return false;
			}
		}
		return false;
	}

	void LUAMessager::dispatchNetMsg()
	{
		if (!m_packetMsgToJSList.empty())
		{
			std::shared_ptr<PacketMsgToLua> jsmsg = m_packetMsgToJSList.try_pop();
			if (jsmsg)
			{
				sendToLua(jsmsg->m_socketID, jsmsg->m_tcpcommand.wMainCmdID, jsmsg->m_tcpcommand.wSubCmdID, (void*)jsmsg->m_dataBuffer, jsmsg->m_dataSize);
				jsmsg.reset();
			}
		}
	}

	bool LUAMessager::dispatchNetStatusMsg()
	{
		initLuaState();
		if (!m_netStatusMsgList.empty())
		{
			std::shared_ptr<NetStatusMsg> statusmsg = m_netStatusMsgList.try_pop();
			if (statusmsg)
			{
				if (m_luaState)
				{
					lua_getglobal(m_luaState, "Lua_Global_onNetworkStatus");
					lua_pushinteger(m_luaState, statusmsg->m_ID);
					lua_pushinteger(m_luaState, statusmsg->m_Re);

					lua_pcall(m_luaState, 2, 0, NULL);
					statusmsg.reset();
					return true;
				}
				return false;
			}
		}
		return false;
	}

	void LUAMessager::update(float dt)
	{
		dispatchNetMsg();
		dispatchNetStatusMsg();
	}
	void LUAMessager::closeConnet(int sid)
	{
		GameNetWorkManager::getInstance().closeConnect(sid);
	}

	void LUAMessager::closeAllConnect()
	{
		GameNetWorkManager::getInstance().closeAllConnect();
	}

	void LUAMessager::beginSendMsg(SOCKET value)
	{
		GameNetWorkManager::getInstance().beginSendMsg(value);
	}

	void LUAMessager::pushNum(SOCKET socketID, void *pData, unsigned int size)
	{
		GameNetWorkManager::getInstance().pushNum(socketID, pData, size);
	}

	void LUAMessager::pushLString(SOCKET socket, char* pData, int size)
	{
		GameNetWorkManager::getInstance().pushLString(socket, pData, size);
	}

	void LUAMessager::pushLStringShortSize(SOCKET socket, const char* pData, short size)
	{
		GameNetWorkManager::getInstance().pushLstringShortSize(socket, pData, size);
	}

	void LUAMessager::endSendMsg(SOCKET socket)
	{
		GameNetWorkManager::getInstance().endSendMsg(socket);
	}

	bool LUAMessager::sendMessage(const SOCKET socketId, unsigned short wMainCmdID, unsigned short wSubCmdID)
	{
		unsigned int sendSize = 0;
		auto result = GameNetWorkManager::getInstance().sendMessage(socketId, wMainCmdID, wSubCmdID, sendSize);
		return result;
	}

	bool LUAMessager::sendMessage(const SOCKET socketId, unsigned short wMainCmdID, unsigned short wSubCmdID, void *pData, unsigned int& sendSize)
	{
		auto result = GameNetWorkManager::getInstance().sendMessage(socketId, wMainCmdID, wSubCmdID, pData, sendSize);
		return result;
	}

	void LUAMessager::constructMsgToLua(PacketMsgToLua &pmjs, SOCKET wSocketID, unsigned short wMainCmdID, unsigned short wSubCmd, void* pData, unsigned short wDataSize)
	{
		pmjs.m_socketID = wSocketID;
		pmjs.m_tcpcommand.wMainCmdID = wMainCmdID;
		pmjs.m_tcpcommand.wSubCmdID = wSubCmd;
		memset(pmjs.m_dataBuffer, 0, IPC_PACKET);
		memcpy(pmjs.m_dataBuffer, (char*)pData, wDataSize);
		pmjs.m_dataSize = wDataSize;
	}

	void LUAMessager::ask2Subserver(int recorder, const char *ip, int port)
	{
		GameNetWorkManager::getInstance().askToSubserver(recorder, ip, port);
	}

	void LUAMessager::initLuaState()
	{
		m_luaState = ((m_luaState == nullptr) ? (cocos2d::LuaEngine::getInstance()->getLuaStack()->getLuaState()) : m_luaState);
	}

	LUAMessager::LUAMessager()
		:m_luaState(nullptr)
	{
		GameNetWorkManager::getInstance().registerMsgAdopter(this);
	}
}