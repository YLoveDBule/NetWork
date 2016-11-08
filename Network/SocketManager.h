#pragma once

#include "TCPSocket.h"



class SocketManager
{
public:
	SocketManager();
	~SocketManager();

	bool startupWSA();

private:
	//bool HasWASStartup;

};
