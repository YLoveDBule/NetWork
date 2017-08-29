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
			delete itor->second;
			itor->second = nullptr;
		}
	}

	m_recorderSID.clear();
	m_gameNetWorkPool.clear();
	while (!m_gameNetWorkStack.empty())
	{
		if (m_gameNetWorkStack.top())
		{
			delete m_gameNetWorkStack.top();
			m_gameNetWorkStack.top() = nullptr;
			m_gameNetWorkStack.pop();
		}
	}
	GameNetWorkStack empty;
	m_gameNetWorkStack.swap(empty);
}

void GameNetWorkManager::end()
{
	m_isValid = false;
}

void GameNetWorkManager::registerMsgAdopter(NetMsgAdpoter *adopter)
{
	m_msgAdopter = adopter;
}

void GameNetWorkManager::askToSubserver(const int recorder, const char*ip, const int port)
{
	GameNetWork *network = nullptr;
	if (strlen(ip) != 0 && getValidNetwork(&network))
	{
		if (network && network->init() && network->connectServer(ip,port))
		{
			/*if (m_gameNetWorkPool.find(network->getSocket()) == m_gameNetWorkPool.end())
			{
				m_gameNetWorkPool.insert(std::make_pair(network->getSocket(), network));
			}*/
			addNewNet(network);
			m_recorderSID.insert(std::make_pair(recorder, network->getSocket()));
			network->multiStart();
			askToSubserverResult();
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

void GameNetWorkManager::addNewNet(GameNetWork *value)
{
	if (m_gameNetWorkPool.find(value->getSocket()) == m_gameNetWorkPool.end())
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
				delete itor->second;
				itor->second = nullptr;
				itor = m_gameNetWorkPool.erase(itor);
			}
			else 
			{
				itor++;
			}
		}
	}
}


bool GameNetWorkManager::getValidNetwork(GameNetWork **value)
{
	bool result = false;

	if (!result) {
		result = ((*value = new GameNetWork()) != nullptr);
	}

	return result;
}

bool GameNetWorkManager::getIdleNetWork(GameNetWork* value)
{
	auto result = false;
	if (m_gameNetWorkStack.size() != 0)
	{
		value = m_gameNetWorkStack.top();
		m_gameNetWorkStack.pop();
		result = true;
	}
	return result;
}

void GameNetWorkManager::closeConnect(const int sid)
{
	if (m_gameNetWorkPool.find(sid) != m_gameNetWorkPool.end())
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

void GameNetWorkManager::pushLString(const SOCKET socketId, char* data, const int size)
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

void GameNetWorkManager::update(float dt /*= 0.0f*/)
{
	if (m_isValid)
	{
		collectIdleNW();
		updateNetWorkPool();
	}
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
