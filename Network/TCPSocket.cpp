#include <assert.h>

#include <stdio.h>
#include <string.h>
#include <sstream>

#include <thread>
#include <functional>

#include "zlib.h"
#include "TCPSocket.h"



bool TCPSocket::HasWSAStartup = false;

bool 
TCPSocket::registerSink(ITCPSocketSink* value)
{
	m_pITCPSocketSink = value;

	return true;
}

TCPSocket::TCPSocket() : SocketInstance(INVALID_SOCKET),
SocketStatus(SOCKET_STATUS_IDLE),
RecvedSize(0),
SingleBuffSendSize(0),
m_pITCPSocketSink(nullptr),
IsSending(false),
CurSendingState(SendState_Sended)
{
	resetData();

	enc.setEncMethod(CEncryptDes::ENCDEC_DES);
	BYTE key[8] = { 'z','x',66,45,'c','q','a','w' };
	enc.set_key_des((const_ZES_cblock*)key);

	memset(Buff4Rcv, 0, sizeof(SOCKET_TCP_RCV_BUFFER));
	memset(Buff4Send, 0, sizeof(SOCKET_TCP_SEND_BUFFER));
}

TCPSocket::~TCPSocket()
{
	closeSock();

	SendPacketList.clear();
}

bool 
TCPSocket::init()
{
#if CJ_TARGET_PLATFORM == CJ_PLATFORM_WIN32
	if (!HasWSAStartup) {
		WSADATA WSAData;
		WORD wVersionRequested = MAKEWORD(2, 2);
		int iErrorCode = WSAStartup(wVersionRequested, &WSAData);
		if (iErrorCode != 0) return false;
		HasWSAStartup = true;
	}
#endif

	return true;

	/////---------------------------------------------
	if (SocketInstance != INVALID_SOCKET) throw ("连接 SOCKET 句柄已经存在");

	if (SocketStatus != SOCKET_STATUS_IDLE) throw ("socket status not idle!");

#if CJ_TARGET_PLATFORM == CJ_PLATFORM_WIN32
	if (!HasWSAStartup) {
		WSADATA WSAData;
		WORD wVersionRequested = MAKEWORD(2, 2);
		int iErrorCode = WSAStartup(wVersionRequested, &WSAData);
		if (iErrorCode != 0) return false;
		HasWSAStartup = true;
	}
	//SocketInstance = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
#else
	//SocketInstance = socket(AF_INET, SOCK_STREAM, 0);
#endif

	SocketInstance = socket(AF_INET, SOCK_STREAM, 0); //IPPROTO_TCP

#if CJ_TARGET_PLATFORM == CJ_PLATFORM_WIN32
	if (SocketInstance == INVALID_SOCKET)
#else
	if (SocketInstance < 0)
#endif
	{
		perror("ERROR opening socket");
		closeSock();
		return false;
	}

	// set non-blocking 
#if( CJ_TARGET_PLATFORM == CJ_PLATFORM_WIN32 )
	unsigned long ul = 1;
	int nRet = ioctlsocket(SocketInstance, FIONBIO, (unsigned long*)&ul);
	if (nRet == SOCKET_ERROR) {
		closeSock();
		throw ("set nonblock fail");
		return false;
	}
#elif( CJ_TARGET_PLATFORM == CJ_PLATFORM_IOS || \
	CJ_TARGET_PLATFORM == CJ_PLATFORM_ANDROID || \
	CJ_TARGET_PLATFORM == CJ_PLATFORM_MAC)
	int nFlags = fcntl(SocketInstance, F_GETFL, 0);
	int nRet = fcntl(SocketInstance, F_SETFL, nFlags | O_NONBLOCK);
	if (nRet == SOCKET_ERROR) {
		closeSock();
		throw ("set nonblock fail");
		return false;
	}
#endif

	int nNoDelay = 1;
	if (setsockopt(SocketInstance, IPPROTO_TCP, TCP_NODELAY,
		(char*)&nNoDelay, sizeof(nNoDelay)) == SOCKET_ERROR) {
		throw ("set no delay fail");
		closeSock();
		return false;
	}

#if CJ_TARGET_PLATFORM == CJ_PLATFORM_MAC || \
	CJ_TARGET_PLATFORM == CJ_PLATFORM_IOS
	int set = 1;
	if (setsockopt(SocketInstance,
		SOL_SOCKET,
		SO_NOSIGPIPE,
		(void *)&set,
		sizeof(int)))
	{
		throw ("set SO_NOSIGPIPE fail");
		closeSock();
		return false;
	}
#endif

	return true;
}

bool 
TCPSocket::connect(unsigned long ip, int port)
{
	//if (ip == INADDR_NONE) 
	//throw ("目标服务器地址格式不正确，请检查后再次尝试！");

	resetData();

	sockaddr_in addr_in;
	bzero((void*)&addr_in, sizeof(addr_in));
	addr_in.sin_family = AF_INET;
	addr_in.sin_port = htons(port);
	addr_in.sin_addr.s_addr = ip;

	int iErrorCode = 0;

#if CJ_TARGET_PLATFORM == CJ_PLATFORM_WIN32
	WSASetLastError(0);
	iErrorCode = ::connect(SocketInstance,
		(SOCKADDR*)&addr_in,
		sizeof(addr_in));
#else
	iErrorCode = ::connect(SocketInstance,
		(struct sockaddr *)&addr_in,
		sizeof(addr_in));
#endif

#if CJ_TARGET_PLATFORM == CJ_PLATFORM_WIN32
	if (iErrorCode == SOCKET_ERROR)
#else
	if (iErrorCode < 0)
#endif
	{
		if (hasError()) {
			closeSock();
			return false;
		}
	}

	int err = errno;

	SocketStatus = SOCKET_STATUS_CONNECT; // SOCKET_STATUS_WAIT;

	return true;
}

bool 
TCPSocket::connect(const char* serverip, int port)
{
	if (true) {
		bool conresult = false;
		struct addrinfo *result = NULL,
			*ptr = NULL,
			hints;

		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_UNSPEC;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;

		std::string portstr;
		std::stringstream ss;
		ss << port;
		ss >> portstr;

		int iresult = getaddrinfo(serverip, portstr.c_str(), &hints, &result);

		if (iresult != 0) {
			// raise error -> iresult
			return conresult;
		}

		for (ptr = result; ptr != NULL; ptr = result->ai_next) {
			SocketInstance = ::socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
			if (SocketInstance == INVALID_SOCKET || SocketInstance < 0) {
				//printf("socket failed with error: %ld\n", WSAGetLastError());
				continue;
			}

			if (::connect(SocketInstance, ptr->ai_addr, ptr->ai_addrlen) < 0) {
				closeSock();
				SocketInstance = INVALID_SOCKET;
				continue;
			}

			break;
		}

		if (SocketInstance == INVALID_SOCKET) {
			//err(1, "%s", cause);
		}
		else {
			conresult = true;

			// set non-blocking 
#if( CJ_TARGET_PLATFORM == CJ_PLATFORM_WIN32 )
			unsigned long ul = 1;
			int nRet = ioctlsocket(SocketInstance, FIONBIO, (unsigned long*)&ul);
			if (nRet == SOCKET_ERROR) {
				closeSock();
				throw ("set nonblock fail");
				return false;
			}
#elif( CJ_TARGET_PLATFORM == CJ_PLATFORM_IOS || \
	CJ_TARGET_PLATFORM == CJ_PLATFORM_ANDROID || \
	CJ_TARGET_PLATFORM == CJ_PLATFORM_MAC)
			int nFlags = fcntl(SocketInstance, F_GETFL, 0);
			int nRet = fcntl(SocketInstance, F_SETFL, nFlags | O_NONBLOCK);
			if (nRet == SOCKET_ERROR) {
				closeSock();
				throw ("set nonblock fail");
				return false;
			}
#endif

			int nNoDelay = 1;
			if (setsockopt(SocketInstance, IPPROTO_TCP, TCP_NODELAY,
				(char*)&nNoDelay, sizeof(nNoDelay)) == SOCKET_ERROR) {
				throw ("set no delay fail");
				closeSock();
				return false;
			}

#if CJ_TARGET_PLATFORM == CJ_PLATFORM_MAC || \
	CJ_TARGET_PLATFORM == CJ_PLATFORM_IOS
			int set = 1;
			if (setsockopt(SocketInstance,
				SOL_SOCKET,
				SO_NOSIGPIPE,
				(void *)&set,
				sizeof(int)))
			{
				throw ("set SO_NOSIGPIPE fail");
				closeSock();
				return false;
			}
#endif
		}

		freeaddrinfo(result);

		return conresult;
	}
	///-----------------------------------------------------------------------
	//
	//unsigned char buff[sizeof(in6_addr)] = {0};
	//inet_pton(AF_INET6, serverip, buff);

	unsigned long serveraddr = inet_addr(serverip);
	if (serveraddr == INADDR_NONE) {
		closeSock();
		return false;
	}

	return connect(serveraddr, port);
}

void 
TCPSocket::closeSock()
{
	bool bClose = (SocketInstance != INVALID_SOCKET);
	SocketStatus = SOCKET_STATUS_IDLE;
	if (SocketInstance != INVALID_SOCKET) {
#if CJ_TARGET_PLATFORM == CJ_PLATFORM_WIN32
		closesocket(SocketInstance);
#else
		close(SocketInstance);
#endif
		setSocket(INVALID_SOCKET);
		SocketStatus = SOCKET_STATUS_IDLE;
	}
	if ((bClose) && (m_pITCPSocketSink != nullptr)) {
		assert(m_pITCPSocketSink != nullptr);
		try {
			m_pITCPSocketSink->OnEventTCPSocketShut(SockID,
				SHUT_REASON_NORMAL);
		}
		catch (...) {}
	}

	resetData();
}

void 
TCPSocket::beginSendMsg()
{
	memset(SingleBuff4Send, 0, SOCKET_TCP_SEND_BUFFER);
	SingleBuffSendSize = 0;
}

void 
TCPSocket::pushNum(void* data, NumbericSize size)
{
	memcpy(SingleBuff4Send + SingleBuffSendSize, data, size);
	SingleBuffSendSize += size;
}

void 
TCPSocket::pushLString(char* data, int size)
{
	memcpy(SingleBuff4Send + SingleBuffSendSize, (void*)data, size);
	SingleBuffSendSize += size;
}

void 
TCPSocket::pushLString(char* data, short size)
{
	short* len = (short*)(SingleBuff4Send + SingleBuffSendSize);
	*len = size;
	memcpy(SingleBuff4Send + SingleBuffSendSize + sizeof(short),
		(void*)data, size);
	SingleBuffSendSize += size + sizeof(short);
}

void 
TCPSocket::endSendMsg()
{
	memset(SingleBuff4Send, 0, SOCKET_TCP_SEND_BUFFER); 
	// SingleBuff4Send + SingleBuffSendSize
	SingleBuffSendSize = 0;
}

bool
TCPSocket::sendMessage(unsigned short wMainCmdID,
	unsigned short wSubCmdID)
{
	return sendMessage(wMainCmdID, 
		wSubCmdID, 
		SingleBuff4Send, 
		SingleBuffSendSize);
}

bool 
TCPSocket::sendMessage(unsigned short wMainCmdID,
	unsigned short wSubCmdID,
	void *pData,
	unsigned short wDataSize)
{
	SendPacket sp;

	bool temp_addencyption = false;

	//BYTE cbDataBuffer[SOCKET_TCP_BUFFER];
	TCP_Head * pHead = (TCP_Head *)sp.cbDataBuffer; // cbDataBuffer; //
	pHead->CommandInfo.wMainCmdID = wMainCmdID;
	pHead->CommandInfo.wSubCmdID = wSubCmdID;
	if (wDataSize) {
		assert(SingleBuff4Send != nullptr);
		BYTE bBuffer[SOCKET_TCP_BUFFER] = { 0 };
		unsigned long lResultSize = SOCKET_TCP_BUFFER - 1;
		wDataSize = CompressData((unsigned char*)pData,
			wDataSize,
			bBuffer,
			lResultSize);
		memcpy(pHead + 1, bBuffer, wDataSize);
	}

	pHead->TCPInfo.cbCheckCode = 1;
	pHead->TCPInfo.wPacketSize = sizeof(TCP_Head) + wDataSize;
	pHead->TCPInfo.cbVersion = SOCKET_TCP_VER;

	unsigned short wSendSize = sizeof(TCP_Head) + wDataSize;

	// with encryption added
	if (temp_addencyption)
	{
		 /*EncryptBuffer(cbDataBuffer, 
		sizeof(TCP_Head) + SingleBuffSendSize, 
		sizeof(cbDataBuffer));*/

		unsigned short wSendSize = EncryptBuffer(sp.cbDataBuffer, 
		sizeof(TCP_Head) + SingleBuffSendSize, 
		sizeof(sp.cbDataBuffer));
	}

	sp.Size = wSendSize;
	SendPacketList.push_back(sp);

	return true;
}

unsigned short 
TCPSocket::EncryptBuffer(unsigned char pcbDataBuffer[],
	unsigned short wDataSize,
	unsigned short wBufferSize)
{
	assert(wDataSize >= sizeof(TCP_Head));
	assert(wDataSize <= (sizeof(TCP_Head) + SOCKET_TCP_BUFFER));
	assert(wBufferSize >= (wDataSize + 2 * sizeof(unsigned short)));

	unsigned short wEncryptSize = wDataSize - sizeof(TCP_Info);
	unsigned char mod = (unsigned char)((wEncryptSize) % 8);
	if (0 != mod) {
		mod = 8 - mod;
		memset(pcbDataBuffer + wDataSize, 0, mod);
	}

	TCP_Head * pHead = (TCP_Head *)pcbDataBuffer;
	pHead->TCPInfo.cbCheckCode = 1;
	//pHead->TCPInfo.cbMod = mod;
	pHead->TCPInfo.wPacketSize = wDataSize + mod;
	pHead->TCPInfo.cbVersion = SOCKET_TCP_VER;

	enc.encdec(pcbDataBuffer + sizeof(TCP_Info), wEncryptSize + mod, true);

	return wDataSize + mod;
}

unsigned short
TCPSocket::CrevasseBuffer(unsigned char pcbDataBuffer[], unsigned short wDataSize)
{
	assert(wDataSize >= sizeof(TCP_Head));
	assert(((TCP_Head *)pcbDataBuffer)->TCPInfo.wPacketSize == wDataSize);

	TCP_Head * pHead = (TCP_Head *)pcbDataBuffer;

	enc.encdec(pcbDataBuffer + sizeof(TCP_Info),
		pHead->TCPInfo.wPacketSize - sizeof(TCP_Info),
		false);

	//wDataSize = pHead->TCPInfo.wPacketSize - pHead->TCPInfo.cbMod;
	return wDataSize;
}

bool 
TCPSocket::sendMessage()
{
	return sendDataBuffer((char*)SingleBuff4Send, SingleBuffSendSize) 
			== SendState_Sended;
}

SendState 
TCPSocket::sendDataBuffer(void *data, size_t size)
{
	assert(size != 0);
	assert(data != nullptr);

	if (size > 0) {
		size_t sended = 0;
		while (sended < size) {
#if( CJ_TARGET_PLATFORM == CJ_PLATFORM_WIN32 || \
	CJ_TARGET_PLATFORM == CJ_PLATFORM_ANDROID )
			int result = ::send(SocketInstance,
				(char*)data + sended,
				size - sended,
				0);
#endif

#if( CJ_TARGET_PLATFORM == CJ_PLATFORM_IOS || \
	CJ_TARGET_PLATFORM == CJ_PLATFORM_MAC)
			int result = ::send(SocketInstance,
				(char*)data + sended,
				size - sended,
				SO_NOSIGPIPE);
#endif
			if (result == SOCKET_ERROR) {
				if (hasError()) {
					hasError();
					int err = errno;
					return SendState_Failed;
				}
				/*if (hasError()) {
					hasError();
					int err = errno;
					return SendState_Failed;
				}
				else {
					continue;
				}*/
				int err = errno;
				continue;
			}
			sended += result;
		}

		return SendState_Sended;
	}

	return SendState_Failed;
}

int 
TCPSocket::recv()
{
	try {
		int iresult = ::recv(SocketInstance,
			(char*)Buff4Rcv + RecvedSize,
			sizeof(Buff4Rcv) - RecvedSize,
			0);

		if (checkrecvfall(iresult)) {
			//throw ("网络连接关闭，读取数据失败");
			//printf("%s", "网络连接关闭，读取数据失败");
			closeSock();

			return 0;
		}
		
		if (iresult < 0) {
			return 0;
		}

		RecvedSize += iresult;

		WORD wPacketSize = 0;
		BYTE cbDataBuffer[SOCKET_TCP_PACKET + sizeof(TCP_Head)];
		TCP_Head * pHead = (TCP_Head *)Buff4Rcv;

		while (RecvedSize >= sizeof(TCP_Head)) {
			wPacketSize = pHead->TCPInfo.wPacketSize;
			assert(pHead->TCPInfo.cbVersion == SOCKET_TCP_VER);
			assert(wPacketSize <= (SOCKET_TCP_BUFFER));

			if (pHead->TCPInfo.cbVersion != SOCKET_TCP_VER) 
			throw ("network package version not valid");
			if (wPacketSize > (SOCKET_TCP_PACKET + sizeof(TCP_Head))) 
			throw ("package size too huge!");
			if (RecvedSize < wPacketSize) return 1;

			memcpy(cbDataBuffer, Buff4Rcv, wPacketSize);
			RecvedSize -= wPacketSize;
			memmove(Buff4Rcv, Buff4Rcv + wPacketSize, RecvedSize);

			WORD wRealySize = wPacketSize;
			// CrevasseBuffer(cbDataBuffer, wPacketSize); //wPacketSize; // 
			assert(wRealySize >= sizeof(TCP_Head));

			//解释数据
			WORD wDataSize = wRealySize - sizeof(TCP_Head);
			void * pDataBuffer = cbDataBuffer + sizeof(TCP_Head);
			TCP_Command Command = ((TCP_Head *)cbDataBuffer)->CommandInfo;

			if (wDataSize > 0) {
				BYTE bBuffer[SOCKET_TCP_BUFFER] = { 0 };
				unsigned long lResultSize = SOCKET_TCP_BUFFER - 1;
				wDataSize = UnCompressData((unsigned char*)pDataBuffer,
					wDataSize,
					bBuffer,
					lResultSize);
				memcpy(pDataBuffer, bBuffer, wDataSize);
			}
			else {
				pDataBuffer = nullptr;
			}

			//内核命令
			if (Command.wMainCmdID == MDM_KN_COMMAND) {
				switch (Command.wSubCmdID) {
				case SUB_KN_DETECT_SOCKET: { //网络检测
					//发送数据
					beginSendMsg();

					sendMessage(MDM_KN_COMMAND, SUB_KN_DETECT_SOCKET);
					break;
				}
				}
				continue;
			}

			//处理数据
			if (m_pITCPSocketSink) {
				bool bSuccess = m_pITCPSocketSink->OnEventTCPSocketRead(
					SocketInstance,
					Command,
					pDataBuffer,
					wDataSize);
				//if (bSuccess == false) throw ("网络数据包处理失败");
			}
		}
	}
	catch (...) {
		closeSock();
	}

	return 1;
}

void 
TCPSocket::update()
{
	//if (!SendPacketList.empty()) {
	//	if (CurSendingState == SendState_Sended ||
	//		CurSendingState == SendState_Failed) {
	//		std::vector<SendPacket>::iterator itor = SendPacketList.begin();
	//		if (sendDataBuffer(itor->cbDataBuffer, itor->Size)) {
	//			SendPacketList.erase(itor);
	//		}
	//	}
	//}

	//recv();
}

void 
TCPSocket::multistart()
{
	std::thread t(&TCPSocket::multiupdate, this);
	t.detach();
}

void 
TCPSocket::multiupdate()
{
	while (isActive()) {
		if (!SendPacketList.empty()) {
			for (std::vector<SendPacket*>::size_type n = 0;
				n < SendPacketList.size();) {
				if (n < SendPacketList.size())
					if (SendState_Sended == sendDataBuffer(
						SendPacketList[n].cbDataBuffer,
						SendPacketList[n].Size)) {
						SendPacketList.erase(SendPacketList.begin() + n);
					}
					else {
						n++;
					}
			}
		}

		recv();
	}
}

inline unsigned short
TCPSocket::SeedRandMap(unsigned short wSeed)
{
	unsigned long dwHold = wSeed;
	return (unsigned short)((dwHold = dwHold * 241103L + 2533101L) >> 16);
}

inline unsigned char
TCPSocket::MapSendByte(unsigned char cbData)
{
	unsigned char cbMap = g_SendByteMap[(unsigned char)(cbData + m_cbSendRound)];
	m_cbSendRound += 3;
	return cbMap;
}

inline unsigned char
TCPSocket::MapRecvByte(unsigned char cbData)
{
	unsigned char cbMap = g_RecvByteMap[cbData] - m_cbRecvRound;
	m_cbRecvRound += 3;
	return cbMap;
}

bool 
TCPSocket::checkSocketStatus()
{
	bool result = CONNECT_SUCCESS == isConnected();
	bool haserr = hasError();

	//result &= SocketInstance != INVALID_SOCKET &&
	//	SocketStatus == SOCKET_STATUS_CONNECT && !haserr;

	result = SocketInstance != INVALID_SOCKET &&
		SocketStatus == SOCKET_STATUS_CONNECT;

	if (!result) {
	}
	else {
	}

	return result;
}

bool 
TCPSocket::checkrecvfall(int rescode)
{
	bool result = false;

	if (rescode == 0) {
		if (m_pITCPSocketSink) m_pITCPSocketSink->OnEventTCPSocketShut(SocketInstance, SHUT_REASON_NORMAL);

		result = true;
	}
	else if (rescode == SOCKET_ERROR || rescode < 0) {
#if CJ_TARGET_PLATFORM == CJ_PLATFORM_WIN32
		int err = WSAGetLastError();
		if (err != WSAEWOULDBLOCK && err != ERROR_SUCCESS)
#else
		int err = errno;
		if (err != EAGAIN && err != EINPROGRESS && err != EWOULDBLOCK)
#endif
		{
			if (m_pITCPSocketSink) m_pITCPSocketSink->OnEventTCPSocketShut(SocketInstance, SHUT_REASON_EXCEPTION);

			result = true;
		}
	}

	if (result) {
		SocketStatus = SOCKET_STATUS_IDLE;
	}

	// it's ok though
	return result;
}

bool 
TCPSocket::sendMsg(unsigned short wMainCmdID, unsigned short wSubCmdID)
{
	BYTE cbDataBuffer[SOCKET_TCP_BUFFER];
	TCP_Head * pHead = (TCP_Head *)cbDataBuffer;
	pHead->CommandInfo.wMainCmdID = wMainCmdID;
	pHead->CommandInfo.wSubCmdID = wSubCmdID;

	WORD wSendSize = sizeof(TCP_Head);
	// EncryptBuffer(cbDataBuffer, sizeof(TCP_Head), sizeof(cbDataBuffer));

	return sendDataBuffer(cbDataBuffer, wSendSize) == SendState_Sended;
}

unsigned long 
TCPSocket::CompressData(unsigned char* pcbSourceData,
	unsigned long lSourceSize,
	unsigned char cbResultData[],
	unsigned long lResultSize)
{
	return compress(cbResultData,
		&lResultSize,
		pcbSourceData,
		lSourceSize) == 0L ? lResultSize  : 0L;
}

unsigned long 
TCPSocket::UnCompressData(unsigned char* pcbSourceData,
	unsigned long lSourceSize,
	unsigned char cbResultData[],
	unsigned long lResultSize)
{
	return uncompress(cbResultData,
		&lResultSize,
		pcbSourceData,
		lSourceSize) == 0L ? lResultSize : 0L;
}

bool 
TCPSocket::sendMsg(unsigned short wMainCmdID,
	unsigned short wSubCmdID,
	void * pData,
	unsigned short wDataSize)
{
	assert(wDataSize <= SOCKET_TCP_PACKET);
	if (wDataSize > SOCKET_TCP_PACKET) return false;

	BYTE cbDataBuffer[SOCKET_TCP_BUFFER];
	TCP_Head * pHead = (TCP_Head *)cbDataBuffer;
	pHead->CommandInfo.wMainCmdID = wMainCmdID;
	pHead->CommandInfo.wSubCmdID = wSubCmdID;
	if (wDataSize > 0) {
		assert(pData != nullptr);
		//memcpy(pHead + 1, pData, wDataSize);
		//进行压缩处理
		BYTE bBuffer[SOCKET_TCP_BUFFER] = { 0 };
		unsigned long lResultSize = SOCKET_TCP_BUFFER - 1;
		wDataSize = CompressData((unsigned char*)pData,
			wDataSize,
			bBuffer,
			lResultSize);
		memcpy(pHead + 1, bBuffer, wDataSize);
	}

	unsigned short wSendSize = EncryptBuffer(cbDataBuffer,
		sizeof(TCP_Head) + wDataSize,
		sizeof(cbDataBuffer));

	return sendDataBuffer(cbDataBuffer, wSendSize) == SendState_Sended;
}

int 
TCPSocket::sendSth(void *pdata, int size)
{
	int iSendResult = ::send(SocketInstance, (char*)pdata, size, 0);

#if CJ_TARGET_PLATFORM == CJ_PLATFORM_WIN32
	if (iSendResult == SOCKET_ERROR) {
		printf("send failed with error: %d\n", WSAGetLastError());
		closesocket(SocketInstance);
		WSACleanup();
		return 1;
	}
#endif

	printf("Bytes sent: %d\n", iSendResult);

	return 0;
}

int 
TCPSocket::isConnected()
{
	if (SocketInstance == INVALID_SOCKET) {
		return CONNECT_FAILURE;
	}

	fd_set	fd;
	struct timeval tv;

	FD_ZERO(&fd);
	FD_SET(SocketInstance, &fd);

	tv.tv_sec = 0;
	tv.tv_usec = 0;

	if (select((int)(SocketInstance), NULL, &fd, NULL, &tv) > 0) { // SocketInstance + 1
		if (FD_ISSET(SocketInstance, &fd)) {
#if( CJ_TARGET_PLATFORM == CJ_PLATFORM_WIN32 )

			return CONNECT_SUCCESS;

#elif( CJ_TARGET_PLATFORM == CJ_PLATFORM_IOS || \
	CJ_TARGET_PLATFORM == CJ_PLATFORM_ANDROID )

			int nError;
			socklen_t len = sizeof(nError);
			if (getsockopt(SocketInstance,
				SOL_SOCKET,
				SO_ERROR,
				&nError,
				&len) < 0) {
				return CONNECT_FAILURE;
			}
			if (nError > 0 ) { // == ECONNREFUSED || nError == ETIMEDOUT) {
				//closeSock();
				return CONNECT_FAILURE;
			}
			return CONNECT_SUCCESS;

#endif
		}
		else {
			return CONNECT_FAILURE;
		}
	}
	else {
		//closeSock();
		return CONNECT_FAILURE;
	}

	return CONNECT_INPROGRESS;
}

bool 
TCPSocket::hasError()
{
#if CJ_TARGET_PLATFORM == CJ_PLATFORM_WIN32
	int err = WSAGetLastError();
	if (err != WSAEWOULDBLOCK && err != ERROR_SUCCESS)
#else
	int err = errno;
	if (err != EINPROGRESS && err != EAGAIN)
#endif
	{
		return true;
	}

	return false;
}

SOCKET 
TCPSocket::getSocket() const { return SocketInstance; }

int 
TCPSocket::getID() const { return SockID; }

void 
TCPSocket::setID(int value) { SockID = value; }

bool 
TCPSocket::isActive() { return SocketInstance != INVALID_SOCKET; }

void 
TCPSocket::resetData()
{
	//m_wRecvSize = 0;
	m_cbSendRound = 0;
	m_cbRecvRound = 0;
	m_dwSendXorKey = 0;
	m_dwRecvXorKey = 0;
	m_dwSendTickCount = 0;
	m_dwRecvTickCount = 0;
	m_dwSendPacketCount = 0;
	m_dwRecvPacketCount = 0;
}

unsigned long 
TCPSocket::GetUnixTickCount()
{
#if  CJ_TARGET_PLATFORM == CJ_PLATFORM_IOS || \
	CJ_TARGET_PLATFORM == CJ_PLATFORM_MAC
	//struct sysinfo si;
	//if (sysinfo(&si) == 0) return si.uptime;
	//return -1;

	struct timeval current;
	gettimeofday(&current, NULL);

	return current.tv_sec * 1000 + current.tv_usec / 1000;
#elif CJ_TARGET_PLATFORM != CJ_PLATFORM_WIN32
	struct timespec ts;

	clock_gettime(CLOCK_MONOTONIC, &ts);

	return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
#endif

	return 0;
}

void 
TCPSocket::setSocket(SOCKET value)
{
	if (value == INVALID_SOCKET) {
		//sprintf(logstr, "$%d 已经失效!", SocketInstance);
	}

	SocketInstance = value;
}
