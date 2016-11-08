#include <sstream>
#include <thread>
//#include "ProcessModule.h"
#include "ProcessModuleManager.h"

#pragma pack(1)


ProcessModuleManager* ProcessModuleManager::Instance = nullptr;

ProcessModuleManager::ProcessModuleManager()
{

}

ProcessModuleManager::ProcessModuleManager(const ProcessModuleManager&)
{

}

ProcessModuleManager&
ProcessModuleManager::operator=(const ProcessModuleManager&)
{
	return *this;
}

ProcessModuleManager::~ProcessModuleManager()
{
	IsInited = false;

	isActive = false;

	release();
}

ProcessModuleManager*
ProcessModuleManager::getInstance()
{
	if (Instance == nullptr) {
		Instance = new ProcessModuleManager();
		Instance->init();
	}
	return Instance;
}

void
ProcessModuleManager::regsterHwndWithSid(SOCKET sid, HWND hwnd)
{
	//SidHwndPairList.insert(std::make_pair(sid, hwnd));
	if (hwnd != (HWND)NULL && IsWindow(hwnd)) {
		SidHwndPairList[sid] = hwnd;
		HwndSidPairList.insert(std::make_pair(hwnd, sid));

		increaseSubProcessCount();
	}
}

void
ProcessModuleManager::removesidhwnd(SOCKET sid, HWND hwnd)
{
	if (hwnd != (HWND)NULL) {
		if (SidHwndPairList.find(sid) != SidHwndPairList.end()) {
			SidHwndPairList.erase(sid);
			decreaseSubProcessCount();
		}

		if (HwndSidPairList.find(hwnd) != HwndSidPairList.end()) {
			HwndSidPairList.erase(hwnd);
		}
	}
}

void
ProcessModuleManager::getHwndBySid(SOCKET sid, HWND& hwnd)
{
	if (IsSubProcess) {
		hwnd = DefaultDesHwnd;
	}
	else if (SidHwndPairList.find(sid) != SidHwndPairList.end()) {
		hwnd = SidHwndPairList[sid];
	}

	if (hwnd == HWND(NULL)) {
		for (auto itor = HwndSidPairList.begin();
			itor != HwndSidPairList.end();
			itor++) {
			if (itor->second == sid) {
				hwnd = itor->first;
				break;
			}
		}
	}
}

void
ProcessModuleManager::getSidByHwnd(SOCKET &sid, const HWND& hwnd)
{
	sid = INVALID_SOCKET;

	if (HwndSidPairList.find(hwnd) != HwndSidPairList.end()) {
		sid = HwndSidPairList[hwnd];
	}
}

bool
ProcessModuleManager::init()
{
	if (!IsInited) {
		std::thread t(&ProcessModuleManager::multiUpdate, this);
		if (t.joinable())
			t.detach();
		IsInited = true;
		isActive = true;
	}

	return true;
}

void 
ProcessModuleManager::update(float dt)
{
	//checkSubValidation();
	//adoptSubMsg();
}

void 
ProcessModuleManager::multiUpdate()
{
	while (isActive) {
		checkSubValidation();
		adoptSubMsg();
	}
}

bool
ProcessModuleManager::startNewProcess(SOCKET sid, const char* cmdline)
{
	if (IsSubProcess) return false;

	bool result = true;

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));

	char buff[MAX_PATH] = { 0 };

	std::stringstream ss;
	ss << cmdline;
	ss << " ";
	ss << (long long)SelfHwnd;

	std::string cmd(ss.str());
	memcpy(buff, cmd.c_str(), cmd.length());

	if (!CreateProcess(NULL, 
		buff,
		NULL, 
		NULL,
		false, 
		0,
		NULL, 
		NULL,
		&si, 
		&pi)) {
		result = false;
	}
	else {
		SidHwndPairList.insert(std::make_pair(sid, (HWND)NULL));

		// detach
		//::CloseHandle(pi.hThread);
		//::CloseHandle(pi.hProcess);

		//::PostThreadMessage(pi.dwThreadId, WM_CLOSE, 0, 0);
		//::TerminateProcess(pi.hProcess, 0);
	}

	return result;
}

void
ProcessModuleManager::setGameScriptName(const std::string &name)
{
	GameScriptName = name;
}

void
ProcessModuleManager::getGameScriptName(std::string &name)
{
	name = GameScriptName;
}

void ProcessModuleManager::setSelfSid(SOCKET sid)
{
	SelfSid = sid;
}

SOCKET 
ProcessModuleManager::getSelfSid() const
{
	return SelfSid;
}

void
ProcessModuleManager::fullSidHwnd(SOCKET sid, HWND hwnd)
{
	if (SidHwndPairList.find(sid) != SidHwndPairList.end()) {
		regsterHwndWithSid(sid, hwnd);
	}
}

bool
ProcessModuleManager::onCopyData(HWND srchwnd, COPYDATASTRUCT *data)
{
	bool result = false;

	result = handleCopyData(data);

	return result;
}

bool 
ProcessModuleManager::onSuicide()
{
	if (!IsSubProcess) {
		if (ProcessCtrlSeek) {
			ProcessCtrlSeek->onFalldown();
		}

		for (auto itor = SubReadyList.begin();
			itor != SubReadyList.end();
			itor++) {
			if (itor->first != (HWND)NULL &&
				IsWindow(itor->first)) {
				::SendMessage(itor->first, WM_CLOSE, NULL, NULL);
			}
		}
	}
	else {
		if (SelfHwnd != (HWND)NULL &&
			IsWindow(SelfHwnd)) {
			::SendMessage(SelfHwnd, WM_CLOSE, NULL, NULL);
		}
	}

	return true;
}

bool
ProcessModuleManager::suicide()
{
	if (!IsSubProcess){
		if (SelfHwnd != (HWND)NULL &&
			IsWindow(SelfHwnd)) {
			::SendMessage(SelfHwnd, WM_CLOSE, NULL, NULL);
		}
	}

	return true;
}

/*
des: handle the dispatch issues of ipc data
*/
bool
ProcessModuleManager::handleCopyData(COPYDATASTRUCT *data)
{
	bool result = false;

	switch (data->dwData) {
	case IDT_IMMEDIATE: {
		result = handleImmediateMsg((IPC_DATA *)data->lpData);
		break;
	}
	case IDT_ASYNCHRONISM: {
		result = handleAsychronismMsg((IPC_DATA *)data->lpData);
		break;
	}
	default:
		break;
	}

	return result;
}

bool
ProcessModuleManager::handleImmediateMsg(IPC_DATA *ipcdata)
{
	//std::lock_guard<std::mutex> guard(MsgMutex);

	bool result = false;

	/*if (ProcessCtrlSeek) {*/
		switch (ipcdata->Head.Signal) {
		case IPC_DATA_IDENTIFY:
		{
			if (!isSubProcess()) {
				long long subhll = *(long long*)ipcdata->Buffer;
				HWND subHwnd = (HWND)subhll;
				regsterHwndWithSid(ipcdata->Head.Sid, subHwnd);

				preSubMsgPool(subHwnd);

				long long testsh =
					(long long)ProcessModuleMgr->getSelfHwnd();
				ProcessModuleMgr->sendData(subHwnd, 0, 0,
					(void*)&testsh, sizeof(long long), IPC_DATA_IDENTIFY);

				HWND selfhwnd = ProcessModuleMgr->getSelfHwnd();

				setDefaultDesHwnd(subHwnd);
			}

			break;
		}
		case IPC_DATA_SUBREADY: {
			if (!isSubProcess()) {
				setSubReady(ipcdata->Head.Sid);
				adpotSubUsrID(ipcdata->Head.Sid);
				adoptUsrsInfo(ipcdata->Head.Sid);
			}

			break;
		}
		case IPC_DATA_SELFUSRID: {
			if (ProcessCtrlSeek) {
				size_t usid = *(size_t*)ipcdata->Buffer;
				ProcessCtrlSeek->onSelfUsrID(usid);
			}

			break;
		}
		case IPC_DATA_SUBFALLDOWN: {
			if (!IsSubProcess) {
				HWND target = (HWND)NULL;
				getHwndBySid(ipcdata->Head.Sid, target);
				if (target != (HWND)NULL) {
					removesidhwnd(ipcdata->Head.Sid, target);
					removeSubMsgPool(target);
				}

				// TODO: close socket with sid
				if (ProcessCtrlSeek) ProcessCtrlSeek->onSubFalldown(ipcdata->Head.Sid);
			}
			else {
				if (ProcessCtrlSeek) ProcessCtrlSeek->onFalldown();
			}

			break;
		}
		default:
		{
			if (ProcessCtrlSeek) {
				result = ProcessCtrlSeek->onProcessControlData(isSubProcess(),
					ipcdata->Head.Sid,
					ipcdata->Head.MainCmd,
					ipcdata->Head.SubCmd,
					ipcdata->Buffer,
					ipcdata->Head.PacketSize - sizeof(IPC_HEAD));
			}

			break;
		}
		}
	//}

	return result;
}

bool
ProcessModuleManager::handleAsychronismMsg(IPC_DATA *data)
{
	bool result = false;
	IPCDataList.push_back(data);
	result = true;
	//if (isSubProcess()) {
	//	// up to game logic 
	//}
	//else {
	//	// 
	//}

	return result;
}

bool
ProcessModuleManager::release()
{
	//for (auto itor = SidProcModuleList.begin();
	//	itor != SidProcModuleList.end();
	//	itor++)
	//{
	//	if (itor->second && itor->second->initialized()) {
	//		itor->second->uninitialize();
	//		delete itor->second;
	//		itor->second = nullptr;
	//	}
	//}

	//SidProcModuleList.clear();

	SidHwndPairList.clear();

	HwndSidPairList.clear();

	IPCDataList.clear();

	StupidUsrsInfoPool.clear();

	StupdiSubUsrsIDPool.clear();

	return true;
}

bool
ProcessModuleManager::isSubProcess()
{
	return IsSubProcess;
}

void
ProcessModuleManager::setIsSubProcess(bool value)
{
	IsSubProcess = value;
}

void
ProcessModuleManager::setSelfHwnd(HWND hwnd)
{
	SelfHwnd = hwnd;
}

HWND
ProcessModuleManager::getSelfHwnd() const
{
	return SelfHwnd;
}

void
ProcessModuleManager::setDefaultDesHwnd(HWND hwnd)
{
	DefaultDesHwnd = hwnd;
}

HWND
ProcessModuleManager::getDefaultDesHwnd() const
{
	return DefaultDesHwnd;
}

int
ProcessModuleManager::getSubProcessCount() const
{
	return SubProcessCount;
}

void 
ProcessModuleManager::setSubProcessCount(int value)
{
	SubProcessCount = value > 0 ? value : 0;
}

void 
ProcessModuleManager::increaseSubProcessCount()
{
	setSubProcessCount(SubProcessCount + 1);
}

void 
ProcessModuleManager::decreaseSubProcessCount()
{
	setSubProcessCount(SubProcessCount - 1);
}

bool
ProcessModuleManager::hasSubProcess()
{
	return SubProcessCount > 0;
}

bool
ProcessModuleManager::sendData(HWND target,
	unsigned short maincmd,
	unsigned short subcmd,
	void* const data,
	unsigned short datasize,
	unsigned short signal)
{
	bool result = false;

	if (target && IsWindow(target))
	{
		IPC_DATA *ipcdata = 
			constructIPCData(SelfSid, maincmd, subcmd, data, datasize, signal);
		if (ipcdata) {
			sendData((HWND)target, ipcdata);
			delete ipcdata;
			ipcdata = nullptr;
			result = true;
		}
	}

	return result;
}

bool
ProcessModuleManager::sendCommand(HWND target,
	unsigned short maincmd,
	unsigned short subcmd,
	void *const data,
	unsigned short datasize,
	unsigned short signal)
{
	bool result = false;

	if (target && IsWindow(target))
	{
		IPC_DATA *ipcdata = 
			constructIPCData(SelfSid, maincmd, subcmd, data, datasize, signal);
		sendCommand((HWND)target, ipcdata);
		delete ipcdata;
		ipcdata = nullptr;
		result = true;
	}

	return result;
}

void 
ProcessModuleManager::preSubMsgPool(HWND hwnd)
{
	std::lock_guard<std::mutex> guard(MsgMutex);

	std::vector<IPC_DATA*> ipcdatalist;
	SubMsgPool.insert(std::make_pair(hwnd, ipcdatalist));
}

void 
ProcessModuleManager::removeSubMsgPool(HWND hwnd)
{
	std::lock_guard<std::mutex> guard(MsgMutex);

	if (SubMsgPool.find(hwnd) != SubMsgPool.end()) {
		for (auto itor = SubMsgPool[hwnd].begin();
			itor != SubMsgPool[hwnd].end();) {
			if (*itor) {
				delete *itor;
				*itor = nullptr;
				itor = SubMsgPool[hwnd].erase(itor);
			}
			else {
				itor++;
			}
		}

		SubMsgPool.erase(hwnd);
	}
}

bool 
ProcessModuleManager::isSubReady(HWND hwnd)
{
	bool result = false;
	if (SubReadyList.find(hwnd) != SubReadyList.end()) {
		result = SubReadyList[hwnd];
	}

	return result;
}

void 
ProcessModuleManager::checkSubValidation()
{
	//std::lock_guard<std::mutex> guard(MsgMutex);

	//for (auto submsglist = SubMsgPool.begin();
	//	submsglist != SubMsgPool.end();
	//	submsglist++) {

	//}
}

void 
ProcessModuleManager::adoptSubMsg()
{
	std::lock_guard<std::mutex> guard(MsgMutex);

	for (auto submsglist = SubMsgPool.begin();
		submsglist != SubMsgPool.end();
		submsglist++) {
		if (isSubReady(submsglist->first)) {
			for (vector<IPC_DATA* > ::size_type n = 0;
				n < submsglist->second.size();) {
				if (true) {
					sendData(submsglist->first, submsglist->second[n]); 
					delete submsglist->second[n];
					submsglist->second[n] = nullptr;

					submsglist->second.erase(submsglist->second.begin() + n);
				}
				else
					n++;
			}
		}
	}
}

void 
ProcessModuleManager::houseUsrsInfo(SOCKET sid,
	unsigned short maincmd,
	unsigned short subcmd,
	void *const data,
	unsigned short datasize,
	unsigned short signal)
{
	IPC_DATA* ipcdata =
		constructIPCData(sid, maincmd, subcmd, data, datasize, signal);

	if (ipcdata) {
		StupidUsrsInfoPool.push_back(ipcdata);
		keepUsrsInfoCount();
	}
}

void 
ProcessModuleManager::keepUsrsInfoCount()
{
	for (std::vector<IPC_DATA* > ::size_type n = 0;
		StupidUsrsInfoPool.size() > MaxUsrsInfoCount;) {
		StupidUsrsInfoPool.erase(StupidUsrsInfoPool.begin() + n);
	}
}

void 
ProcessModuleManager::adoptUsrsInfo(HWND target)
{
	if (target != (HWND)NULL && IsWindow(target)) {
		for (vector<IPC_DATA* > ::size_type n = 0;
			n < StupidUsrsInfoPool.size(); n++) {
				sendData(target, StupidUsrsInfoPool[n]);
		}
	}
}

void 
ProcessModuleManager::adoptUsrsInfo(SOCKET sid)
{
	HWND hwnd = (HWND)NULL;
	getHwndBySid(sid, hwnd);
	if (hwnd != (HWND)NULL && IsWindow(hwnd))
	{
		adoptUsrsInfo(hwnd);
	}
}

void 
ProcessModuleManager::houseSubMsg(HWND th, IPC_DATA* data)
{
	std::lock_guard<std::mutex> guard(MsgMutex);

	if (SubMsgPool.find(th) != SubMsgPool.end()) {
		SubMsgPool[th].push_back(data);
	}
}

void 
ProcessModuleManager::setSubReady(HWND hwnd)
{
	if (hwnd != (HWND)NULL) {
		//if (SubReadyList.find(hwnd) != SubReadyList.end()) {
			SubReadyList[hwnd] = true;
		//}
	}
}

void 
ProcessModuleManager::setSubReady(SOCKET sid)
{
	HWND hwnd = HWND(NULL);
	getHwndBySid(sid, hwnd);
	setSubReady(hwnd);
}

void 
ProcessModuleManager::patchIngo2Sub(HWND hwnd)
{

}

bool
ProcessModuleManager::sendData(SOCKET sid,
	unsigned short maincmd,
	unsigned short subcmd,
	void *const data,
	unsigned short datasize,
	unsigned short signal)
{
	bool result = false;

	HWND th = HWND(NULL);
	getHwndBySid(sid, th);

	if (th != (HWND)NULL && IsWindow(th)) {
		IPC_DATA* ipcdata =
			constructIPCData(sid, maincmd, subcmd, data, datasize, signal);

		if (ipcdata) {
			if (isSubReady(th)) {
				sendData(th, ipcdata);
				delete ipcdata;
				ipcdata = nullptr;
			}
			else {
				houseSubMsg(th, ipcdata);
			}
			result = true;
		}
	}
	else {
		if (isFreeMainCMDForSub(maincmd)) {
			houseUsrsInfo(sid, maincmd, subcmd, data, datasize, signal);
		}
	}

	return result;
}

bool
ProcessModuleManager::sendCommand(SOCKET sid,
	unsigned short maincmd,
	unsigned short subcmd,
	void *const data,
	unsigned short datasize,
	unsigned short signal)
{
	bool result = false;

	HWND th = HWND(NULL);
	getHwndBySid(sid, th);

	if (th != (HWND)NULL && IsWindow(th)) {
		IPC_DATA* ipcdata =
			constructIPCData(sid, maincmd, subcmd, data, datasize, signal);
		if (ipcdata) {
			if (isSubReady(th)) {
				sendCommand(th, ipcdata);
				delete ipcdata;
				ipcdata = nullptr;
			}
			else {
				houseSubMsg(th, ipcdata);
			}

			result = true;
		}
	}

	return result;
}

IPC_DATA*
ProcessModuleManager::constructIPCData(SOCKET sid,
	unsigned short maincmd,
	unsigned short subcmd,
	void *data,
	unsigned short datasize,
	unsigned short signal)
{
	IPC_DATA *ipcdata = new IPC_DATA;

	ipcdata->Head.Version = IPC_VER;
	ipcdata->Head.MainCmd = maincmd;
	ipcdata->Head.SubCmd = subcmd;
	ipcdata->Head.Sid = sid;
	ipcdata->Head.PacketSize = datasize + sizeof(IPC_HEAD);
	ipcdata->Head.Signal = signal;

	if (datasize) {
		memcpy(ipcdata->Buffer, data, datasize);
	}

	return ipcdata;
}

void
ProcessModuleManager::sendData(HWND target, IPC_DATA *data)
{
	int numer = 1;
	COPYDATASTRUCT CopyDataStruct;
	CopyDataStruct.lpData = (void*)data;
	CopyDataStruct.dwData = IDT_IMMEDIATE;
	CopyDataStruct.cbData = data->Head.PacketSize;
	::SendMessage((HWND)target, WM_COPYDATA,
		(WPARAM)SelfHwnd, (LPARAM)&CopyDataStruct);
}

void
ProcessModuleManager::sendCommand(HWND target, IPC_DATA *data)
{
	COPYDATASTRUCT CopyDataStruct;
	long long llshwnd = (long long)SelfHwnd;
	CopyDataStruct.lpData = (void*)data;
	CopyDataStruct.dwData = IDT_ASYNCHRONISM;
	CopyDataStruct.cbData = data->Head.PacketSize;
	::SendMessage((HWND)target, WM_COPYDATA,
		(LPARAM)SelfHwnd, (WPARAM)&CopyDataStruct);
}

void
ProcessModuleManager::registerSeek(IProcessControlSeek *seek)
{
	ProcessCtrlSeek = seek;
}

void 
ProcessModuleManager::registerSubUsrID(size_t usrid)
{
	StupdiSubUsrsIDPool.push_back(usrid);
}

void 
ProcessModuleManager::adpotSubUsrID(SOCKET sid)
{
	for (std::vector<size_t>::size_type n = 0;
		n < StupdiSubUsrsIDPool.size(); n++) {
		sendData(sid, 0, 0, &StupdiSubUsrsIDPool[n], sizeof(size_t), IPC_DATA_SELFUSRID);
	}
}

void 
ProcessModuleManager::pushFreeMainCMDForSub(int value)
{
	FreeMainCMDsForSub.push_back(value);
}

bool 
ProcessModuleManager::isFreeMainCMDForSub(int value)
{
	bool result = false;

	for (auto itor = FreeMainCMDsForSub.begin();
		itor != FreeMainCMDsForSub.end();
		itor++) {
		if (*itor == value) {
			result = true;
			break;
		}
	}
	
	return result;
}