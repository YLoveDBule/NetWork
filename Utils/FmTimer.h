#pragma once
#include "cocos2d.h"
#include "cocos/platform/CCPlatformMacros.h"
typedef unsigned int uint;
// ����ģʽ
#define SINGLETON_MODE( ClassName ) \
	static ClassName& GetInstance() \
	{\
		static ClassName s_##ClassName;\
		return s_##ClassName;\
	}


// ���Ժ�
#ifdef COCOS2D_DEBUG
#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#ifdef UNICODE
#define Assert(_Expression)\
				if( !(_Expression) )\
			{\
				printf( "ASSERT:%s", #_Expression );\
				printf( " (%s:%d)\n", __FILE__, __LINE__ );\
				char buf[1024*4] = { NULL };\
				printf( buf, "ASSERT:%s (%s:%d)\n", #_Expression, __FILE__, __LINE__ );\
				WCHAR wszBuf[1024*4] = {NULL};\
				MultiByteToWideChar(CP_ACP, 0, buf, -1, wszBuf, sizeof(wszBuf));\
				OutputDebugStringW( wszBuf );\
			}
#else
#define Assert(_Expression)\
				if( !(_Expression) )\
			{\
				printf( "ASSERT:%s", #_Expression );\
				printf( " (%s:%d)\n", __FILE__, __LINE__ );\
				char buf[1024*4] = { NULL };\
				printf( buf, "ASSERT:%s (%s:%d)\n", #_Expression, __FILE__, __LINE__ );\
				OutputDebugString( buf );\
			}
#endif //#ifdef UNICODE
#else
#define Assert(_Expression)\
			if( !(_Expression) )\
		{\
			printf( "ASSERT:%s", #_Expression );\
			printf( " (%s:%d)\n", __FILE__, __LINE__ );\
		}
#endif // // #if FM_PLATFORM == FM_PLATFORM_WIN32
#else
#define Assert(_Expression) ((void)0)
#endif // #ifdef _DEBUG
class /*FmUtil_EXPORTS*/ TickCounter
{
public:
	uint	m_RunningTick;	// ����ʱ��
	uint	m_ExpiredTick;	// ����ʱ��,Ϊ0ʱ,��ʾ��������

public:
	TickCounter()
	{
		m_RunningTick = m_ExpiredTick = 0;
	}

	// ����
	void Reset(uint expiredTick)
	{
		m_RunningTick = 0;
		m_ExpiredTick = expiredTick;
	}

	// ����ʱ��
	void Update(uint delta)
	{
		m_RunningTick += delta;
	}

	// �Ƿ����
	bool IsExpired()
	{
		if (m_ExpiredTick == 0)
			return false;
		return (m_RunningTick >= m_ExpiredTick);
	}

	// �Ƿ����,�������������ʱ��
	bool IsExpiredAndResetTick()
	{
		if (m_ExpiredTick == 0)
			return false;
		bool isExpire = false;
		if (m_RunningTick >= m_ExpiredTick)
		{
			isExpire = true;
			m_RunningTick -= m_ExpiredTick;
		}
		return isExpire;
	}

	// �Ƿ����,�������������ʱ��Ϊ0
	bool IsExpiredAndResetZero()
	{
		if (m_ExpiredTick == 0)
			return false;
		bool isExpire = false;
		if (m_RunningTick >= m_ExpiredTick)
		{
			isExpire = true;
			m_RunningTick = 0;
		}
		return isExpire;
	}
};
