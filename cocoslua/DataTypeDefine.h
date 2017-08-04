#ifndef _LUA_DATATYPE_H_
#define _LUA_DATATYPE_H_
#include "platform/CCPlatformConfig.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef __cplusplus
extern "C" {
#endif

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS || CC_TARGET_PLATFORM == CC_PLATFORM_MAC

#define TRUE	1
#define FALSE	0

#if __LP64__ || (TARGET_OS_EMBEDDED && !TARGET_OS_IPHONE) || TARGET_OS_WIN32 || NS_BUILD_32_LIKE_64
	typedef void                VOID;
	typedef unsigned char		BYTE;
	typedef unsigned short		WORD;
	typedef short               SHORT;
	typedef unsigned int		DWORD;
	typedef	long long			LONGLONG;
	typedef unsigned short		TCHAR;
	typedef long long			SCORE;
	typedef double              DOUBLE;
	typedef int                 LONG;
	typedef int                 INT;
	typedef int                 INT_PTR;
	typedef unsigned int        UINT;
	typedef DWORD               COLORREF;
#else
	typedef void                VOID;
	typedef unsigned char		BYTE;
	typedef unsigned short		WORD;
	typedef short               SHORT;
	typedef unsigned long		DWORD;
	typedef	long long			LONGLONG;
	typedef unsigned short		TCHAR;
	typedef long long			SCORE;
	typedef double              DOUBLE;
	typedef long                LONG;
	typedef int                 INT;
	typedef int                 INT_PTR;
	typedef unsigned int        UINT;
	typedef DWORD               COLORREF;
#endif

#else
#ifndef WIN32
	typedef void VOID;
	typedef unsigned char BYTE;
	typedef unsigned short WORD;
	typedef short  SHORT;
	typedef int BOOL;
	typedef unsigned int DWORD;
	typedef double DOUBLE;
	typedef float FLOAT;
	typedef long long SCORE;
	typedef long long __time64_t;
	typedef unsigned short TCHAR, WCHAR;
	typedef long LONG;
	typedef DWORD COLORREF;
	typedef unsigned int UINT;

#ifdef __x86_64__
	typedef long long INT_PTR;
#else
	typedef int INT_PTR;
#endif

#define TRUE	1
#define FALSE	0
#endif
#endif

#ifndef NULL
#ifdef __cplusplus
#define NULL    0
#else
#define NULL    ((void *)0)
#endif
#endif

#ifdef WIN32
#define FILE_SEPARATOR "\\"
#else
#define FILE_SEPARATOR "/"
#endif

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifdef WIN32
#include <winsock2.h>
	inline void sleep_ms(int ms)
	{
		Sleep(ms * 1000);
	}


#else
#include <unistd.h>

	inline void sleep_ms(int ms)
	{
		usleep(ms * 1000 * 1000);
	}
#endif

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef SAFE_DELETE
#define SAFE_DELETE(p) { if(p) { delete (p); (p)=NULL; } }
#endif
#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p); (p)=NULL; } }
#endif
#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p) { if(p) { (p)->Release(); (p)=NULL; } }
#endif

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef WIN32
#define _atoi64(val)     						strtoll(val, NULL, 10)
#endif

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static DWORD m_random = 0;

#ifdef WIN32
#include <windows.h>
#else
#include <pthread.h>
#endif
	inline DWORD PthreadSelf()
	{
#ifdef WIN32
		return GetCurrentThreadId();
#else
		return m_random++;
#endif
	}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef interface
#define interface struct
#endif

#define		FACE_CX									48									//ͷ����
#define		FACE_CY									48									//ͷ��߶�

#define 	LEN_LESS_ACCOUNTS						6									//����ʺ�
#define 	LEN_LESS_NICKNAME						6									//����ǳ�
#define 	LEN_LESS_PASSWORD						6									//�������
#define 	LEN_LESS_COMPELLATION					4									//��С����

#define 	MAX_CHAIR								100									//�������
#define 	MAX_TABLE								512									//�������
#define 	MAX_COLUMN								32									//����б�
#define 	MAX_BATCH								48									//�������
#define	 	MAX_ANDROID								256									//������
#define 	MAX_PROPERTY							128									//������
#define 	MAX_WHISPER_USER						16									//���˽��
#define 	MAX_CHAIR_GENERAL						8									//�������

#define 	MAX_KIND								256									//�������
#define 	MAX_SERVER								1024								//��󷿼�

#define 	INVALID_CHAIR							0xFFFF								//��Ч����
#define 	INVALID_TABLE							0xFFFF								//��Ч����
#define 	INVALID_USERID							0									//��Ч�û�

#define 	REVENUE_BENCHMARK						1000L							    //˰�����
#define 	REVENUE_DENOMINATOR						1000L								//˰�շ�ĸ

#define 	GAME_STATUS_FREE						0									//����״̬
#define 	GAME_STATUS_PLAY						100									//��Ϸ״̬
#define 	GAME_STATUS_WAIT						200									//�ȴ�״̬
#define 	GAME_STATUS_ENDED   					255                     			//����״̬

#define 	LEN_USER_CHAT							128									//���쳤��
#define 	TIME_USER_CHAT							1L									//������
#define 	TRUMPET_MAX_CHAR            			128									//���ȳ���

#define 	PRIME_TYPE								11L									//������Ŀ
#define 	PRIME_KIND								53L									//������Ŀ
#define 	PRIME_NODE								101L								//�ڵ���Ŀ
#define 	PRIME_PAGE								53L									//�Զ���Ŀ
#define 	PRIME_SERVER							1009L								//������Ŀ

#define 	PRIME_SERVER_USER						503L								//��������
#define 	PRIME_ANDROID_USER						503L								//��������
#define 	PRIME_PLATFORM_USER						100003L								//ƽ̨����

#define 	PRIME_ANDROID_PARAMETER					65									//������Ŀ

#define 	LEN_USER_UIN        					33									//ƽ̨����
#define 	LEN_MD5									33									//��������
#define 	LEN_USERNOTE							32									//��ע����
#define 	LEN_ACCOUNTS							32									//�ʺų���
#define 	LEN_NICKNAME							32									//�ǳƳ���
#define 	LEN_PASSWORD							33									//���볤��
#define 	LEN_GROUP_NAME							32									//��������
#define 	LEN_UNDER_WRITE							32									//����ǩ��

#define 	LEN_QQ									16									//QQ����
#define 	LEN_EMAIL								33									//�����ʼ�
#define 	LEN_USER_NOTE							256									//�û���ע
#define 	LEN_SEAT_PHONE							33									//�̶��绰
#define 	LEN_MOBILE_PHONE						16									//�ƶ��绰
#define 	LEN_PASS_PORT_ID						19									//֤������
#define 	LEN_COMPELLATION						16									//��ʵ����
#define 	LEN_DWELLING_PLACE						128									//��ϵ��ַ
#define 	LEN_WEEK								7									//���ڳ���
#define 	LEN_TASK_NAME							64									//��������
#define 	LEN_TRANS_REMARK						32									//ת�˱�ע

#define 	LEN_NETWORK_ID							13									//��������
#define 	LEN_MACHINE_ID							33									//���г���

#define 	LEN_TYPE								32									//���೤��
#define 	LEN_KIND								32									//���ͳ���
#define 	LEN_NODE								32									//�ڵ㳤��
#define 	LEN_PAGE								32									//���Ƴ���
#define 	LEN_SERVER								32									//���䳤��
#define 	LEN_PROCESS								32									//���̳���

#define		CP_NORMAL								0									//δ֪��ϵ
#define		CP_FRIEND								1									//���ѹ�ϵ
#define		CP_DETEST								2									//����ϵ
#define 	CP_SHIELD								3									//��������

#define 	GENDER_FEMALE							0									//Ů���Ա�
#define 	GENDER_MANKIND							1									//�����Ա�

#define 	GAME_GENER_DEFAULT  					0x0000
#define		GAME_GENRE_GOLD							0x0001								//�������
#define		GAME_GENRE_SCORE						0x0002								//��ֵ����
#define		GAME_GENRE_MATCH						0x0004								//��������
#define		GAME_GENRE_EDUCATE						0x0008								//ѵ������

#define 	SERVER_GENRE_NORMAL						0x0001                              //��ͨ����
#define 	SERVER_GENRE_PASSWD         			0x0002								//��������

#define 	SCORE_GENRE_NORMAL						0x0100								//��ͨģʽ
#define 	SCORE_GENRE_POSITIVE					0x0200								//�Ǹ�ģʽ

#define 	TASK_TYPE_WIN_INNINGS  					0x01								//Ӯ������
#define 	TASK_TYPE_SUM_INNINGS  					0x02								//�ܾ�����
#define 	TASK_TYPE_FIRST_WIN         			0x04								//��ʤ����
#define 	TASK_TYPE_JOIN_MATCH        			0x08								//��������

#define 	TASK_STATUS_UNFINISH  					0									//����״̬
#define 	TASK_STATUS_SUCCESS  					1									//����ɹ�
#define 	TASK_STATUS_FAILED  					2									//����ʧ��
#define 	TASK_STATUS_REWARD  					3									//��ȡ����

#define 	TASK_MAX_COUNT							128									//��������

#define 	US_NULL									0x00								//û��״̬
#define 	US_FREE									0x01								//վ��״̬
#define 	US_SIT									0x02								//����״̬
#define 	US_READY								0x03								//ͬ��״̬
#define 	US_LOOKON								0x04								//�Թ�״̬
#define 	US_PLAYING								0x05								//��Ϸ״̬
#define 	US_OFFLINE								0x06								//����״̬

#define 	MS_NULL									0x00								//û��״̬
#define 	MS_SIGNUP								0x01								//����״̬
#define 	MS_MATCHING								0x02								//��������
	//#define 	MS_MATCHEND								0x03								//��������
	//#define 	MS_OUT									0x04								//��̭״̬
#define 	MS_OUT									0x03								//��̭״̬

#define 	SRL_LOOKON								0x00000001							//�Թ۱�־
#define 	SRL_OFFLINE								0x00000002							//���߱�־
#define 	SRL_SAME_IP								0x00000004							//ͬ����־

#define 	SRL_ROOM_CHAT							0x00000100							//�����־
#define 	SRL_GAME_CHAT							0x00000200							//�����־
#define 	SRL_WISPER_CHAT							0x00000400							//˽�ı�־
#define 	SRL_HIDE_USER_INFO						0x00000800							//���ر�־

#define 	UD_NULL									0									//��Ч����
#define 	UD_IMAGE								100									//ͼ������
#define 	UD_CUSTOM								200									//�Զ�����

#define 	UD_GAME_ID								1									//��Ϸ��ʶ
#define 	UD_USER_ID								2									//�û���ʶ
#define		UD_NICKNAME								3									//�û��ǳ�

#define 	UD_GENDER								10									//�û��Ա�
#define 	UD_GROUP_NAME							11									//��������
#define 	UD_UNDER_WRITE							12									//����ǩ��

#define 	UD_TABLE								20									//��Ϸ����
#define 	UD_CHAIR								21									//���Ӻ���

#define 	UD_SCORE								30									//�û�����
#define 	UD_BANKER								31									//�û�����
#define 	UD_EXPERIENCE							32									//�û�����
#define 	UD_LOVELINESS							33									//�û�����
#define 	UD_WIN_COUNT							34									//ʤ������
#define 	UD_LOST_COUNT							35									//�������
#define 	UD_DRAW_COUNT							36									//�;�����
#define 	UD_FLEE_COUNT							37									//�Ӿ�����
#define 	UD_PLAY_COUNT							38									//�ܾ�����

#define 	UD_WIN_RATE								40									//�û�ʤ��
#define 	UD_LOST_RATE							41									//�û�����
#define 	UD_DRAW_RATE							42									//�û�����
#define 	UD_FLEE_RATE							43									//�û�����
#define	 	UD_GAME_LEVEL							44									//��Ϸ�ȼ�

#define 	UD_NOTE_INFO							50									//�û���ע
#define 	UD_LOOKON_USER							51									//�Թ��û�

#define 	UD_IMAGE_FLAG							(UD_IMAGE+1)						//�û���־
#define 	UD_IMAGE_STATUS							(UD_IMAGE+2)						//�û�״̬

#define 	DB_ERROR 								-1  								//����ʧ��
#define 	DB_SUCCESS 								0  									//����ɹ�
#define 	DB_NEEDMB 								18 									//����ʧ��
#define 	DB_PASSPORT								19									//����ʧ��

#define 	PT_USE_MARK_DOUBLE_SCORE    			0x0001								//˫������
#define 	PT_USE_MARK_FOURE_SCORE     			0x0002								//�ı�����
#define 	PT_USE_MARK_GUARDKICK_CARD  			0x0010								//���ߵ���
#define 	PT_USE_MARK_POSSESS         			0x0020								//�������

#define 	MAX_PT_MARK                	 			4									//��ʶ��Ŀ

#define 	VALID_TIME_DOUBLE_SCORE     			3600								//��Чʱ��
#define 	VALID_TIME_FOUR_SCORE       			3600								//��Чʱ��
#define 	VALID_TIME_GUARDKICK_CARD   			3600								//����ʱ��
#define 	VALID_TIME_POSSESS          			3600 								//����ʱ��
#define 	VALID_TIME_KICK_BY_MANAGER  			3600								//��Ϸʱ��

#define 	DEVICE_TYPE_PC              			0x00								//PC
#define 	DEVICE_TYPE_ANDROID         			0x10								//Android
#define 	DEVICE_TYPE_ITOUCH         			 	0x20								//iTouch
#define 	DEVICE_TYPE_IPHONE          			0x40								//iPhone
#define 	DEVICE_TYPE_IPAD            			0x80								//iPad

#define 	VIEW_MODE_ALL  							0x0001								//ȫ������
#define 	VIEW_MODE_PART  						0x0002								//���ֿ���

#define 	VIEW_INFO_LEVEL_1  						0x0010								//������Ϣ
#define 	VIEW_INFO_LEVEL_2  						0x0020								//������Ϣ
#define 	VIEW_INFO_LEVEL_3  						0x0040								//������Ϣ
#define 	VIEW_INFO_LEVEL_4  						0x0080								//������Ϣ

#define 	RECVICE_GAME_CHAT  						0x0100								//��������
#define 	RECVICE_ROOM_CHAT  						0x0200								//��������
#define 	RECVICE_ROOM_WHISPER  					0x0400								//����˽��

#define 	BEHAVIOR_LOGON_NORMAL  					0x0000								//��ͨ��¼
#define 	BEHAVIOR_LOGON_IMMEDIATELY  			0x1000								//������¼

#define 	RESULT_ERROR 							-1  								//�������
#define 	RESULT_SUCCESS 							0  									//����ɹ�
#define 	RESULT_FAIL 							1  									//����ʧ��

#define 	SCORE_REASON_WRITE             		 	0 									//д�ֱ仯
#define 	SCORE_REASON_INSURE             		1 									//���б仯
#define 	SCORE_REASON_PROPERTY           		2									//���߱仯
#define 	SCORE_REASON_MATCH_FEE          		3									//��������
#define 	SCORE_REASON_MATCH_QUIT         		4									//��������

#define 	LOGON_FAIL_SERVER_INVALIDATION  		200 								//��¼����ʧ��ԭ��

#define 	ANDROID_SIMULATE						0x01								//�໥ģ��
#define 	ANDROID_PASSIVITY						0x02								//�������
#define 	ANDROID_INITIATIVE						0x04								//�������

#define 	MATCH_FEE_TYPE_GOLD						0x00								//�۷�����
#define 	MATCH_FEE_TYPE_MEDAL					0x01								//�۷�����

#define 	MATCH_TYPE_LOCKTIME						0x00								//��ʱ����
#define	 	MATCH_TYPE_IMMEDIATE					0x01								//��ʱ����

#define 	INVALID_ITEM							65535								//��Ч����
#define 	LEN_TASK_TEXT							320									//�����ı�
#define 	MAX_PATH          						260									//��ַ����

#define		INVALID_BYTE							((BYTE)(0xff))						//��Ч��ֵ
#define 	INVALID_WORD							((WORD)(0xffff))					//��Ч��ֵ
#define 	INVALID_DWORD							((DWORD)(0xffffffff))				//��Ч��ֵ

#define 	CountArray(Array) 						(sizeof(Array)/sizeof(Array[0]))	//�������

#define 	INTERFACE_OF(DST,SRC)			  		( SRC != NULL && NULL!=dynamic_cast<DST *>(SRC))	//ת���ж�

#define 	EMPTY_CHAR(p)							(p==NULL||p[0]=='\0')				//���ַ�

#define		LUA_BREAK(cond)							 if(cond) break						//���

#define 	REV_CONTINUE							0									//��������
#define 	REV_CLOSE								-1									//�ر�����


#define 	MSG_SOCKET_CONNECT						1									//��������
#define 	MSG_SOCKET_DATA							2									//��������
#define		MSG_SOCKET_CLOSED						3									//����ر�
#define 	MSG_SOCKET_ERROR						4									//�������
#define 	MSG_HTTP_DOWN							5 									//��������
#define     MSG_UN_ZIP                              6                                   //��ѹ��

#define 	DOWN_PRO_INFO							1
#define 	DOWN_COMPELETED							3
#define 	DOWN_ERROR_PATH							4 									//·������
#define 	DOWN_ERROR_CREATEFILE					5 									//�ļ���������
#define 	DOWN_ERROR_CREATEURL					6 									//��������ʧ��
#define 	DOWN_ERROR_NET		 					7 									//����ʧ��
#define     DOWN_ERROR_UNZIP 						8

#define 	PATH_DIR								"LYGame"

	//��Ȩ��Ϣ
#ifdef 	WIN32
	const TCHAR szCompilation[] = TEXT("B3D44854-9C2F-4C78-807F-8C08E940166D");
#else
#define  szCompilation  "B3D44854-9C2F-4C78-807F-8C08E940166D" //@compilation
#endif
#ifdef __cplusplus
}
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif