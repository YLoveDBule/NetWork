#include "GameNetWorkManager.h"
#include <assert.h>


GameNetWorkManager::~GameNetWorkManager()
{

}

// c++11 Singleton
GameNetWorkManager& GameNetWorkManager::getInstance()
{
	static GameNetWorkManager s_GameNetWorkManager;
	return s_GameNetWorkManager;
}

void GameNetWorkManager::start()
{
	if (!m_isInited)
	{
		m_isInited = m_isValid = true;
	}
}

void GameNetWorkManager::clear()
{
	for (auto itor = m_gameNetWorkPool.begin(); itor != m_gameNetWorkPool.end(); ++itor)
	{
		if (itor->second)
		{
			itor->second->closeSock();
			itor->second.reset(nullptr);
		}
	}

	m_recorderSID.clear();
	m_gameNetWorkPool.clear();
	while (!m_gameNetWorkStack.empty())
	{
		if (m_gameNetWorkStack.top())
		{
			m_gameNetWorkStack.top().reset(nullptr);
			m_gameNetWorkStack.pop();
		}
	}

}

void GameNetWorkManager::end()
{
	m_isValid = false;
}

void GameNetWorkManager::registerMsgAdopter(std::unique_ptr<NetMsgAdpoter> &adopter)
{
	m_msgAdopter.reset(adopter.release());
}

void GameNetWorkManager::askToSubserver(const int recorder, const char*ip, const int port)
{
	std::unique_ptr<GameNetWork> network = nullptr;
	if (strlen(ip) != 0 && getValidNetwork(network))
	{
		if (network && network->init() && network->connectServer(ip,port))
		{
			addNewNet(network);
			m_recorderSID.insert(std::make_pair(recorder, network->getSocket()));
			
		}
	}
	else
	{
		assert("failed to create the socket!!!");
	}
}

void GameNetWorkManager::askToSubserverResult()
{
	for (auto itor : m_recorderSID)
	{
		pushServerAskResult(itor.first, itor.second);
	}
	m_recorderSID.clear();
}

void GameNetWorkManager::pushServerAskResult(const int recorder, const SOCKET socketId)
{
	if (m_msgAdopter)
	{
		m_msgAdopter->pushServerAskResult(recorder, socketId);
	}
}

void GameNetWorkManager::addNewNet(std::unique_ptr<GameNetWork>& value)
{
	if (m_gameNetWorkPool.find(value->getSocket())== m_gameNetWorkPool.end())
	{
		m_gameNetWorkPool.insert(std::make_pair(value->getSocket(), value));
	}
}


void GameNetWorkManager::removeNet(const SOCKET value)
{
	m_gameNetWorkPool.erase(value);
}

void GameNetWorkManager::collectIdleNW()
{
	if (m_gameNetWorkPool.size() > 0)
	{
		auto itor = m_gameNetWorkPool.begin();
		while (itor != m_gameNetWorkPool.end())
		{
			if (itor->second && !itor->second->isActive())
			{
				itor->second.reset(nullptr);
				itor = m_gameNetWorkPool.erase(itor);
			}
			else 
			{
				itor++;
			}
		}
	}
}

bool GameNetWorkManager::getValidNetwork(std::unique_ptr<GameNetWork>& value)
{
	value = make_unique<GameNetWork>();
	return (value != nullptr);
}

bool GameNetWorkManager::getIdleNetWork(std::unique_ptr<GameNetWork>& value)
{
	auto result = false;
	if (m_gameNetWorkStack.size() != 0)
	{
		value.reset(m_gameNetWorkStack.top().release());
		m_gameNetWorkStack.pop();
		result = true;
	}
	return result;
}

void GameNetWorkManager::closeConnect(const int sid)
{
	if (m_gameNetWorkPool.find(sid) != m_gameNetWorkPool.end)
	{
		if (m_gameNetWorkPool.at(sid))
		{
			m_gameNetWorkPool.at(sid)->closeSock();
			m_gameNetWorkStack.push(m_gameNetWorkPool.at(sid));
		}
		m_gameNetWorkPool.erase(sid);
	}
}

void GameNetWorkManager::closeAllConnect()
{
	while (!m_gameNetWorkPool.empty())
	{
		auto itor = m_gameNetWorkPool.begin();
		if (itor != m_gameNetWorkPool.end() && itor->second)
		{
			if (m_gameNetWorkPool.at(itor->first))
			{
				m_gameNetWorkPool.at(itor->first)->closeSock();
			}
			m_gameNetWorkPool.erase(itor);
		}
	}
	m_gameNetWorkPool.clear();
}

void GameNetWorkManager::beginSendMsg(const SOCKET socketId)
{
	if (m_gameNetWorkPool.at(socketId))
	{
		m_gameNetWorkPool.at(socketId)->beginSendMsg();
	}
}

void GameNetWorkManager::pushNum(const SOCKET socketId, void* data, const unsigned int size)
{
	if (m_gameNetWorkPool.at(socketId))
	{
		m_gameNetWorkPool.at(socketId)->pushNum(data, size);
	}
}

void GameNetWorkManager::pushLString(const SOCKET socketId, const char* data, const int size)
{
	if (m_gameNetWorkPool.at(socketId))
	{
		m_gameNetWorkPool.at(socketId)->pushLString(data, size);
	}
}

void GameNetWorkManager::pushLstringShortSize(const SOCKET socketId, const char* data, const short size)
{
	if (m_gameNetWorkPool.at(socketId))
	{
		m_gameNetWorkPool.at(socketId)->pushLString(data, size);
	}
}

void GameNetWorkManager::endSendMsg(const SOCKET socketId)
{
	if (m_gameNetWorkPool.at(socketId))
	{
		m_gameNetWorkPool.at(socketId)->endSendMsg();
	}
}

bool GameNetWorkManager::sendMessage(const SOCKET socketId, unsigned short wMainCmdID, unsigned short wSubCmdID, unsigned int& sendSize)
{
	auto result = false;
	if (m_gameNetWorkPool.at(socketId))
	{
		result = m_gameNetWorkPool.at(socketId)->sendData(wMainCmdID, wSubCmdID, sendSize);
	}
	return result;
}

bool GameNetWorkManager::sendMessage(const SOCKET socketId, unsigned short wMainCmdID, unsigned short wSubCmdID, void *pData, unsigned int& sendSize)
{
	auto result = false;
	if (m_gameNetWorkPool.at(socketId))
	{
		result = m_gameNetWorkPool.at(socketId)->sendData(wMainCmdID, wSubCmdID, pData, sendSize);
	}
	return result;
}

bool GameNetWorkManager::houseNetMessage(unsigned short wSocketID, TCP_Command command, void *pData, unsigned short wDataSize)
{
	if (m_msgAdopter)
	{
		return m_msgAdopter->houseNetMsg(wSocketID, command.wMainCmdID, command.wSubCmdID, pData,wDataSize);
	}
	return true;
}

bool GameNetWorkManager::OnEventTCPSocketShut(unsigned short wSocketID, unsigned char cbShutReason)
{
	if (m_msgAdopter)
	{
		return m_msgAdopter->onConnectShutdown(wSocketID, cbShutReason);
	}
	return true;
}

GameNetWorkManager::GameNetWorkManager()
	:m_isValid(true),
	m_isInited(false),
	m_macAddress(false),
	m_msgAdopter(nullptr)
{
	clear();
}

void GameNetWorkManager::updateNetWorkPool()
{
	for (auto itor = m_gameNetWorkPool.begin(); itor != m_gameNetWorkPool.end(); ++ itor)
	{
		if (itor->second)
		{
			itor->second->update();
		}
	}
}
