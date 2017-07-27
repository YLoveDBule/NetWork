#include "JSMessager.h"
#include "ScriptingCore.h"
#include "ccUTF8.h"

JSMessager * JSMessager::s_jsMessager = nullptr;
JSMessager *JSMessager::getInstance()
{
	if (s_jsMessager == nullptr)
	{
		s_jsMessager = new(std::nothrow) JSMessager();
	}
	return s_jsMessager;
}

JSMessager::~JSMessager()
{

}

bool JSMessager::houseNetMsg(SOCKET wSocketID, unsigned short maincmd, unsigned short subcmd, void *pData, unsigned short wDataSize)
{
	PacketMsgToJs jsMsg;
	constructMsgToJs(jsMsg, wSocketID, maincmd, subcmd, pData, wDataSize);
	m_packetMsgToJSList.push(jsMsg);
	return true;
}

bool JSMessager::houseProcMsg(SOCKET wSocketID, unsigned short maincmd, unsigned short subcmd, void *pData, unsigned short wDataSize)
{
	return true;
}

bool JSMessager::pushServerAskResult(const int recorder, SOCKET sid)
{
	try
	{
		auto stringcmd = cocos2d::StringUtils::format("JS_Global_ask2subserverResult(%d,%u)", recorder, sid);
		ScriptingCore::getInstance()->evalString(stringcmd.c_str());
		return true;
	}
	catch (...)
	{
		return false;
	}
	return false;
}

bool JSMessager::onConnectShutdown(unsigned short sockid, unsigned char shutReason)
{
	NetStatusMsg nsm;
	nsm.m_ID = sockid;
	nsm.m_Re = shutReason;
	m_netStatusMsgList.push(nsm);
	return true;
}

bool JSMessager::sendToJs(SOCKET wSocketID, unsigned short wMainCmd, unsigned short wSubCmd, void *pData, unsigned short wDataSize)
{
	try
	{
		auto stringcmd = cocos2d::StringUtils::format("JS_Global_recvmsg(%u,%hu,%hu,%p,%hu)", wSocketID, wMainCmd, wSubCmd, pData, wDataSize);
		ScriptingCore::getInstance()->evalString(stringcmd.c_str());
		return true;
	}
	catch(...)
	{
		return false;
	}
	return false;
}

void JSMessager::dispatchNetMsg()
{
	if (!m_packetMsgToJSList.empty())
	{
		std::shared_ptr<PacketMsgToJs> jsmsg = m_packetMsgToJSList.try_pop();
		if (jsmsg)
		{
			sendToJs(jsmsg->m_socketID, jsmsg->m_tcpcommand.wMainCmdID, jsmsg->m_tcpcommand.wSubCmdID, (void*)jsmsg->m_dataBuffer, jsmsg->m_dataSize);
			jsmsg.reset();
		}
	}
}

bool JSMessager::dispatchNetStatusMsg()
{
	if (!m_netStatusMsgList.empty())
	{
		std::shared_ptr<NetStatusMsg> statusmsg = m_netStatusMsgList.try_pop();
		if (statusmsg)
		{
			auto stringcmd = cocos2d::StringUtils::format("JS_Global_onNetWorkStatus(%hu,%c)", statusmsg->m_ID, statusmsg->m_Re);
			ScriptingCore::getInstance()->evalString(stringcmd.c_str());
			return true;
		}
	}
	return false;
}

void JSMessager::update(float dt)
{
	dispatchNetMsg();
	dispatchNetStatusMsg();
}

void JSMessager::closeConnet(int sid)
{
	GameNetWorkManager::getInstance().closeConnect(sid);
}

void JSMessager::closeAllConnect()
{
	GameNetWorkManager::getInstance().closeAllConnect();
}

void JSMessager::beginSendMsg(SOCKET value)
{
	GameNetWorkManager::getInstance().beginSendMsg(value);
}

void JSMessager::pushNum(SOCKET socketID, void *pData, unsigned int size)
{
	GameNetWorkManager::getInstance().pushNum(socketID, pData, size);
}

void JSMessager::pushLString(SOCKET socket, const char* pData, int size)
{
	GameNetWorkManager::getInstance().pushLString(socket, pData, size);
}

void JSMessager::pushLStringShortSize(SOCKET socket, const char* pData, short size)
{
	GameNetWorkManager::getInstance().pushLstringShortSize(socket, pData, size);
}

void JSMessager::endSendMsg(SOCKET socket)
{
	GameNetWorkManager::getInstance().endSendMsg(socket);
}

bool JSMessager::sendMessage(const SOCKET socketId, unsigned short wMainCmdID, unsigned short wSubCmdID)
{
	unsigned int sendSize = 0;
	auto result = GameNetWorkManager::getInstance().sendMessage(socketId, wMainCmdID, wSubCmdID, sendSize);
	return result;
}

bool JSMessager::sendMessage(const SOCKET socketId, unsigned short wMainCmdID, unsigned short wSubCmdID, void *pData, unsigned int& sendSize)
{
	auto result = GameNetWorkManager::getInstance().sendMessage(socketId, wMainCmdID, wSubCmdID,pData, sendSize);
	return result;
}

void JSMessager::constructMsgToJs(PacketMsgToJs &pmjs, SOCKET wSocketID, unsigned short wMainCmdID, unsigned short wSubCmd, void* pData, unsigned short wDataSize)
{
	pmjs.m_socketID = wSocketID;
	pmjs.m_tcpcommand.wMainCmdID = wMainCmdID;
	pmjs.m_tcpcommand.wSubCmdID = wSubCmd;
	memset(pmjs.m_dataBuffer, 0, IPC_PACKET);
	memcpy(pmjs.m_dataBuffer, (char*)pData, wDataSize);
	pmjs.m_dataSize = wDataSize;
}

void JSMessager::ask2Subserver(int recorder, const char *ip, int port)
{
	GameNetWorkManager::getInstance().askToSubserver(recorder, ip, port);
}

JSMessager::JSMessager()
{
	GameNetWorkManager::getInstance().registerMsgAdopter(this);
}

