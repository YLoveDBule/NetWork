#include <chrono>  
#include "GameNetWork.h"
#include "GameNetWorkManager.h"
using namespace std;
using namespace chrono;
GameNetWork::GameNetWork()
	:m_tcpSocket(nullptr)
{
	//LARGE_INTEGER nFreq;
	//QueryPerformanceFrequency(&nFreq);
	//_animationInterval.QuadPart = (LONGLONG)(1.0 / 60.0 * nFreq.QuadPart);
	
	_animationInterval = 1.0 / 60 * 1000;
}

GameNetWork::~GameNetWork()
{
	if (m_tcpSocket)
	{
		m_tcpSocket->closeSock();
		//m_tcpSocket.reset(nullptr);
	}
}

bool GameNetWork::init()
{
	if (m_tcpSocket == nullptr)
	{
		m_tcpSocket = new TCPSocket();
	}
	return (m_tcpSocket->init() && m_tcpSocket->registerSink(this));
}

void GameNetWork::update()
{
	//if (m_tcpSocket)
	//{
	//	if (m_tcpSocket->isActive())
	//	{
	//		m_tcpSocket->update();
	//	}
	//}
}

void GameNetWork::multiStart()
{
	std::thread t(&GameNetWork::multiEntry, this);
	if (t.joinable())
	{
		t.detach();
	}
}

long long GameNetWork::getCurMillTime()
{
	auto time_now = chrono::system_clock::now();
	auto duration_in_ms = chrono::duration_cast<chrono::milliseconds>(time_now.time_since_epoch());
	return duration_in_ms.count();
}

void GameNetWork::multiEntry()
{
	long long nLast = 0;
	long long nNow = 0;
	nLast = getCurMillTime();	
	while (m_tcpSocket && m_tcpSocket->isActive())
	{
		nNow = getCurMillTime();
		auto duration = nNow - nLast;
		if (duration > _animationInterval)
		{
			nLast = nNow; // -nNow % _animationInterval;
			m_tcpSocket->update();
		}
		else {
			Sleep(1);
		}
	}
	//LARGE_INTEGER nLast;
	//LARGE_INTEGER nNow;

	//QueryPerformanceCounter(&nLast);

	//while (m_tcpSocket && m_tcpSocket->isActive()) {
	//	QueryPerformanceCounter(&nNow);
	//	if (nNow.QuadPart - nLast.QuadPart > _animationInterval.QuadPart && _animationInterval.QuadPart > 0) {
	//		nLast.QuadPart = nNow.QuadPart - (nNow.QuadPart % _animationInterval.QuadPart);
	//		m_tcpSocket->update();
	//	}
	//	else
	//	{
	//		Sleep(1);
	//	}
	//}
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
	GameNetWorkManager::getInstance().OnEventTCPSocketShut(wSocketID, cbShutReason);
	return true;
}

bool GameNetWork::OnEventTCPSocketRead(unsigned short wSocketID, TCP_Command Command, void *pData, unsigned short wDataSize)
{
	GameNetWorkManager::getInstance().houseNetMessage(wSocketID, Command, pData, wDataSize);
	return true;
}
