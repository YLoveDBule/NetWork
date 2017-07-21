#ifndef _TCPSOCKET_H_
#define _TCPSOCKET_H_
#include "TCPPlatform.h"
#include "SocketHeader.h"
#include "Packet.h"
#include <string>
#include "NimeiQueue.h"
#include "cocos2d.h"
#if TCP_TARGET_PLATFORM == TCP_PLATFORM_WIN32
#include <WinSock2.h>
#include <io.h>
#include <WS2tcpip.h>
#pragma comment( lib, "ws2_32.lib" )
#ifndef bzero 
#define bzero(a,b) memset(a,0,b);
#endif // !bzero 
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
#include <errno.h>
#include <fcntl.h>
#define SOCKET int
#define SOCKET_ERROR -1
#define INVALID_SOCKET -1
#endif

enum SendState {
	SendState_Sending = 0,
	SendState_Sended,
	SendState_Failed,
	SendState_Success
};

class SendPacket {
private:
	char m_pData[SOCKET_TCP_PACKET];
	size_t m_size;
	unsigned short m_wMainID;
	unsigned short m_wSubID;
public:
	SendPacket() = delete;
	SendPacket(const char * pBuffer, const unsigned short wMainID, const unsigned short wSubID,const unsigned short wSize)
	{
		m_wMainID = wMainID;
		m_wSubID = wSubID;
		m_size = wSize;
		memset(m_pData, 0, sizeof(m_pData));
		memcpy(m_pData, pBuffer, wSize);
	}
	SendPacket(const char *pBuffer, const unsigned short wSize)
	{
		m_size = wSize;
		memset(m_pData, 0,sizeof(m_pData));
		memcpy(m_pData, pBuffer, wSize);
	}
	char * getData()
	{
		return m_pData;
	}
	const int getSize()
	{
		return m_size;
	}
};


typedef struct RecvPacket {
	unsigned short Size;
	SOCKET SID;
	char Data[SOCKET_TCP_BUFFER];
	TCP_Command Comd;

	RecvPacket() : Size(0),
		SID(0)
	{
		memset(Data, 0, SOCKET_TCP_BUFFER);
	}
} RecvPacket;

class TCPSocket : public cocos2d::Ref
{
public:
	TCPSocket();
	virtual ~TCPSocket();
	virtual bool init();
	virtual bool connectServer(const std::string serverIp, const int serverPort);
	virtual bool connectServer(const unsigned long serverIp, const int serverport);
	virtual void closeSock();
	virtual void beginSendMsg();
	virtual void pushNum(void * data, const unsigned int size);
	virtual void pushLString(const char* data, const int size);
	virtual void pushLString(const char* data, const short size);
	virtual void endSendMsg();
	virtual bool sendData(unsigned short wMainCmdID, unsigned short wSubCmdID, unsigned int& sendedSize);
	virtual bool sendData(unsigned short wMainCmdID, unsigned short wSubCmdID, void * pData, unsigned short wDataSize);
	virtual bool sendData();
	virtual bool MappedBuffer(void *pData, unsigned short wDataSize);
	virtual bool UnMappedBuffer(void *pData, unsigned short wDataSize);
	virtual void update(float dt);
	virtual int  getData();
	virtual bool registerSink(ITCPSocketSink * value);
protected:
	virtual bool dnsParse(std::string serverIp);
	virtual void setSocket(SOCKET value);
	virtual void resetData();
	virtual bool checkrecvfall(const int resultCode);
private:
	virtual SendState sendDataBuffer(void *data, size_t size);
private:
	std::string m_sAddress;
	SOCKET  m_socket;
	ITCPSocketSink *		m_pITCPSocketSink;
protected:
	char    m_socketStatus;
private:
	int		m_socketID;
	unsigned int m_singleBuffSendSize;
	char m_singleBuff4Send[SOCKET_TCP_SEND_BUFFER];
	NimeiQueue<SendPacket> m_sendpacketList;
	NimeiQueue<RecvPacket> m_recvPacketList;
	SendState m_curSendingState;
	char m_buff4Rcv[SOCKET_TCP_RCV_BUFFER];
	char m_buff4Send[SOCKET_TCP_SEND_BUFFER]; 
	int m_recvedSize;
};

#endif