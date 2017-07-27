#pragma once
#include "Packet.h"



//关闭原因
#define SHUT_REASON_INSIDE			0									//内部原因
#define SHUT_REASON_NORMAL			1									//正常关闭
#define SHUT_REASON_REMOTE			2									//远程关闭
#define SHUT_REASON_TIME_OUT		3									//网络超时
#define SHUT_REASON_EXCEPTION		4									//异常关闭

//网络状态
#define SOCKET_STATUS_IDLE			0									//空闲状态
#define SOCKET_STATUS_WAIT			1									//等待状态
#define SOCKET_STATUS_CONNECT		2									//连接状态

//连接错误
#define CONNECT_SUCCESS				0									//连接成功
#define CONNECT_FAILURE				1									//连接失败
#define CONNECT_EXCEPTION			2									//参数异常
#define CONNECT_PROXY_FAILURE		3									//连接成功
#define CONNECT_PROXY_USER_INVALID	4									//用户无效
#define CONNECT_INPROGRESS			5

//代理类型
#define PROXY_NONE					0									//没有代理
#define PROXY_HTTP					1									//代理类型
#define PROXY_SOCKS4				2									//代理类型
#define PROXY_SOCKS5				3									//代理类型

#ifndef NULL
#define NULL 0
#endif

//网络接口
class ITCPSocketSink //interface
{
	//网络事件
public:
	//连接事件
	virtual bool OnEventTCPSocketLink(unsigned short wSocketID, 
		int nErrorCode) = 0;

	//关闭事件
	virtual bool OnEventTCPSocketShut(unsigned short wSocketID, 
		unsigned char cbShutReason) = 0;

	//读取事件
	virtual bool OnEventTCPSocketRead(unsigned short wSocketID, 
		TCP_Command Command, void *pData, 
		unsigned short wDataSize) = 0;
};
