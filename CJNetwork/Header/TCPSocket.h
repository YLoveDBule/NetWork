#pragma once

#ifndef _TCPSOCKET_H_
#define _TCPSOCKET_H_



#include "CJPlatform.h"

#if CJ_TARGET_PLATFORM == CJ_PLATFORM_WIN32
#include <Winsock2.h>
#include <windows.h>
#include <io.h>

#include <WS2tcpip.h>

#pragma comment( lib, "ws2_32.lib" )

#ifndef bzero
#define bzero(a, b) memset(a, 0, b);
#endif

#else
#include <netdb.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>


//
#include <errno.h>
#include <fcntl.h>

//#include <uuid/uuid.h>

typedef int                 BOOL;
typedef unsigned char       BYTE;
typedef unsigned short      WORD;
typedef unsigned long       DWORD;

//typedef struct _GUID {
//	unsigned long  Data1;
//	unsigned short Data2;
//	unsigned short Data3;
//	unsigned char  Data4[8];
//} GUID;

#endif

#include <thread>
#include <mutex>

#include <vector>

#include "Packet.h"
#include "EncryptDes.h"
#include "SocketHeader.h"

enum NumbericSize {
	NumbericSize_INT = sizeof(int),
	NumbericSize_SHORT = sizeof(short),
	NumbericSize_LONG = sizeof(long)
};

enum SendState {
    SendState_Sending = 0,
    SendState_Sended,
    SendState_Failed,
    SendState_Success
};

typedef struct SendPacket {
	unsigned char cbDataBuffer[SOCKET_TCP_BUFFER] = {0};
	size_t Size = 0;

	SendPacket() : Size(0) {
		memset(cbDataBuffer, 0, SOCKET_TCP_BUFFER);
	}

} SendPacket;

class TCPSocket
{
public:
	TCPSocket();
	virtual ~TCPSocket();

	bool init();

	bool connect(unsigned long ip, int port);

	bool connect(const char* serverip, int port);

	void closeSock();

	void beginSendMsg();

	void pushNum(void* data, NumbericSize size);

	void pushLString(char* data, int size);

	void pushLString(char* data, short size);

	void endSendMsg();

	bool sendMessage(unsigned short wMainCmdID, 
		unsigned short wSubCmdID);

	bool sendMessage(unsigned short wMainCmdID,
		unsigned short wSubCmdID,
		void *pData,
		unsigned short wDataSize);

	bool sendMessage();

	bool sendMsg(unsigned short wMainCmdID, 
		unsigned short wSubCmdID);

	bool sendMsg(unsigned short wMainCmdID, 
		unsigned short wSubCmdID, 
		void *pData, 
		unsigned short wDataSize);

	int sendSth(void *pdata, int size);

	int isConnected();

	unsigned long CompressData(
		unsigned char* pcbSourceData, 
		unsigned long lSourceSize, 
		unsigned char cbResultData[], 
		unsigned long lResultSize); 

	unsigned long UnCompressData(
		unsigned char* pcbSourceData, 
		unsigned long lSourceSize, 
		unsigned char cbResultData[], 
		unsigned long lResultSize);

	bool hasError();

	void setSocket(SOCKET value);

	SOCKET getSocket() const;

	int getID() const;

	void setID(int value);

	bool isActive();

protected:
	//解密数据
	unsigned short CrevasseBuffer(
		unsigned char cbDataBuffer[], 
		unsigned short wDataSize);

	//加密数据
	unsigned short EncryptBuffer(
		unsigned char cbDataBuffer[], 
		unsigned short wDataSize, 
		unsigned short wBufferSize);

	void resetData();

	unsigned long GetUnixTickCount();

public:
	int recv();

	void update();

	void multistart();

	void multiupdate();

public:
	bool registerSink(ITCPSocketSink*);

private:
	SendState sendDataBuffer(void *data, size_t size);

	inline unsigned short SeedRandMap(unsigned short wSeed);

	inline unsigned char MapSendByte(unsigned char cbData);

	inline unsigned char MapRecvByte(unsigned char cbData);

	bool checkSocketStatus();

protected:
	char SocketStatus;

protected:
	unsigned char							m_cbSendRound;

	unsigned char							m_cbRecvRound;

	unsigned long							m_dwSendXorKey;

	unsigned long							m_dwRecvXorKey;

protected:
	unsigned long							m_dwSendTickCount;

	unsigned long							m_dwRecvTickCount;

	unsigned long							m_dwSendPacketCount;
	
	unsigned long							m_dwRecvPacketCount;

private:
	static bool HasWSAStartup;

	bool IsSending;

	SOCKET SocketInstance;

	ITCPSocketSink *				m_pITCPSocketSink;

	SendState CurSendingState;

	int SockID;
	
	int RecvedSize;

	int SingleBuffSendSize;

	int OutBufLen;

	char Buff4Rcv[SOCKET_TCP_RCV_BUFFER];

	char Buff4Send[SOCKET_TCP_SEND_BUFFER];

	char SingleBuff4Send[SOCKET_TCP_SEND_BUFFER];

	std::vector<SendPacket> SendPacketList;

	CEncryptDes enc;
};



#endif
