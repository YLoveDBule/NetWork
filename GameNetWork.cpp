#include "GameNetWork.h"

GameNetWork::GameNetWork()
	:m_tcpSocket(nullptr)
{
	
}

GameNetWork::~GameNetWork()
{
	if (m_tcpSocket)
	{
		m_tcpSocket->closeSock();
		m_tcpSocket.reset(nullptr);
	}
}

bool GameNetWork::init()
{
	if (m_tcpSocket == nullptr)
	{
		m_tcpSocket = make_unique<TCPSocket>();
	}
	return (m_tcpSocket->init() && m_tcpSocket->registerSink(this));
}

void GameNetWork::update()
{
	if (m_tcpSocket)
	{
		if (m_tcpSocket->isActive())
		{
			m_tcpSocket->update();
		}
	}
}

void GameNetWork::multiStart()
{
	std::thread t(&GameNetWork::multiEntry, this);
	if (t.joinable())
	{
		t.detach();
	}
}

void GameNetWork::multiEntry()
{

}

bool GameNetWork::connectServer(const std::string serverIp, const int serverPort)
{
	return m_tcpSocket && m_tcpSocket->connectServer(serverIp, serverPort);
}

bool GameNetWork::connectServer(const unsigned long ip, const int port)
{
	return m_tcpSocket && m_tcpSocket->connectServer(ip, port);
}

void GameNetWork::closeSock()
{
	if (m_tcpSocket)
	{
		m_tcpSocket->closeSock();
	}
}

void GameNetWork::beginSendMsg()
{
	if (m_tcpSocket)
	{
		m_tcpSocket->beginSendMsg();
	}
}

void GameNetWork::pushNum(void * data, const unsigned int size)
{
	if (m_tcpSocket)
	{
		m_tcpSocket->pushNum(data, size);
	}
}

void GameNetWork::pushLString(const char* data, const int size)
{
	if (m_tcpSocket)
	{
		m_tcpSocket->pushLString(data, size);
	}
}

void GameNetWork::pushLString(const char* data, const short size)
{
	if (m_tcpSocket)
	{
		m_tcpSocket->pushLString(data, size);
	}
}

void GameNetWork::endSendMsg()
{
	if (m_tcpSocket)
	{
		m_tcpSocket->endSendMsg();
	}
}

bool GameNetWork::sendData(unsigned short wMainCmdID, unsigned short wSubCmdID, unsigned int& sendedSize)
{
	if (m_tcpSocket)
	{
		return m_tcpSocket->sendData(wMainCmdID, wSubCmdID, sendedSize);
	}
	return false;
}

bool GameNetWork::sendData(unsigned short wMainCmdID, unsigned short wSubCmdID, void * pData, unsigned short wDataSize)
{
	if (m_tcpSocket)
	{
		return m_tcpSocket->sendData(wMainCmdID, wSubCmdID, pData, wDataSize);
	}
	return false;
}

bool GameNetWork::sendData()
{
	if (m_tcpSocket)
	{
		return m_tcpSocket->sendData();
	}
	return false;
}

SOCKET GameNetWork::getSocket()
{
	return m_tcpSocket ? m_tcpSocket->getSocket() : INVALID_SOCKET;
}

bool GameNetWork::isActive()
{
	if (m_tcpSocket)
	{
		return m_tcpSocket->isActive();
	}
	return false;
}

bool GameNetWork::OnEventTCPSocketLink(unsigned short wSocketID, int nErrorCode)
{
	return true;
}

bool GameNetWork::OnEventTCPSocketShut(unsigned short wSocketID, unsigned char cbShutReason)
{

}

bool GameNetWork::OnEventTCPSocketRead(unsigned short wSocketID, TCP_Command Command, void *pData, unsigned short wDataSize)
{

}
