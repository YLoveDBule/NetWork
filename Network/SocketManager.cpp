#include "SocketManager.h"



SocketManager::SocketManager() //: HasWASStartup(false)
{

}

SocketManager::~SocketManager()
{

}

bool SocketManager::startupWSA()
{
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	//if (!HasWASStartup)
	{
		WSADATA WSAData;
		WORD wVersionRequested = MAKEWORD(2, 2);
		int iErrorCode = WSAStartup(wVersionRequested, &WSAData);
		if (iErrorCode != 0) return false;
		//HasWASStartup = true;
	}
#endif
	//HasWASStartup = true;

	return true;
}

