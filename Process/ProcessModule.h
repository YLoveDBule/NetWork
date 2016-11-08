#pragma once

#include "Packet.h"

#define IDT_IMMEDIATE			0x0101
#define IDT_ASYNCHRONISM		0x0201

#define IPC_SOCKET_DATA         0x0301
#define IPC_COPY_DATA			0x0401

class IProcessControlSeek;/*
{
public:
	virtual bool onProcessControlData(WORD wMainCmdID, WORD wSubCmdID, VOID *pData, WORD wDataSize, HWND hWndSend) = NULL;
};*/


class __declspec(dllexport) ProcessModule
{
public:
	ProcessModule();
	~ProcessModule();

	bool msgnotify();
	bool testcreatep(const char* proname);

public:
	VOID release() { delete this; }

	VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

public:
	HWND getProcessHwnd();

	bool setProcessSink(IProcessControlSeek*);

public:
	bool initialize();
	bool uninitialize();

	bool initialized();

	bool isConnected();

public:
	bool sendData(HWND target, WORD maincmd, WORD subcmd);

	bool sendData(HWND target, WORD maincmd, WORD subcmd, VOID* const data = NULL, WORD datasize = 0);

	bool sendCommand(HWND target, WORD maincmd, WORD subcmd, VOID* const data, WORD datasize);

public:
	bool onCopyData(COPYDATASTRUCT *data);

	LRESULT OnMessageAsynchronismData(WPARAM wParam, LPARAM lParam);

private:
	bool Initialized = false;
	HWND SelfHwnd = NULL;
	HWND TargetHwnd = NULL;

protected:
	IProcessControlSeek *ProcessCtrlSeek = nullptr;

};