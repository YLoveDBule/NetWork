#include "TCPSocket.h"
#include <memory>
#include <sstream>
#include <assert.h>
#include "TCPCommon.h"


TCPSocket::TCPSocket()
	:m_sAddress(""),
	m_socket(INVALID_SOCKET),
	m_pITCPSocketSink(nullptr),
	m_socketStatus(SOCKET_STATUS_IDLE),
	m_socketID(0),
	m_singleBuffSendSize(0),
	m_curSendingState(SendState_Sended),
	m_recvedSize(0)
{
	memset(m_singleBuff4Send, 0, SOCKET_TCP_SEND_BUFFER);
	memset(m_buff4Rcv, 0, SOCKET_TCP_RCV_BUFFER);
	memset(m_buff4Send, 0, SOCKET_TCP_SEND_BUFFER);
}

TCPSocket::~TCPSocket()
{
	closeSock();
	std::shared_ptr<SendPacket> sp = m_sendpacketList.try_pop();
	while (sp)
	{
		sp.reset();
		sp = m_sendpacketList.try_pop();
	}

	std::shared_ptr<RecvPacket> sp_recv = m_recvPacketList.try_pop();
	while (sp_recv)
	{
		sp_recv.reset();
		sp_recv = m_recvPacketList.try_pop();
	}
}

bool TCPSocket::init()
{
#if TCP_TARGET_PLATFORM == TCP_PLATFORM_WIN32 
	WSADATA wsaData;
	auto wVersion = MAKEWORD(2, 2);
	auto iErrorCode = WSAStartup(wVersion, &wsaData);
	if (iErrorCode != 0)
	{
		printf("WSAStartup is error");
		return false;
	}
#endif
	//cocos2d::Director::getInstance()->getScheduler()->schedule(schedule_selector(TCPSocket::update), this, 1.0f / 60.0f, false);
	return true;
}

bool TCPSocket::connectServer(const std::string serverIp, const int serverPort)
{
	struct addrinfo *result = nullptr,*ptr = nullptr,hints;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	std::stringstream ss;
	std::string portStr = "";
	ss << serverPort;
	ss >> portStr;

	auto i_result = getaddrinfo(serverIp.c_str(),portStr.c_str(), &hints, &result);
	if (i_result != 0)
	{
		printf("TCPSocket::connect i_result is not zero");
		return false;
	}

	for (ptr = result ; ptr != nullptr ; ptr = result->ai_next)
	{
		m_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
		if (m_socket == INVALID_SOCKET || m_socket < 0 )
		{
			continue;
		}
		if (connect(m_socket, ptr->ai_addr, ptr->ai_addrlen) < 0)
		{
			closeSock();
			m_socket = INVALID_SOCKET;
			continue;
		}
		break;
	}
	if (m_socket == INVALID_SOCKET)
	{
		printf("TCPSocket::connectServer m_socket is null");
	}
	else
	{
#if TCP_TARGET_PLATFORM == TCP_PLATFORM_WIN32 
		unsigned long ul = 1;
		auto nRet = ioctlsocket(m_socket, FIONBIO, &ul);
		if (nRet == SOCKET_ERROR)
		{
			closeSock();
			throw("set nonblock fail");
			return false;
		}
#elif (TCP_TARGET_PLATFORM == TCP_PLATFORM_IOS || \
	  TCP_TARGET_PLATFORM == TCP_PLATFORM_ANDROID || \
	  TCP_TARGET_PLATFORM == TCP_PLATFORM_MAC )
		auto nFlags = fcntl(m_socket, F_GETFL, 0);
		auto nRet = fcntl(m_socket, F_SETFL, nFlags | O_NONBLOCK);
		if (nRet == SOCKET_ERROR)
		{
			closeSock();
			throw("set nonblock fail");
			return false;
		}
#endif
		int nNoDelay = 1;
		if (setsockopt(m_socket,IPPROTO_TCP,TCP_NODELAY,
			(char *)&nNoDelay,sizeof(nNoDelay)) == SOCKET_ERROR)
		{
			throw("setsockopt is error");
			closeSock();
			return false;
		}
#if (TCP_TARGET_PLATFORM == TCP_PLATFORM_IOS || TCP_TARGET_PLATFORM == TCP_PLATFORM_MAC )
		int set = 1;
		if (setsockopt(m_socket,SOL_SOCKET, SO_NOSIGPIPE,(void*)&set,sizeof(int)))
		{
			throw("setsockopt is error");
			closeSock();
			return false;
		}
#endif
	}
	m_socketStatus = SOCKET_STATUS_CONNECT;
	freeaddrinfo(result);
	return true;
}

bool TCPSocket::connectServer(const unsigned long serverIp, const int serverport)
{
	resetData();
	sockaddr_in addr_in;
	memset((void*)&addr_in, 0, sizeof(addr_in));
	addr_in.sin_family = AF_INET;
	return true;
}

bool TCPSocket::dnsParse(std::string serverIp)
{
	//Ipv4
	std::unique_ptr<hostent> hpstr;
	hpstr.reset(gethostbyname(serverIp.c_str()));

	if (hpstr == nullptr)
	{
		printf("dnsParse serverIp is wrong!!!!");
		return false;
	}
	auto str = new(std::nothrow)char[32];
	switch (hpstr->h_addrtype)
	{
	case AF_INET:
	case AF_INET6:
		m_sAddress = inet_ntop(hpstr->h_addrtype, hpstr->h_addr, str, sizeof(str));
	default:
		break;
	}
	return true;
}

void TCPSocket::closeSock()
{
	auto bClose = (m_socket != INVALID_SOCKET);
	m_socketStatus = SOCKET_STATUS_IDLE;
	if (m_socket != INVALID_SOCKET)
	{
#if TCP_TARGET_PLATFORM == TCP_PLATFORM_WIN32 
		closesocket(m_socket);
#else 
		close(m_socket);
#endif 
	}

	if (bClose && (m_pITCPSocketSink != nullptr) && m_socket != INVALID_SOCKET)
	{
		m_pITCPSocketSink->OnEventTCPSocketShut(m_socketID, SHUT_REASON_NORMAL);
		setSocket(INVALID_SOCKET);
	}
	resetData();
}

void TCPSocket::setSocket(SOCKET value)
{
	if (value == INVALID_SOCKET)
	{
		printf("setSocket INVALID_SOCKET");
	}
	m_socket = value;
}

void TCPSocket::resetData()
{
	beginSendMsg();
}

bool TCPSocket::checkrecvfall(const int resultCode)
{
	auto result = false;
	if (m_socket != INVALID_SOCKET && m_socketStatus != SOCKET_STATUS_IDLE)
	{
		if (resultCode == 0)
		{
			if (m_pITCPSocketSink)
			{
				m_pITCPSocketSink->OnEventTCPSocketShut(m_socket, SHUT_REASON_NORMAL);
			}
			result = true;
		}
		else if (resultCode == SOCKET_ERROR || resultCode < 0)
		{
#if TCP_TARGET_PLATFORM == TCP_PLATFORM_WIN32
			auto error = WSAGetLastError();
			if (error != WSAEWOULDBLOCK && error != ERROR_SUCCESS)
			{
				if (m_pITCPSocketSink)
				{
					m_pITCPSocketSink->OnEventTCPSocketShut(m_socket, SHUT_REASON_EXCEPTION);
				}
				result = true;
			}
#else 
			auto error = errno;
			if (error != EAGAIN && error != EINPROGRESS && error != EWOULDBLOCK)
			{
				if (m_pITCPSocketSink)
				{
					m_pITCPSocketSink->OnEventTCPSocketShut(m_socket, SHUT_REASON_EXCEPTION);
				}
				result = true;
			}
#endif
		}
		if (result)
		{
			m_socketStatus = SOCKET_STATUS_IDLE;
		}
	}
	return result;
}

void TCPSocket::beginSendMsg()
{
	memset(m_singleBuff4Send, 0, SOCKET_TCP_SEND_BUFFER);
	m_singleBuffSendSize = 0;
}

void TCPSocket::pushNum(void * data, const unsigned int size)
{
	memcpy(m_singleBuff4Send + m_singleBuffSendSize, data, size);
	m_singleBuffSendSize += size;
}

void TCPSocket::pushLString(const char* data, const int size)
{
	memcpy(m_singleBuff4Send + m_singleBuffSendSize, (void *)data, size);
	m_singleBuffSendSize += size;
}

void TCPSocket::pushLString(const char* data, const short size)
{
	memcpy(m_singleBuff4Send + m_singleBuffSendSize + sizeof(short), (void *)data, size);
	m_singleBuffSendSize += size + sizeof(short);
}

void TCPSocket::endSendMsg()
{
	memset(m_singleBuff4Send, 0, SOCKET_TCP_SEND_BUFFER);
	m_singleBuffSendSize = 0;
}

bool TCPSocket::sendData(unsigned short wMainCmdID, unsigned short wSubCmdID, unsigned int& sendedSize)
{
	
	sendedSize = m_singleBuffSendSize;
	auto result = sendData(wMainCmdID, wSubCmdID, m_singleBuff4Send, m_singleBuffSendSize);
	endSendMsg();
	return result;

}

bool TCPSocket::sendData(unsigned short wMainCmdID, unsigned short wSubCmdID, void * pData, unsigned short wDataSize)
{
	TCP_Buffer tcpBuffer;
	memset(&tcpBuffer, 0, sizeof(TCP_Buffer));
	tcpBuffer.Head.CommandInfo.wMainCmdID = wMainCmdID;
	tcpBuffer.Head.CommandInfo.wSubCmdID = wSubCmdID;
	memcpy(tcpBuffer.cbBuffer, pData, wDataSize);
	unsigned short size = wDataSize + sizeof(TCP_Head);
	MappedBuffer(&tcpBuffer, size);
	SendPacket *pSendData = new(std::nothrow) SendPacket((char*)(&tcpBuffer), wMainCmdID, wSubCmdID, size);
	m_sendpacketList.push(*pSendData);
	return true;
}

bool TCPSocket::sendData()
{
	return true;
}

bool TCPSocket::MappedBuffer(void *pData, unsigned short wDataSize)
{
	//变量定义
	unsigned char *buffer = (unsigned char*)pData;
	unsigned char cbCheckCode = 0;
	//设置数据
	TCP_Head *pInfo = (TCP_Head*)pData;
	pInfo->TCPInfo.wPacketSize = wDataSize;
	pInfo->TCPInfo.cbDataKind = DK_MAPPED;
	//映射数据
	for (size_t i = sizeof(TCP_Info); i < wDataSize ; ++i)
	{
		cbCheckCode += buffer[i];
		buffer[i] = g_SendByteMap[buffer[i]];
	}
	pInfo->TCPInfo.cbCheckCode = ~cbCheckCode + 1;
	return true;
}

bool TCPSocket::UnMappedBuffer(void *pData, unsigned short wDataSize)
{
	unsigned char *buffer = (unsigned char *)pData;
	assert(wDataSize >= sizeof(TCP_Head));
	assert(((TCP_Head *)pData)->TCPInfo.wPacketSize == wDataSize);
	TCP_Head *pInfo = (TCP_Head *)pData;
	if (pInfo->TCPInfo.cbDataKind == DK_MAPPED)
	{
		for (size_t i = sizeof(TCP_Info); i < wDataSize; ++i)
		{
			buffer[i] = g_RecvByteMap[buffer[i]];
		}
	}

	/*if ((pInfo->cbDataKind & DK_MAPPED) != 0)
	{
		unsigned short cbCheckCode = pInfo->cbCheckCode;
		for (size_t i = sizeof(TCP_Info); i < wDataSize ; ++i)
		{
			cbCheckCode += g_RecvByteMap[buffer[i]];
			buffer[i] = g_RecvByteMap[buffer[i]];
		}

		if (cbCheckCode != 0)
		{
			return false;
		}
	}*/
	return true;
}

void TCPSocket::update()
{
	if (m_socket != SOCKET_ERROR)
	{
		getData();
		if (!m_recvPacketList.empty() && m_pITCPSocketSink)
		{
			if (std::shared_ptr<RecvPacket> str = m_recvPacketList.try_pop())
			{
				m_pITCPSocketSink->OnEventTCPSocketRead(str->SID, str->Comd, str->Data, str->Size);
				str.reset();
			}
		}
		if (!m_sendpacketList.empty() && m_curSendingState != SendState_Sending)
		{
			if (m_curSendingState != SendState_Sending)
			{
				m_curSendingState = SendState_Sending;
				std::shared_ptr<SendPacket> sp = m_sendpacketList.try_pop();
				if (sp)
				{
					m_curSendingState = sendDataBuffer(sp->getData(), sp->getSize());
					sp.reset();
				}
			}
		}
	}
}

int TCPSocket::getData()
{
	try
	{
		auto iresult = recv(m_socket, (char *)m_buff4Rcv + m_recvedSize, sizeof(m_buff4Rcv) - m_recvedSize, 0);
		if (checkrecvfall(iresult))
		{
			closeSock();
			return 0;
		}
		if (iresult < 0)
		{
			return 0;
		}
		m_recvedSize += iresult;
		unsigned short wPacketSize = 0;
		unsigned char cbDataBuffer[SOCKET_TCP_BUFFER];
		TCP_Head *pHead = (TCP_Head*)m_buff4Rcv;
		while (m_recvedSize >= sizeof(TCP_Head))
		{
			wPacketSize = pHead->TCPInfo.wPacketSize;
			assert(pHead->TCPInfo.cbDataKind == DK_MAPPED);
			assert(wPacketSize <= (SOCKET_TCP_BUFFER));
			if (m_recvedSize < wPacketSize)
			{
				return 1;
			}
			memcpy(cbDataBuffer, m_buff4Rcv, wPacketSize);
			m_recvedSize -= wPacketSize;
			memmove(m_buff4Rcv, m_buff4Rcv + wPacketSize, m_recvedSize);
			UnMappedBuffer(&cbDataBuffer, wPacketSize);
			unsigned short wRealySize = wPacketSize;
			TCP_Head *tcpHead = (TCP_Head *)cbDataBuffer;
			void *pDataBuffer = cbDataBuffer + sizeof(TCP_Head);
			unsigned short wDataSize = wRealySize - sizeof(TCP_Head);
			if (tcpHead->CommandInfo.wMainCmdID == MDM_KN_COMMAND)
			{
				switch (tcpHead->CommandInfo.wSubCmdID)
				{
				case SUB_KN_DETECT_SOCKET: //网络检测
				{
					//send data 
					sendData(MDM_KN_COMMAND, SUB_KN_DETECT_SOCKET, pDataBuffer, wDataSize);
					printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$send heart break$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");
					printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$>>>>>>> send heart break <<<<<$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");
					printf("$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$send heart break$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$");
					break;
				}
				default:
					break;
				}
				continue;
			}
			//处理数据
			//bool bSuccess = m_pITCPSocketSink->OnEventTCPSocketRead(m_socket, tcpHead->CommandInfo, pDataBuffer, wDataSize);
			//if (bSuccess == false )
			//{
			//	throw TEXT("网络数据包处理失败");
			//}

			RecvPacket rp;
			rp.SID = m_socket;
			rp.Comd = tcpHead->CommandInfo;
			memcpy(rp.Data, pDataBuffer, wDataSize);
			rp.Size = wDataSize;
			m_recvPacketList.push(rp);

		}
	}
	catch (...)
	{
		closeSock();
	}
	return 1;
}


bool TCPSocket::registerSink(ITCPSocketSink * value)
{
	m_pITCPSocketSink = value;
	return true;
}

int TCPSocket::isConnected()
{
	if (m_socket == INVALID_SOCKET)
	{
		return CONNECT_FAILURE;
	}
	fd_set set;
	struct timeval tv;
	//将set清零使得集合中不含任何套接字
	FD_ZERO(&set);
	//将套接字加入set集合
	FD_SET(m_socket, &set);
	//设置时间 0 0  非阻塞
	tv.tv_sec = 0;
	tv.tv_usec = 0;
	if (select((int)(m_socket), NULL, &set, NULL, &tv) > 0)
	{
		//测试 套接字是否在set中
		if (FD_ISSET(m_socket,&set))
		{
#if TCP_TARGET_PLATFORM == TCP_PLATFORM_WIN32 
			return CONNECT_FAILURE;
#elif TCP_TARGET_PLATFORM == TCP_PLATFORM_IOS || TCP_TARGET_PLATFORM == TCP_PLATFORM_ANDROID 
			int error;
			socklen_t len = sizeof(error);
			if (getsockopt(m_socket,SOL_SOCKET,SO_ERROR,&error,&len) < 0)
			{
				return CONNECT_FAILURE;
			}
#endif
		}
		else
		{
			return CONNECT_FAILURE;
		}
	}
	else
	{
		return CONNECT_FAILURE;
	}
	return CONNECT_INPROGRESS;
}

bool TCPSocket::hasError()
{
#if TCP_TARGET_PLATFORM == TCP_PLATFORM_WIN32
	int err = WSAGetLastError();
	if (err != WSAEWOULDBLOCK && err != ERROR_SUCCESS)
	{
		return true;
	}
#else 
	int err = errno;
	if (err != EINPROGRESS && err != EAGAIN)
	{
		return true;
	}
#endif
	return false;
}

SOCKET TCPSocket::getSocket()
{
	return m_socket;
}

bool TCPSocket::isActive()
{
	return (m_socket != INVALID_SOCKET);
}

SendState TCPSocket::sendDataBuffer(void *data, size_t size)
{
	assert(size != 0);
	assert(data != nullptr);
	//UnMappedBuffer(data, size); 

	if (m_socketStatus != SOCKET_STATUS_IDLE  && size > 0)
	{
		size_t sended = 0;

		while (sended < size)
		{
#if (TCP_TARGET_PLATFORM == TCP_PLATFORM_WIN32 || \
	TCP_TARGET_PLATFORM == TCP_PLATFORM_ANDROID)
			auto result = send(m_socket, (char*)data + sended,
				size - sended, 0);
#endif

#if (TCP_TARGET_PLATFORM == TCP_PLATFORM_IOS|| \
	TCP_TARGET_PLATFORM == TCP_PLATFORM_MAC)
			auto result = send(m_socket, (char *)data + sended, size - sended, SO_NOSIGPIPE);
#endif
			if (result == SOCKET_ERROR) {
#if(TCP_TARGET_PLATFORM == TCP_PLATFORM_WIN32)
				int err = WSAGetLastError();
				if (err == WSAEWOULDBLOCK || err == ERROR_SUCCESS) {
					continue;
				}
#endif

#if( TCP_TARGET_PLATFORM == TCP_PLATFORM_IOS || \
	TCP_TARGET_PLATFORM == TCP_PLATFORM_MAC || \
	TCP_TARGET_PLATFORM == TCP_PLATFORM_ANDROID )
				int err = errno;
				if (err == EAGAIN || err == EWOULDBLOCK) {
					continue;
				}
#endif
				if (m_pITCPSocketSink)
				{
					m_pITCPSocketSink->OnEventTCPSocketShut(m_socket, SHUT_REASON_EXCEPTION);
				}

				closeSock();

				return SendState_Failed;
			}
			sended += result;
		}
		return SendState_Sended;
	}
	return SendState_Failed;
}
