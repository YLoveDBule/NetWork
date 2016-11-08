#include "scripting/lua-bindings/manual/CCLuaEngine.h"
#include "WinProcess.h"
#include "CJNetworkManager.h"
#include "Messager.h"

#include <windows.h>
#include <sstream>



WinProcess *WinProcess::Instance = nullptr;

WinProcess* WinProcess::getInstance()
{
	if (Instance == nullptr) {
		Instance = new WinProcess;
	}

	return Instance;
}

WinProcess::WinProcess()
{

}

WinProcess::~WinProcess()
{

}

WinProcess::WinProcess(const WinProcess&)
{

}

bool 
WinProcess::init()
{
	bool result = false;

	ProcessModuleMgr->registerSeek(this);

	if (!ProcessModuleMgr->isSubProcess()) {
		LPWSTR *szArglist;
		int nArgs = 0;
		szArglist = CommandLineToArgvW(GetCommandLine(), &nArgs);

		if (NULL == szArglist) {
			wprintf(L"CommandLineToArgvW failed\n");
		}
		else if (nArgs >= 3) {
			for (int i = 0; i < nArgs; i++) {
				LPWSTR str = szArglist[i];
				str = str;
			}

			LPWSTR luaname = szArglist[nArgs - 3];
			LPWSTR sid = szArglist[nArgs - 2];
			LPWSTR srcHwnd = szArglist[nArgs - 1];

			char buff[512] = { 0 };
			size_t ret = wcstombs(buff, luaname, sizeof(buff));
			if (ret != (size_t)-1 && strlen(buff)) {
				buff[strlen(buff)] = '\0';
				std::string luafilename(buff);

				if (luafilename.find(".lua") != std::string::npos) {
					ProcessModuleMgr->setIsSubProcess(true);
					ProcessModuleMgr->setGameScriptName(luafilename);

					memset(buff, 0, sizeof(buff));
					ret = wcstombs(buff, sid, sizeof(buff));
					if (ret != (size_t)-1 && strlen(buff)) {
						buff[strlen(buff)] = '\0';
						std::string sidstr(buff);
						SOCKET sid = INVALID_SOCKET;
						std::stringstream ss;
						ss << sidstr;
						ss >> sid;

						std::string selfhwndstr;
						ss.clear();
						ss << ProcessModuleMgr->getSelfHwnd();
						ss >> selfhwndstr;

						ss.clear();
						long long pmsh = (long long)ProcessModuleMgr->getSelfHwnd();
						ss << (long long)pmsh;
						ss >> selfhwndstr;

						if (sid != INVALID_SOCKET) {
							ProcessModuleMgr->setSelfSid(sid);
							ProcessModuleMgr->regsterHwndWithSid(sid,
								ProcessModuleMgr->getSelfHwnd());
						}
						else {
						}
					}

					memset(buff, 0, sizeof(buff));
					ret = wcstombs(buff, srcHwnd, sizeof(buff));
					if (ret != (size_t)-1 && strlen(buff)) {
						buff[strlen(buff)] = '\0';
						std::string hwndstr(buff);
						long long llhwnd = 0;
						HWND srchwnd;
						std::stringstream ss;
						ss << hwndstr;
						ss >> llhwnd;
						srchwnd = (HWND)llhwnd;

						std::string srchwndstr;
						ss.clear();
						ss << srchwnd;
						ss >> srchwndstr;

						ProcessModuleMgr->setIsSubProcess(true);

						ProcessModuleMgr->setDefaultDesHwnd(srchwnd);

						HWND selfhwnd = ProcessModuleMgr->getSelfHwnd();
						long long testsh = (long long)ProcessModuleMgr->getSelfHwnd();
						ProcessModuleMgr->sendData(srchwnd, 0, 0,
							(void*)&testsh, sizeof(long long), IPC_DATA_IDENTIFY);

						result = true;
					}
				}
			}
		}

		LocalFree(szArglist);
	}

	return result;
}

bool 
WinProcess::onProcessControlData(bool issub, 
	SOCKET sid,
	unsigned short wMainCmdID,
	unsigned short wSubCmdID,
	void *pData, 
	unsigned short wDataSize)
{
	bool result = false;
	if (issub) {
		result = YourMessager->send2Lua(sid, wMainCmdID, wSubCmdID, pData, wDataSize);
	}
	else {
		result = CJNetworkMgr->sendMessage(sid, wMainCmdID, wSubCmdID, pData, wDataSize);
	}

	return result;
}

bool 
WinProcess::onSelfUsrID(size_t id)
{
	bool result = YourMessager->sendSelfUsrID(id);

	return result;
}

bool 
WinProcess::onSubFalldown(SOCKET sid)
{
	YourMessager->closeConnect(sid);

	return true;
}

bool 
WinProcess::onFalldown()
{
	suicide();
	return true;
}

void 
WinProcess::suicide()
{
	YourMessager->closeAllConnect();
}