#include "Define.h"
#include "Messager.h"


USING_NS_CC;


Messager* Messager::Instance = nullptr;

Messager* Messager::getInstance()
{
	if (Instance == nullptr) {
		Instance = new Messager;
	}

	return Instance;
}

Messager::~Messager()
{
	PacketMsg2LuaList.clear();
}

Messager::Messager()
{
	CJNetworkMgr->registerMsgAdopter(this);

#if CUR_DEVICE_TYPE == DEVICE_TYPE_PC
	IsForPC = CUR_DEVICE_TYPE == DEVICE_TYPE_PC;
	IsSubProcess = IsForPC && ProcessModuleMgr->isSubProcess();
#endif
}

bool 
Messager::houseNetMsg(SOCKET wSocketID,
	unsigned short maincmd,
	unsigned short subcmd,
	void *pData,
	unsigned short wDataSize)
{
	bool result = true;

	PacketMsg2Lua* pml = new PacketMsg2Lua;
	pml->Sockid = wSocketID;
	pml->Comm.wMainCmdID = maincmd;
	pml->Comm.wSubCmdID = subcmd;
	pml->DataBuff = new char[wDataSize];
	memcpy(pml->DataBuff, (char*)pData, wDataSize);
	pml->DataSize = wDataSize;

	PacketMsg2LuaList.push_back(pml);

	//bool result = send2Lua(wSocketID,
	//	maincmd,
	//	subcmd,
	//	(char*)pData,
	//	wDataSize);

	//if (IsForPC) {
	//	result |= ProcessModuleMgr->sendData(wSocketID,
	//		maincmd,
	//		subcmd,
	//		pData,
	//		wDataSize);
	//}

	return result;
}

void 
Messager::update(float dt)
{
	for (std::vector<PacketMsg2Lua*>::size_type n = 0;
		n < PacketMsg2LuaList.size();)
	{
		if (n < PacketMsg2LuaList.size() && PacketMsg2LuaList[n]) {

			bool result = send2Lua(PacketMsg2LuaList[n]->Sockid,
				PacketMsg2LuaList[n]->Comm.wMainCmdID,
				PacketMsg2LuaList[n]->Comm.wSubCmdID,
				(char*)PacketMsg2LuaList[n]->DataBuff,
				PacketMsg2LuaList[n]->DataSize);

			if (IsForPC) {
				result |= ProcessModuleMgr->sendData(PacketMsg2LuaList[n]->Sockid,
					PacketMsg2LuaList[n]->Comm.wMainCmdID,
					PacketMsg2LuaList[n]->Comm.wSubCmdID,
					(char*)PacketMsg2LuaList[n]->DataBuff,
					PacketMsg2LuaList[n]->DataSize);
			}

			//houseNetMsg(PacketMsg2LuaList[n]->Sockid,
			//	PacketMsg2LuaList[n]->Comm.wMainCmdID,
			//	PacketMsg2LuaList[n]->Comm.wSubCmdID,
			//	(char*)PacketMsg2LuaList[n]->DataBuff,
			//	PacketMsg2LuaList[n]->DataSize);

			PacketMsg2LuaList[n]->clear();
			delete PacketMsg2LuaList[n];

			PacketMsg2LuaList.erase(PacketMsg2LuaList.begin() + n);
		}
		else
			n++;
	}
}

bool 
Messager::onConnectShutdown(unsigned short sockid,
	unsigned char shutReason)
{
	bool result = false;

	//return result;

	if (!LuaStatePointer) {
		auto engine = LuaEngine::getInstance();
		if (engine) LuaStatePointer = engine->getLuaStack()->getLuaState();
	}

	if (LuaStatePointer) {
		lua_getglobal(LuaStatePointer, "CJ_onNetworkStatus");

		lua_pushinteger(LuaStatePointer, sockid);
		lua_pushinteger(LuaStatePointer, shutReason);

		result = lua_pcall(LuaStatePointer, 2, 0, NULL) == 0;
	}

	return result;
}

Messager::Messager(const Messager&)
{

}

bool 
Messager::send2Lua(SOCKET sockid,
	unsigned short maincmd,
	unsigned short subcmd,
	void *pdata,
	unsigned short size)
{
	bool result = false;

	if (!LuaStatePointer) {
		auto engine = LuaEngine::getInstance();
		if (engine) LuaStatePointer = engine->getLuaStack()->getLuaState();
	}

	if (LuaStatePointer) {
		lua_getglobal(LuaStatePointer, "CJ_global_recvmsg");

		lua_pushinteger(LuaStatePointer, sockid);
		lua_pushinteger(LuaStatePointer, maincmd);
		lua_pushinteger(LuaStatePointer, subcmd);

		lua_pushlstring(LuaStatePointer, (char*)pdata, size);
		lua_pushinteger(LuaStatePointer, size);

		result = lua_pcall(LuaStatePointer, 5, 0, NULL) == 0;
	}

	return result;
}

bool 
Messager::sendSelfUsrID(size_t id)
{
	bool result = false;

	if (!LuaStatePointer) {
		auto engine = LuaEngine::getInstance();
		if (engine) LuaStatePointer = engine->getLuaStack()->getLuaState();
	}

	if (LuaStatePointer) {
		lua_getglobal(LuaStatePointer, "CJ_Butler_setSelfUsrID");

		lua_pushinteger(LuaStatePointer, id);

		result = lua_pcall(LuaStatePointer, 1, 0, NULL) == 0;
	}

	return result;
}

int 
Messager::getSelfUsrID()
{
	int result = 0;

	if (!LuaStatePointer) {
		auto engine = LuaEngine::getInstance();
		if (engine) LuaStatePointer = engine->getLuaStack()->getLuaState();
	}

	if (LuaStatePointer) {
		lua_getglobal(LuaStatePointer, "CJ_Butler_getSelfUsrID");

		if (!lua_pcall(LuaStatePointer, 0, 1, NULL)) {
			result = lua_tonumber(LuaStatePointer, -1);
		}
	}

	return result;
}

bool 
Messager::pushServerAskResult(int recorder, SOCKET sid)
{
	bool result = false;
	if (!LuaStatePointer) {
		auto engine = LuaEngine::getInstance();
		if (engine) LuaStatePointer = engine->getLuaStack()->getLuaState();
	}

	if (LuaStatePointer) {
		lua_getglobal(LuaStatePointer, "CJ_ask2subserverResult");  //CJ_ask2subserverResult //CJ_nimeidedaye
		lua_pushinteger(LuaStatePointer, recorder);
		lua_pushinteger(LuaStatePointer, sid);
		result = lua_pcall(LuaStatePointer, 2, 0, NULL) == 0;
	}

	return result;
}

void 
Messager::ask2Subserver(int recorder, const char* ip, int port)
{
	if (IsForPC && IsSubProcess) {

	}
	else {
		CJNetworkMgr->ask2Subserver(recorder, ip, port);
	}
}

void 
Messager::closeConnect(int value)
{
	if (IsForPC && IsSubProcess) {
		long long testsh = (long long)ProcessModuleMgr->getSelfHwnd();
		ProcessModuleMgr->sendData(ProcessModuleMgr->getDefaultDesHwnd(), 0, 0,
			NULL, 0, IPC_DATA_SUBFALLDOWN);

		ProcessModuleMgr->onSuicide();
	}
	else {
		CJNetworkMgr->closeConnect(value);
	}
}

void 
Messager::suicide()
{
	ProcessModuleMgr->suicide();	
}

void 
Messager::closeAllConnect()
{
	if (IsForPC && !IsSubProcess) {
		CJNetworkMgr->closeAllConnect();
	}
}

void 
Messager::beginSendMsg(SOCKET value)
{
	if (IsForPC && IsSubProcess) {
		memset(SingleBuff4Send, 0, IPC_PACKET);
		SingleBuffSendSize = 0;
	}
	else {
		CJNetworkMgr->beginSendMsg(value);
	}
}

void 
Messager::pushNum(SOCKET socket, void* data, unsigned int size)
{
	if (IsForPC && IsSubProcess) {
		memcpy(SingleBuff4Send + SingleBuffSendSize, data, size);
		SingleBuffSendSize += size;
	}
	else {
		CJNetworkMgr->pushNum(socket, data, (NumbericSize)size);
	}
}

void 
Messager::pushLString(SOCKET socket, char* data, int size)
{
	if (IsForPC && IsSubProcess) {
		memcpy(SingleBuff4Send + SingleBuffSendSize, (void *)data, size);
		SingleBuffSendSize += size;
	}
	else {
		CJNetworkMgr->pushLString(socket, data, size);
	}
}

void 
Messager::pushLStringShortSize(SOCKET socket, char* data, short size)
{
	if (IsForPC && IsSubProcess) {
		short* len = (short*)(SingleBuff4Send + SingleBuffSendSize);
		*len = size;
		memcpy(SingleBuff4Send + SingleBuffSendSize + sizeof(short),
			(void*)data, size);
		SingleBuffSendSize += size + sizeof(short);
	}
	else {
		CJNetworkMgr->pushLStringShortSize(socket, data, size);
	}
}

void 
Messager::endSendMsg(SOCKET socket)
{
	if (IsForPC && IsSubProcess) {
		memset(SingleBuff4Send, 0, IPC_PACKET);
		SingleBuffSendSize = 0;
	}
	else {
		CJNetworkMgr->endSendMsg(socket);
	}
}

bool 
Messager::sendMessage(SOCKET socket,
	unsigned short maincmd,
	unsigned short subcmd)
{
	bool result = false;

	if (IsForPC && IsSubProcess) {
		HWND target = ProcessModuleMgr->getDefaultDesHwnd();
		if (target != (HWND)NULL) {
			result = ProcessModuleMgr->sendData(target,
				maincmd,
				subcmd,
				SingleBuff4Send,
				SingleBuffSendSize);
		}
	}
	else {
		result = CJNetworkMgr->sendMessage(socket, maincmd, subcmd);
	}

	return result;
}

bool 
Messager::sendMessage(SOCKET socket,
	unsigned short maincmd,
	unsigned short subcmd,
	void *pData,
	unsigned short wDataSize)
{
	bool result = false;

	if (IsForPC && IsSubProcess) {
		result = ProcessModuleMgr->sendData(socket,
			maincmd,
			subcmd,
			pData,
			wDataSize);
	}
	else {
		result = CJNetworkMgr->sendMessage(socket,
			maincmd,
			subcmd,
			pData,
			wDataSize);
	}

	return result;
}

void 
Messager::addFreeMainCMDForSub(int value)
{
	if (IsForPC && !IsSubProcess) {
		ProcessModuleMgr->pushFreeMainCMDForSub(value);
	}
}

void 
Messager::minimizeWindow()
{
	HWND selfhwnd = ProcessModuleMgr->getSelfHwnd();
	if (selfhwnd != (HWND)NULL) {
		WINDOWPLACEMENT* wp = new WINDOWPLACEMENT;
		wp->length = sizeof(WINDOWPLACEMENT);
		if (GetWindowPlacement(selfhwnd, wp)) {
			wp->showCmd = SW_MINIMIZE;
			SetWindowPlacement(selfhwnd, wp);
		}
	}
}

void 
Messager::setIsSubProcess(bool value)
{
	IsSubProcess = value;
}

bool 
Messager::isSubProcess() const
{
	return IsSubProcess;
}