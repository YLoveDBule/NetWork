#include <windows.h>
#include "ProcessModule.h"



ProcessModule::ProcessModule()
{

}

ProcessModule::~ProcessModule()
{

}

bool ProcessModule::msgnotify()
{
	return false;
}

bool ProcessModule::testcreatep(const char* proname)
{
	bool result = true;

	STARTUPINFO si;
	PROCESS_INFORMATION pi;

	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	ZeroMemory(&pi, sizeof(pi));
	//DXSDK_Jun10.exe
	
	char buff[256] = {0};
	memcpy(buff, proname, strlen(proname));

	if (!CreateProcess(NULL, buff, NULL, NULL, false, 0, NULL, NULL, &si, &pi))
	{
		result = false;
	}

	return result;
}

HWND ProcessModule::getProcessHwnd()
{
	return SelfHwnd;
}

bool ProcessModule::setProcessSink(IProcessControlSeek* seek)
{
	ProcessCtrlSeek = seek;
	return true;
}

bool ProcessModule::initialize()
{
	Initialized = true;

	return true;
}

bool ProcessModule::uninitialize()
{
	return true;
}

bool ProcessModule::initialized()
{
	return Initialized;
}

bool ProcessModule::sendData(HWND target, WORD maincmd, WORD subcmd)
{
	return true;
}

bool ProcessModule::sendData(HWND target, WORD maincmd, WORD subcmd, VOID* const data, WORD datasize)
{
	return true;
}

bool ProcessModule::sendCommand(HWND target, WORD maincmd, WORD subcmd, VOID* const data, WORD datasize)
{
	return true;
}

bool ProcessModule::onCopyData(COPYDATASTRUCT *data)
{
	return true;
}
