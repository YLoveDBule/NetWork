/*
Des: handle msg adopted from process.
*/

#pragma once

#include "ProcessModuleManager.h"



class WinProcess : public IProcessControlSeek
{
public:
	static WinProcess* getInstance();

	bool init();

public:
	virtual bool onProcessControlData(bool issub, 
		SOCKET sid,
		unsigned short wMainCmdID,
		unsigned short wSubCmdID,
		void *pData, 
		unsigned short wDataSize);

	virtual bool onSelfUsrID(size_t id);

	virtual bool onSubFalldown(SOCKET sid);

	virtual bool onFalldown();

private:
	static WinProcess *Instance;

private:
	WinProcess();
	~WinProcess();
	WinProcess(const WinProcess&);

private:
	void suicide();

};