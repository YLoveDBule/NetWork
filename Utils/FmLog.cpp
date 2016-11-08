#include "FmLog.h"
#include <iomanip>

bool FmLogMgr::s_UseCache = false;

FmLog::FmLog(const string& fileName, int openMode/*=ios::out*/, bool addTimestamp/*=true*/, bool debugPrint/*=true*/)
{
	m_File.open(fileName.c_str(), (std::ios_base::openmode)openMode);
	m_AddTimestamp = addTimestamp;
	m_DebugPrint = debugPrint;
	m_HasCache = false;
#ifdef COCOS2D_DEBUG
	m_UpdateTimer.Reset(3 * 1000); // 3秒记一次日志
#else
	m_UpdateTimer.Reset(30 * 1000); // 30秒记一次日志
#endif
}

FmLog::~FmLog()
{
	m_File.close();
}

void FmLog::Write(const char* msg)
{
	if (!m_File.is_open())
	{
		Assert(false);
		return;
	}
	if (msg == NULL)
	{
		return;
	}

	if (m_DebugPrint)
	{
#ifdef COCOS2D_DEBUG
		printf("%s\n", msg);
#endif
	}

	if (m_AddTimestamp)
	{
		time_t ctTime;
		time(&ctTime);
		struct tm* pTime = localtime(&ctTime);
		if (pTime)
		{
			m_File
				<< std::setw(4) << std::setfill('0') << pTime->tm_year + 1900
				<< std::setw(2) << std::setfill('0') << pTime->tm_mon + 1
				<< std::setw(2) << std::setfill('0') << pTime->tm_mday
				<< " " << std::setw(2) << std::setfill('0') << pTime->tm_hour
				<< ":" << std::setw(2) << std::setfill('0') << pTime->tm_min
				<< ":" << std::setw(2) << std::setfill('0') << pTime->tm_sec
				<< ": ";
		}
	}

	m_File << msg << std::endl;

	if (!FmLogMgr::IsUseCache())
		m_File.flush();
	else
		m_HasCache = true;
}

void FmLog::Update(uint delta)
{
	m_UpdateTimer.Update(delta);
	if (m_UpdateTimer.IsExpiredAndResetZero() && m_HasCache)
	{
		m_File.flush();
		m_HasCache = false;
	}
}

FmLogMgr::FmLogMgr()
{
	m_DefaultLog = NULL;
	AddLog("Freeman.log");
}
FmLogMgr::~FmLogMgr()
{
	Update(1000 * 60);
	for (LogMap::iterator it = m_Logs.begin(); it != m_Logs.end(); ++it)
	{
		delete it->second;
	}
	m_Logs.clear();
}

void FmLogMgr::AddLog(const string& fileName, int openMode/*=ios::out*/, bool addTimestamp/*=true*/, bool debugPrint/*=true*/)
{
	m_Logs[fileName] = new FmLog(fileName, openMode, addTimestamp, debugPrint);
	if (m_Logs.size() == 1)
	{
		m_DefaultLog = m_Logs[fileName];
	}
}

void FmLogMgr::LogFile(const string& fileName, const char* format, ...)
{
	LogMap::iterator it = m_Logs.find(fileName);
	if (it == m_Logs.end())
	{
		return;
	}
	char buf[s_MaxLogLen] = { 0 };

	va_list args;
	va_start(args, format);
	vsprintf(buf, format, args);
	va_end(args);

	it->second->Write(buf);
}

void FmLogMgr::LogMsg(const char* format, ...)
{
	if (m_DefaultLog == NULL)
		return;
	char buf[s_MaxLogLen] = { 0 };

	va_list args;
	va_start(args, format);
	vsprintf(buf, format, args);
	va_end(args);

	m_DefaultLog->Write(buf);
}

void FmLogMgr::SetDefaultLog(const string& fileName)
{
	LogMap::iterator it = m_Logs.find(fileName);
	if (it == m_Logs.end())
	{
		return;
	}
	m_DefaultLog = it->second;
}

void FmLogMgr::Update(uint delta)
{
	for (LogMap::iterator it = m_Logs.begin(); it != m_Logs.end(); ++it)
	{
		it->second->Update(delta);
	}
}

void FmLogMgr::LogCFile(const string& fileName, const string& buf)
{
	FILE* fp = fopen(fileName.c_str(), "a+");
	if (fp == NULL)
	{
		return;
	}
	string logStr;
	time_t ctTime;
	time(&ctTime);
	struct tm* pTime = localtime(&ctTime);
	if (pTime)
	{
		char dateStr[256] = { NULL };
		sprintf(dateStr, "%d%02d%02d %02d:%02d:%02d:",
			pTime->tm_year + 1900,
			pTime->tm_mon + 1,
			pTime->tm_mday,
			pTime->tm_hour,
			pTime->tm_min,
			pTime->tm_sec);
		logStr = dateStr;
	}
	logStr += buf;
	logStr += "\n";
	fwrite(logStr.c_str(), 1, logStr.length(), fp);
	fclose(fp);
}

void FmLogMgr::LogCFileEx(const string& fileName, const char* format, ...)
{
	char buf[s_MaxLogLen] = { 0 };

	va_list args;
	va_start(args, format);
	vsprintf(buf, format, args);
	va_end(args);

	LogCFile(fileName, buf);
}


