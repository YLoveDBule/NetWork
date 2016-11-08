#pragma once

//#include "Packet.h"

#pragma pack(1)

#ifndef IPC_VER
#define IPC_VER						1									//版本标识
#endif

#ifndef IPC_PACKET
#define IPC_PACKET					(10240-sizeof(IPC_HEAD))			//最大包长
#endif

#ifndef IPC_BUFFER
#define IPC_BUFFER					(sizeof(IPC_HEAD)+IPC_PACKET)		//缓冲长度
#endif // !1

//#ifndef IPC_SOCKET_DATA
//#define IPC_SOCKET_DATA		0x0001
//#endif
//
//#ifndef IPC_COPY_DATA
//#define IPC_COPY_DATA		0x0010
//#endif

#define IDT_IMMEDIATE			0x0101
#define IDT_ASYNCHRONISM		0x0201

#define IPC_SOCKET_DATA         0x0301
#define IPC_COPY_DATA			0x0401

#define IPC_DATA_IDENTIFY   0x0100
#define IPC_DATA_SUBREADY   0x0101
#define IPC_DATA_SUBFALLDOWN    0x0102
#define IPC_DATA_SELFUSRID    0x0103

typedef struct IPC_HEAD {
	unsigned short Version = IPC_VER;
	SOCKET Sid = INVALID_SOCKET;
	unsigned short PacketSize = 0;
	unsigned short MainCmd = 0;
	unsigned short SubCmd = 0;
	unsigned short Signal = 0;
} IPC_HEAD;

typedef struct IPC_DATA {
	IPC_HEAD Head;
	unsigned char Buffer[IPC_PACKET] = {0};
} IPC_DATA;