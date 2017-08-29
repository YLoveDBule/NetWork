#pragma once
#include "Packet.h"



//�ر�ԭ��
#define SHUT_REASON_INSIDE			0									//�ڲ�ԭ��
#define SHUT_REASON_NORMAL			1									//�����ر�
#define SHUT_REASON_REMOTE			2									//Զ�̹ر�
#define SHUT_REASON_TIME_OUT		3									//���糬ʱ
#define SHUT_REASON_EXCEPTION		4									//�쳣�ر�

//����״̬
#define SOCKET_STATUS_IDLE			0									//����״̬
#define SOCKET_STATUS_WAIT			1									//�ȴ�״̬
#define SOCKET_STATUS_CONNECT		2									//����״̬

//���Ӵ���
#define CONNECT_SUCCESS				0									//���ӳɹ�
#define CONNECT_FAILURE				1									//����ʧ��
#define CONNECT_EXCEPTION			2									//�����쳣
#define CONNECT_PROXY_FAILURE		3									//���ӳɹ�
#define CONNECT_PROXY_USER_INVALID	4									//�û���Ч
#define CONNECT_INPROGRESS			5

//��������
#define PROXY_NONE					0									//û�д���
#define PROXY_HTTP					1									//��������
#define PROXY_SOCKS4				2									//��������
#define PROXY_SOCKS5				3									//��������

#ifndef NULL
#define NULL 0
#endif

//����ӿ�
class ITCPSocketSink //interface
{
	//�����¼�
public:
	//�����¼�
	virtual bool OnEventTCPSocketLink(unsigned short wSocketID, 
		int nErrorCode) = 0;

	//�ر��¼�
	virtual bool OnEventTCPSocketShut(unsigned short wSocketID, 
		unsigned char cbShutReason) = 0;

	//��ȡ�¼�
	virtual bool OnEventTCPSocketRead(unsigned short wSocketID, 
		TCP_Command Command, void *pData, 
		unsigned short wDataSize) = 0;
};
