#include "GameNetWorkManager.h"

GameNetWorkManager::~GameNetWorkManager()
{

}

// c++11 Singleton
GameNetWorkManager& GameNetWorkManager::getInstance()
{
	static GameNetWorkManager s_GameNetWorkManager;
	return s_GameNetWorkManager;
}
