#include "TCPSocket.h"
#include <memory>
#include <sstream>

TCPSocket::TCPSocket()
	:m_sAddress(""),
	m_socket(INVALID_SOCKET)
{
	
}

TCPSocket::~TCPSocket()
{
	
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
	ss << serverIp;
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
		unsigned long ul = 1;
		auto nRet = ioctlsocket(m_socket, FIONBIO, &ul);
	}
}

bool TCPSocket::connectServer(const unsigned long serverIp, const int serverport)
{
	//resetData();
	sockaddr_in addr_in;
	memset((void*)&addr_in, 0, sizeof(addr_in));
	addr_in.sin_family = AF_INET;
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

}
