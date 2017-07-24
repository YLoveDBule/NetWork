#pragma once
#ifndef __GAMENETWORKMANAGER__
#define __GAMENETWORKMANAGER__
#include "TCPSocket.h"
#include "Define.h"
class GameNetWorkManager
{
public:
	virtual ~GameNetWorkManager();
	static GameNetWorkManager&getInstance();
private:
	GameNetWorkManager();
};

#endif