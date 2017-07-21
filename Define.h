#pragma once 
//设备类型
#define DEVICE_TYPE_PC              0x00                                //PC
#define DEVICE_TYPE_ANDROID         0x10                                //Android
#define DEVICE_TYPE_ANDROID_COCOS2D	0x11								//Android COCOS2D
#define DEVICE_TYPE_ANDROID_U3D		0x12								//Android U3D
#define DEVICE_TYPE_ITOUCH          0x20                                //iTouch
#define DEVICE_TYPE_IPHONE          0x30                                //iPhone
#define DEVICE_TYPE_IPHONE_COCOS2D	0x31								//Android COCOS2D
#define DEVICE_TYPE_IPHONE_U3D		0x32								//Android U3D
#define DEVICE_TYPE_IPAD            0x40                                //iPad
#define DEVICE_TYPE_IPAD_COCOS2D	0x41								//Android COCOS2D
#define DEVICE_TYPE_IPAD_U3D		0x42								//Android U3D


#define BULID_VER					0									//授权版本
#define PRODUCT_VER					6									//产品版本
//程序版本
#define PROCESS_VERSION(cbMainVer,cbSubVer,cbBuildVer)					\
		(DWORD)(														\
		(((BYTE)(PRODUCT_VER))<<24)+									\
		(((BYTE)(cbMainVer))<<16)+										\
		((BYTE)(cbSubVer)<<8)+											\
		(BYTE)(cbBuildVer))


#define VERSION_FRAME				PROCESS_VERSION(7,0,1)				//框架版本
#define VERSION_PLAZA				PROCESS_VERSION(7,0,1)				//大厅版本
#define VERSION_MOBILE_ANDROID		PROCESS_VERSION(7,0,1)				//手机版本
#define VERSION_MOBILE_IOS			PROCESS_VERSION(7,0,1)				//手机版本
