#pragma once
#include <fstream>
#include <map>
#include <string.h>
#include "FmTimer.h"
using namespace std;

class FmLog
{
protected:
	std::ofstream	m_File;
	bool			m_AddTimestamp;	// ʹ��ʱ���
	bool			m_DebugPrint;	// debugģʽ,print
	bool			m_HasCache;
	TickCounter		m_UpdateTimer;
public:
	FmLog(const string& fileName, int openMode = ios::out, bool addTimestamp = true, bool debugPrint = true);
	~FmLog();

	void Write(const char* msg);

	void Update(uint delta);
};

class FmLogMgr
{
protected:
	typedef map<string, FmLog*>	LogMap;
	LogMap	m_Logs;
	FmLog*	m_DefaultLog;

	static bool s_UseCache;					// �Ƿ�ʹ�û���
	static const int s_MaxLogLen = 16 * 2048;	// ������־��󳤶�
public:
	FmLogMgr();
	~FmLogMgr();

	SINGLETON_MODE(FmLogMgr);

	static bool IsUseCache() { return s_UseCache; }
	static void SetUseCache(bool useCache) { s_UseCache = useCache; }

	void AddLog(const string& fileName, int openMode = ios::out, bool addTimestamp = true, bool debugPrint = true);

	void LogFile(const string& fileName, const char* format, ...);

	void LogMsg(const char* format, ...);

	void SetDefaultLog(const string& fileName);

	void Update(uint delta);

	// ֱ��д��־
	static void LogCFile(const string& fileName, const string& buf);
	static void LogCFileEx(const string& fileName, const char* format, ...);
};

#ifdef COCOS2D_DEBUG
#define DbgLog( ... ) FmLogMgr::GetInstance().LogMsg( __VA_ARGS__ )
#else
#define DbgLog( ... )
#endif

#define FmGameLog( ... ) FmLogMgr::GetInstance().LogMsg( __VA_ARGS__ )
