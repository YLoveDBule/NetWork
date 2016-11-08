#pragma once

#include <map>
#include <vector>
#include <queue>
#include <mutex>
#include <Windows.h>

#include "ProcessData.h"

class IProcessControlSeek
{
public:
	virtual bool onProcessControlData(bool issub, 
		SOCKET sid, 
		unsigned short wMainCmdID,
		unsigned short wSubCmdID,
		void *pData, 
		unsigned short wDataSize) = NULL;

	virtual bool onSelfUsrID(size_t id) = NULL;

	virtual bool onSubFalldown(SOCKET sid) = NULL;

	virtual bool onFalldown() = NULL;
};

#pragma warning( disable: 4251)

class ProcessModule;

using namespace std;



class __declspec(dllexport) ProcessModuleManager
{
public:
	~ProcessModuleManager();

	static ProcessModuleManager* getInstance();

public:
	bool init();

	void update(float);

	void multiUpdate();

	bool startNewProcess(SOCKET sid, const char* cmdline);

	void setGameScriptName(const std::string &name);

	void getGameScriptName(std::string &name);

	void setSelfSid(SOCKET sid);

	SOCKET getSelfSid() const;

	void registerSeek(IProcessControlSeek *);

public:
	bool sendData(HWND target, 
		unsigned short maincmd, 
		unsigned short subcmd, 
		void* const data = NULL, 
		unsigned short datasize = 0,
		unsigned short signal = 0);

	bool sendCommand(HWND target, 
		unsigned short maincmd, 
		unsigned short subcmd, 
		void* const data = 0, 
		unsigned short datasize = 0,
		unsigned short signal = 0);

	bool sendData(SOCKET sid, 
		unsigned short maincmd, 
		unsigned short subcmd, 
		void* const data = NULL, 
		unsigned short datasize = 0,
		unsigned short signal = 0);

	bool sendCommand(SOCKET sid, 
		unsigned short maincmd, 
		unsigned short subcmd, 
		void* const data, 
		unsigned short datasize,
		unsigned short signal = 0);

public:
	void regsterHwndWithSid(SOCKET sid, HWND hwnd);

	void removesidhwnd(SOCKET sid, HWND hwnd);

	void getHwndBySid(SOCKET sid, HWND& hwnd);

	void getSidByHwnd(SOCKET &sid, const HWND& hwnd);

	bool onCopyData(HWND srchwnd, COPYDATASTRUCT *data);

	bool onSuicide();

	bool suicide();

	bool release();

	bool isSubProcess();

	void setIsSubProcess(bool value);

	void setSelfHwnd(HWND hwnd);

	HWND getSelfHwnd() const;

	void setDefaultDesHwnd(HWND hwnd);

	HWND getDefaultDesHwnd() const;

	void registerSubUsrID(size_t usrid);

public:
	int getSubProcessCount() const;

	void setSubProcessCount(int value);

	bool hasSubProcess();

	void pushFreeMainCMDForSub(int);

private:
	void increaseSubProcessCount();

	void decreaseSubProcessCount();

protected:
	void fullSidHwnd(SOCKET sid, HWND hwnd);

	bool addProcessModule(SOCKET sid, HWND hwnd);

	// entry of message from processes
	bool handleCopyData(COPYDATASTRUCT *data);

	bool handleImmediateMsg(IPC_DATA *);

	bool handleAsychronismMsg(IPC_DATA *);

private:
	ProcessModuleManager();

	ProcessModuleManager(const ProcessModuleManager&);

	ProcessModuleManager& operator=(const ProcessModuleManager&);

private:
	IPC_DATA* constructIPCData(SOCKET sid,
		unsigned short maincmd, 
		unsigned short subcmd,
		void* data = NULL, 
		unsigned short datasize = 0,
		unsigned short signal = 0);

	// send process data to target windows with hwnd as target.
	void sendData(HWND target, IPC_DATA *);

	// send process command to target windows with hwnd as target.
	void sendCommand(HWND target, IPC_DATA *);

	// forward construct the message pool for a process with target hwnd.
	void preSubMsgPool(HWND);

	//Remove the pooled msg for sub process.
	void removeSubMsgPool(HWND);

	//house the msg sent to sub process while sub process not ready.
	void houseSubMsg(HWND th, IPC_DATA*);

	void setSubReady(HWND);

	void setSubReady(SOCKET sid);

	bool isSubReady(HWND);

	void patchIngo2Sub(HWND);

	void checkSubValidation();

	// adopt pooled messages to relative sub process which is ready.
	void adoptSubMsg();

private:
	void houseUsrsInfo(SOCKET sid,
		unsigned short maincmd,
		unsigned short subcmd,
		void *const data,
		unsigned short datasize,
		unsigned short signal);

	void keepUsrsInfoCount();

	void adoptUsrsInfo(HWND target);

	void adoptUsrsInfo(SOCKET sid);

	void adpotSubUsrID(SOCKET sid);

	bool isFreeMainCMDForSub(int);

private:
	static ProcessModuleManager* Instance;

	bool IsInited = false;

	bool isActive = false;

	bool IsSubProcess = false;

	int SubProcessCount = 0;

	HWND SelfHwnd = NULL;

	HWND DefaultDesHwnd = NULL;

	SOCKET SelfSid = INVALID_SOCKET;

	IProcessControlSeek *ProcessCtrlSeek = nullptr;

	std::mutex MsgMutex;

	std::string GameScriptName;

	//map<HWND, HWND> ProcHwndPairsList;
	
	map<SOCKET, HWND> SidHwndPairList;

	map<HWND, SOCKET> HwndSidPairList;

	//map<SOCKET, ProcessModule*> SidProcModuleList;

	map<HWND, bool> SubReadyList;

	map<HWND, vector<IPC_DATA*>> SubMsgPool;

	//////////////////////////////////////////////////////////////////////////
	std::vector<IPC_DATA*> IPCDataList;

	//////////////////////////////////////////////////////////////////////////
	std::vector<IPC_DATA*> StupidUsrsInfoPool;

	size_t MaxUsrsInfoCount = 128;

	std::vector<size_t> StupdiSubUsrsIDPool;

	std::vector<int> FreeMainCMDsForSub;

};

#define ProcessModuleMgr ProcessModuleManager::getInstance()