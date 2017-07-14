#ifndef _TCPSOCKET_H_
#define _TCPSOCKET_H_
#include "TCPPlatform.h"
#include <string>
#if TCP_TARGET_PLATFORM == TCP_PLATFORM_WIN32
#include <Winsock2.h>
#include <windows.h>
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
class TCPSocket
{
public:
	TCPSocket();
	virtual ~TCPSocket();
	virtual bool init();
	virtual bool connectServer(const std::string serverIp, const int serverPort);
	virtual bool connectServer(const unsigned long serverIp, const int serverport);
	virtual void closeSock();
protected:
	virtual bool dnsParse(std::string serverIp);
private:
	std::string m_sAddress;
	SOCKET  m_socket;
};

#endif