#include "TCPSocket.h"
#include <memory>
#include <sstream>
#include <assert.h>


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
	cocos2d::Director::getInstance()->getScheduler()->schedule(schedule_selector(TCPSocket::update), this, 1.0f / 60.0f, false);
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
	return true;
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
	TCP_Info *pInfo = (TCP_Info*)pData;
	pInfo->wPacketSize = wDataSize;
	pInfo->cbDataKind = DK_MAPPED;
	//映射数据
	for (size_t i = sizeof(TCP_Head); i < wDataSize ; ++i)
	{
		cbCheckCode += buffer[i];
		buffer[i] = g_SendByteMap[buffer[i]];
	}
	pInfo->cbCheckCode = ~cbCheckCode + 1;
	return true;
}

bool TCPSocket::UnMappedBuffer(void *pData, unsigned short wDataSize)
{
	unsigned char *buffer = (unsigned char *)pData;
	TCP_Info *pInfo = (TCP_Info *)pData;

	if ((pInfo->cbDataKind & DK_MAPPED) != 0)
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
	}
	return true;
}

void TCPSocket::update(float dt)
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
	return 1;
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
		unsigned char cbDataBuffer[SOCKET_TCP_PACKET + sizeof(TCP_Head)];
		TCP_Head *pHead = (TCP_Head*)m_buff4Rcv;
		while (m_recvedSize >= sizeof(TCP_Head))
		{
			wPacketSize = pHead->TCPInfo.wPacketSize;
			assert(pHead->TCPInfo.cbDataKind == SOCKET_TCP_VER);
			assert(wPacketSize <= (SOCKET_TCP_BUFFER));
			if (m_recvedSize < wPacketSize)
			{
				return 1;
			}
			memcpy(cbDataBuffer, m_buff4Rcv, wPacketSize);
			m_recvedSize -= wPacketSize;
			memmove(m_buff4Rcv, m_buff4Rcv + wPacketSize, m_recvedSize);
			unsigned short wRealySize = wPacketSize;

			
		}
	}
	catch (...)
	{
		closeSock();
	}
	return 1;
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
			SendPacket *a = (SendPacket*)data;
			auto b = a->getData();
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
