#include <assert.h>

#include <stdio.h>
#include <string.h>

#include "zlib.h"
#include "TCPSocket.h"
#include "cocos2d.h"


USING_NS_CC;


void TCPSocket::registerSink(ITCPSocketSink* value)
{
	m_pITCPSocketSink = value;
}

bool TCPSocket::HasWSAStartup = false;

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

bool TCPSocket::init()
{
	//if (SocketInstance != INVALID_SOCKET) throw ("连接 SOCKET 句柄已经存在");
	//if (SocketStatus != SOCKET_STATUS_IDLE) throw ("连接状态不是等待连接状态");

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	if (!HasWSAStartup)
	{
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

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	if (SocketInstance == INVALID_SOCKET)
#else
	if (SocketInstance < 0)
#endif
	{
		perror("ERROR opening socket");
		closeSock();
		return false;
	}

#if( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )
	unsigned long ul = 1;
	int nRet = ioctlsocket(SocketInstance, FIONBIO, (unsigned long*)&ul);
	if (nRet == SOCKET_ERROR)
	{
		closeSock();
		throw ("set nonblock fail");
		return false;
	}
#endif

#if( CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
	int nFlags = fcntl(SocketInstance, F_GETFL, 0);
	int nRet = fcntl(SocketInstance, F_SETFL, nFlags | O_NONBLOCK);
	if (nRet == SOCKET_ERROR)
	{
		closeSock();
		return false;
	}
#endif

	int nNoDelay = 1;
	if (setsockopt(SocketInstance, IPPROTO_TCP, TCP_NODELAY, (char*)&nNoDelay, sizeof(nNoDelay)) == SOCKET_ERROR)
	{
		closeSock();
		return false;
	}

#if CC_TARGET_PLATFORM == CC_PLATFORM_MAC || CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	int set = 1;
	if (setsockopt(SocketInstance, SOL_SOCKET, SO_NOSIGPIPE, (void *)&set, sizeof(int)))
	{
		closeSock();
		return false;
	}
#endif

	return true;
}

bool TCPSocket::connect(unsigned long ip, int port)
{
	//if (ip == INADDR_NONE) throw ("目标服务器地址格式不正确，请检查后再次尝试！");

	resetData();

	sockaddr_in addr_in;
	bzero((void*)&addr_in, sizeof(addr_in));
	addr_in.sin_family = AF_INET;
	addr_in.sin_port = htons(port);
	addr_in.sin_addr.s_addr = ip;

	int iErrorCode = 0;

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	WSASetLastError(0);
	iErrorCode = ::connect(SocketInstance, (SOCKADDR*)&addr_in, sizeof(addr_in));
#else
	iErrorCode = ::connect(SocketInstance, (struct sockaddr *)&addr_in, sizeof(addr_in));
#endif

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	if (iErrorCode == SOCKET_ERROR)
#else
	if (iErrorCode < 0)
#endif
	{
		if (hasError())
		{
			closeSock();
			return false;
		}
	}

	int err = errno;

	SocketStatus = SOCKET_STATUS_CONNECT; // SOCKET_STATUS_WAIT;

	return true;
}

bool TCPSocket::connect(const char* serverip, int port)
{
	unsigned long serveraddr = inet_addr(serverip);
	if (serveraddr == INADDR_NONE)
	{
		closeSock();
		return false;
	}

	return connect(serveraddr, port);
}

void TCPSocket::closeSock()
{
	//关闭连接
	bool bClose = (SocketInstance != INVALID_SOCKET);
	SocketStatus = SOCKET_STATUS_IDLE;
	if (SocketInstance != INVALID_SOCKET)
	{
		
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
		closesocket(SocketInstance);
#else
		close(SocketInstance);
#endif
		setSocket(INVALID_SOCKET);
		SocketStatus = SOCKET_STATUS_IDLE;
	}
	if ((bClose == true) && (m_pITCPSocketSink != nullptr))
	{
		assert(m_pITCPSocketSink != nullptr);
		try { m_pITCPSocketSink->OnEventTCPSocketShut(SockID, SHUT_REASON_NORMAL); }
		catch (...) {}
	}

	resetData();
}

void TCPSocket::beginSendMsg()
{
	memset(SingleBuff4Send, 0, SOCKET_TCP_SEND_BUFFER);
	SingleBuffSendSize = 0;
}

void TCPSocket::pushNum(void* data, NumbericSize size)
{
	memcpy(SingleBuff4Send + SingleBuffSendSize, data, size);
	SingleBuffSendSize += size;
}

void TCPSocket::pushLString(char* data, int size)
{
	memcpy(SingleBuff4Send + SingleBuffSendSize, (void*)data, size);
	SingleBuffSendSize += size;
}

void TCPSocket::pushLString(char* data, short size)
{
	short* len = (short*)(SingleBuff4Send + SingleBuffSendSize);
	*len = size;
	memcpy(SingleBuff4Send + SingleBuffSendSize + sizeof(short), (void*)data, size);
	SingleBuffSendSize += size + sizeof(short);
}


void TCPSocket::endSendMsg()
{
	memset(SingleBuff4Send + SingleBuffSendSize, 0, SOCKET_TCP_SEND_BUFFER);
	SingleBuffSendSize = 0;
}

bool TCPSocket::sendMessage(unsigned short wMainCmdID, unsigned short wSubCmdID)
{
	if (!checkSocketStatus()) return false;

	SendPacket sp;

	//BYTE cbDataBuffer[SOCKET_TCP_BUFFER];
	TCP_Head * pHead = (TCP_Head *)sp.cbDataBuffer; // cbDataBuffer; //
	pHead->CommandInfo.wMainCmdID = wMainCmdID;
	pHead->CommandInfo.wSubCmdID = wSubCmdID;
	if (SingleBuffSendSize > 0)
	{
		assert(SingleBuff4Send != nullptr);
		//memcpy(pHead + 1, SingleBuff4Send, SingleBuffSendSize);
		BYTE bBuffer[SOCKET_TCP_BUFFER] = { 0 };
		unsigned long lResultSize = SOCKET_TCP_BUFFER - 1;
		SingleBuffSendSize = CompressData((unsigned char*)SingleBuff4Send, SingleBuffSendSize, bBuffer, lResultSize);
		memcpy(pHead + 1, bBuffer, SingleBuffSendSize);
	}

	pHead->TCPInfo.cbCheckCode = 1;
	pHead->TCPInfo.wPacketSize = sizeof(TCP_Head) + SingleBuffSendSize;
	pHead->TCPInfo.cbVersion = SOCKET_TCP_VER;

	WORD wSendSize = sizeof(TCP_Head) + SingleBuffSendSize;// EncryptBuffer(cbDataBuffer, sizeof(TCP_Head) + SingleBuffSendSize, sizeof(cbDataBuffer));
	//WORD wSendSize = EncryptBuffer(sp.cbDataBuffer, sizeof(TCP_Head) + SingleBuffSendSize, sizeof(sp.cbDataBuffer));

	sp.Size = wSendSize;
	SendPacketList.push_back(sp);

	return true;
	//return sendDataBuffer(cbDataBuffer, wSendSize);
}

//加密数据
WORD TCPSocket::EncryptBuffer(BYTE pcbDataBuffer[], WORD wDataSize, WORD wBufferSize)
{
	//效验参数
	assert(wDataSize >= sizeof(TCP_Head));
	assert(wDataSize <= (sizeof(TCP_Head) + SOCKET_TCP_BUFFER));
	assert(wBufferSize >= (wDataSize + 2 * sizeof(DWORD)));

	//调整长度
	WORD wEncryptSize = wDataSize - sizeof(TCP_Info);
	BYTE mod = (wEncryptSize) % 8;
	if (0 != mod)
	{
		mod = 8 - mod;
		memset(pcbDataBuffer + wDataSize, 0, mod);
	}

	//填写信息头
	TCP_Head * pHead = (TCP_Head *)pcbDataBuffer;
	pHead->TCPInfo.cbCheckCode = 1;
	//pHead->TCPInfo.cbMod = mod;
	pHead->TCPInfo.wPacketSize = wDataSize + mod;
	pHead->TCPInfo.cbVersion = SOCKET_TCP_VER;

	//加密数据
	enc.encdec(pcbDataBuffer + sizeof(TCP_Info), wEncryptSize + mod, true);

	return wDataSize + mod;
}

//解密数据
WORD TCPSocket::CrevasseBuffer(BYTE pcbDataBuffer[], WORD wDataSize)
{
	//效验参数
	assert(wDataSize >= sizeof(TCP_Head));
	assert(((TCP_Head *)pcbDataBuffer)->TCPInfo.wPacketSize == wDataSize);

	TCP_Head * pHead = (TCP_Head *)pcbDataBuffer;

	enc.encdec(pcbDataBuffer + sizeof(TCP_Info), pHead->TCPInfo.wPacketSize - sizeof(TCP_Info), false);

	//wDataSize = pHead->TCPInfo.wPacketSize - pHead->TCPInfo.cbMod;
	return wDataSize;
}


bool TCPSocket::sendMessage()
{
	if (!checkSocketStatus()) return false;

	return sendDataBuffer((char*)SingleBuff4Send, SingleBuffSendSize);
}

SendState TCPSocket::sendDataBuffer(void *data, size_t size)
{
	if (!checkSocketStatus()) return SendState_Failed;

	assert(size != 0);
	assert(data!= nullptr);
	
	if (size > 0)
	{
		size_t sended = 0;
		while (sended < size)
		{
#if( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )
			int result = ::send(SocketInstance, (char*)data + sended, size - sended, 0);
#endif

#if( CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
			int result = ::send(SocketInstance, (char*)data + sended, size - sended, SO_NOSIGPIPE);
#endif
			if (result == SOCKET_ERROR)
			{
                /*
				if (hasError())
				{
					CCLOG("$发送数据失败!");
					hasError();
					int err = errno;
					return SendState_Failed;
				}
				else
				{
					continue;
				}
                 */
                continue;
			}
			sended += result;
		}

		return SendState_Sended;
	}

	return SendState_Failed;
}

int TCPSocket::recv()
{
	if (!checkSocketStatus()) return 0;

	try {
		int iresult = ::recv(SocketInstance, (char*)Buff4Rcv + RecvedSize, sizeof(Buff4Rcv) - RecvedSize, 0);
		if (iresult == SOCKET_ERROR)
		{
			//throw ("网络连接关闭，读取数据失败");
			//printf("%s", "网络连接关闭，读取数据失败");
			return 0;
		}
		RecvedSize += iresult;

		WORD wPacketSize = 0;
		BYTE cbDataBuffer[SOCKET_TCP_PACKET + sizeof(TCP_Head)];
		TCP_Head * pHead = (TCP_Head *)Buff4Rcv;

		while (RecvedSize >= sizeof(TCP_Head))
		{
			wPacketSize = pHead->TCPInfo.wPacketSize;
			assert(pHead->TCPInfo.cbVersion == SOCKET_TCP_VER);
			assert(wPacketSize <= (SOCKET_TCP_BUFFER));

			//if (pHead->TCPInfo.cbVersion != SOCKET_TCP_VER) throw ("版本不对");
			//if (wPacketSize > (SOCKET_TCP_PACKET + sizeof(TCP_Head))) throw ("数据包太大");
			if (RecvedSize < wPacketSize) return 1;

			memcpy(cbDataBuffer, Buff4Rcv, wPacketSize);
			RecvedSize -= wPacketSize;
			memmove(Buff4Rcv, Buff4Rcv + wPacketSize, RecvedSize);

			WORD wRealySize = wPacketSize; // CrevasseBuffer(cbDataBuffer, wPacketSize); //wPacketSize; // 
			assert(wRealySize >= sizeof(TCP_Head));

			//解释数据
			WORD wDataSize = wRealySize - sizeof(TCP_Head);
			void * pDataBuffer = cbDataBuffer + sizeof(TCP_Head);
			TCP_Command Command = ((TCP_Head *)cbDataBuffer)->CommandInfo;

			if (wDataSize > 0)
			{
				BYTE bBuffer[SOCKET_TCP_BUFFER] = { 0 };
				unsigned long lResultSize = SOCKET_TCP_BUFFER - 1;
				wDataSize = UnCompressData((unsigned char*)pDataBuffer, wDataSize, bBuffer, lResultSize);
				memcpy(pDataBuffer, bBuffer, wDataSize);
			}
			else
			{
				pDataBuffer = nullptr;
			}

			//内核命令
			if (Command.wMainCmdID == MDM_KN_COMMAND)
			{
				switch (Command.wSubCmdID)
				{
				case SUB_KN_DETECT_SOCKET:	//网络检测
				{
					//发送数据
					beginSendMsg();

					CCLOG("$get 网络检测 !");
					sendMessage(MDM_KN_COMMAND, SUB_KN_DETECT_SOCKET);
					break;
				}
				}
				continue;
			}

			//处理数据
			if (m_pITCPSocketSink)
			{
				bool bSuccess = m_pITCPSocketSink->OnEventTCPSocketRead(SocketInstance, Command, pDataBuffer, wDataSize);
				//if (bSuccess == false) throw ("网络数据包处理失败");
			}
		}
	}
	catch (...)
	{
		closeSock();
	}

	return 1;
}

void TCPSocket::update()
{
	if (!SendPacketList.empty())
	{
		if (CurSendingState == SendState_Sended || 
			CurSendingState == SendState_Failed)
		{
			std::vector<SendPacket>::iterator itor = SendPacketList.begin();
			if (sendDataBuffer(itor->cbDataBuffer, itor->Size))
			{
				SendPacketList.erase(itor);
			}
		}
	}
}

WORD TCPSocket::SeedRandMap(WORD wSeed)
{
	unsigned long dwHold = wSeed;
	return (WORD)((dwHold = dwHold * 241103L + 2533101L) >> 16);
}

BYTE TCPSocket::MapSendByte(BYTE cbData)
{
	BYTE cbMap = g_SendByteMap[(BYTE)(cbData + m_cbSendRound)];
	m_cbSendRound += 3;
	return cbMap;
}

BYTE TCPSocket::MapRecvByte(BYTE cbData)
{
	BYTE cbMap = g_RecvByteMap[cbData] - m_cbRecvRound;
	m_cbRecvRound += 3;
	return cbMap;
}

bool TCPSocket::checkSocketStatus()
{
	bool result = CONNECT_SUCCESS == isConnected();
	bool haserr = hasError();
	result = SocketInstance != INVALID_SOCKET &&
		SocketStatus == SOCKET_STATUS_CONNECT;

	if (!result)
	{
		//CCLOG("$网络已无效!");
	}
	else
	{
	}

	return result;
}

bool TCPSocket::sendMsg(unsigned short wMainCmdID, unsigned short wSubCmdID)
{
	if (!checkSocketStatus()) return false;

	BYTE cbDataBuffer[SOCKET_TCP_BUFFER];
	TCP_Head * pHead = (TCP_Head *)cbDataBuffer;
	pHead->CommandInfo.wMainCmdID = wMainCmdID;
	pHead->CommandInfo.wSubCmdID = wSubCmdID;

	WORD wSendSize = sizeof(TCP_Head);// EncryptBuffer(cbDataBuffer, sizeof(TCP_Head), sizeof(cbDataBuffer));

	return sendDataBuffer(cbDataBuffer, wSendSize);
}

//压缩数据
unsigned long TCPSocket::CompressData(unsigned char* pcbSourceData, unsigned long lSourceSize, BYTE cbResultData[], unsigned long lResultSize)
{
	if (compress(cbResultData, &lResultSize, pcbSourceData, lSourceSize) == 0L)
	{
		return lResultSize;
	}

	return 0L;
}

unsigned long TCPSocket::UnCompressData(unsigned char* pcbSourceData, unsigned long lSourceSize, BYTE cbResultData[], unsigned long lResultSize)
{
	if (uncompress(cbResultData, &lResultSize, pcbSourceData, lSourceSize) == 0L)
	{
		return lResultSize;
	}

	return 0L;
}

bool TCPSocket::sendMsg(unsigned short wMainCmdID, unsigned short wSubCmdID, void * pData, unsigned short wDataSize)
{
	if (!checkSocketStatus()) return false;

	assert(wDataSize <= SOCKET_TCP_PACKET);
	if (wDataSize > SOCKET_TCP_PACKET) return false;

	BYTE cbDataBuffer[SOCKET_TCP_BUFFER];
	TCP_Head * pHead = (TCP_Head *)cbDataBuffer;
	pHead->CommandInfo.wMainCmdID = wMainCmdID;
	pHead->CommandInfo.wSubCmdID = wSubCmdID;
	if (wDataSize > 0)
	{
		assert(pData != nullptr);
		//memcpy(pHead + 1, pData, wDataSize);
		//进行压缩处理
		BYTE bBuffer[SOCKET_TCP_BUFFER] = { 0 };
		unsigned long lResultSize = SOCKET_TCP_BUFFER - 1;
		wDataSize = CompressData((unsigned char*)pData, wDataSize, bBuffer, lResultSize);
		memcpy(pHead + 1, bBuffer, wDataSize);
	}

	WORD wSendSize = EncryptBuffer(cbDataBuffer, sizeof(TCP_Head) + wDataSize, sizeof(cbDataBuffer));

	return sendDataBuffer(cbDataBuffer, wSendSize);
}


int TCPSocket::sendSth(void *pdata, int size)
{
	if (!checkSocketStatus()) return false;

	int iSendResult = ::send(SocketInstance, (char*)pdata, size, 0);

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
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

int TCPSocket::recSth(void *data, int& size)
{
	//if (!checkSocketStatus()) return 0;

	//int iResult = ::recv(SocketInstance, (char*)data, size, 0);
	//if (iResult > 0) {
	//	printf("Bytes received: %d\n", iResult);
	//}
	//else if (iResult == 0)
	//{
	//}
	//else {
	//	printf("recv failed with error: %d\n", WSAGetLastError());
	//	closesocket(SocketInstance);
	//	WSACleanup();
	//	return 1;
	//}

	return 0;
}

int TCPSocket::isConnected()
{
	if (SocketInstance == INVALID_SOCKET)
	{
		//CCLOG("$网络连接断开!");
		return CONNECT_FAILURE;
	}

	fd_set	fd;
	struct timeval tv;

	FD_ZERO(&fd);
	FD_SET(SocketInstance, &fd);

	tv.tv_sec = 0;
	tv.tv_usec = 0;

	if( select((int)(SocketInstance + 1), NULL, &fd, NULL, &tv) > 0 )
	{
		if( FD_ISSET(SocketInstance, &fd) )
		{
#if( CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 )

			return CONNECT_SUCCESS;

#elif( CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID )

			int nError;
			socklen_t len = sizeof(nError);
			if( getsockopt(SocketInstance, SOL_SOCKET, SO_ERROR, &nError, &len) < 0 )
			{
				CCLOG("$网络连接断开！");
				return CONNECT_FAILURE;
			}
			if( nError == ECONNREFUSED || nError == ETIMEDOUT )
			{
				CCLOG("$网络连接断开！");
				return CONNECT_FAILURE;
			}
			return CONNECT_SUCCESS;

#endif
		}
	}
	return CONNECT_INPROGRESS;
}

bool TCPSocket::hasError()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	int err = WSAGetLastError();
	if (err != WSAEWOULDBLOCK)
	{
#else
	int err = errno;
	if (err != EINPROGRESS && err != EAGAIN) {
#endif
	return true;
	}

	return false;
}

SOCKET TCPSocket::getSocket() const
{
	return SocketInstance;
}

int TCPSocket::getID() const
{
	return SockID;
}

void TCPSocket::setID(int value)
{
	SockID = value;
}

bool TCPSocket::isActive()
{
	return SocketInstance != INVALID_SOCKET;
}

void TCPSocket::resetData()
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

unsigned long TCPSocket::GetUnixTickCount()
{
#if  CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC
	//struct sysinfo si;
	//if (sysinfo(&si) == 0) return si.uptime;
	//return -1;

	struct timeval current;
	gettimeofday(&current, NULL);

	return current.tv_sec * 1000 + current.tv_usec / 1000;
#elif CC_TARGET_PLATFORM != CC_PLATFORM_WIN32
	struct timespec ts;

	clock_gettime(CLOCK_MONOTONIC, &ts);

	return (ts.tv_sec * 1000 + ts.tv_nsec / 1000000);
#endif

	return 0;
}

void TCPSocket::setSocket(SOCKET value)
{
	if (value == INVALID_SOCKET)
	{
		CCLOG("关闭连接!");
		char logstr[32] = {0};
		sprintf(logstr, "$%d 已经失效!", SocketInstance);
		CCLOG(logstr);
	}
	SocketInstance = value;
}
