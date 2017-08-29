#include <wtypes.h>
#pragma pack(1)
//登录命令
#define MDM_MB_LOGON				100									//广场登录

//登录模式
#define SUB_MB_LOGON_GAMEID			1									//I D 登录
#define SUB_MB_LOGON_ACCOUNTS		2									//帐号登录
#define SUB_MB_REGISTER_ACCOUNTS	3									//注册帐号
#define SUB_MB_LOGON_OTHERPLATFORM  4									//其他登陆
#define SUB_MB_CHECK_VERSION		5									//版本检测
#define SUB_MB_FILL_ACCOUNT			6									//填写账号
#define SUB_MB_USER_REQUEST_FAILURE 7                                   //用户请求失败

//登录结果
#define SUB_MB_LOGON_SUCCESS		100									//登录成功
#define SUB_MB_LOGON_FAILURE		101									//登录失败
#define SUB_MB_DISABLE_VISITOR_LOGON 106                                //禁用游客登陆

//升级提示
#define SUB_MB_UPDATE_NOTIFY		200									//升级提示
//资料数据
#define LEN_MD5						33									//加密密码
#define LEN_USERNOTE				32									//备注长度
#define LEN_ACCOUNTS				33 									//帐号长度
#define LEN_NICKNAME				33									//昵称长度
#define LEN_PASSWORD				33									//密码长度
#define LEN_GROUP_NAME				32									//社团名字
#define LEN_UNDER_WRITE				32									//个性签名
#define LEN_GUID					50									//
#define LEN_LASTLOGONTIME			50									//最后登录时间
#define LEN_PRIVATE_TABLE_NAME      32                                  //私有房间名称
#define LEN_PRIVATE_TABLE_ID        7                                   //私有房间ID
#define LEN_PRIVATE_TABLE_RULE		1024								//规则数据大小
//数据长度
#define LEN_QQ						16									//Q Q 号码
#define LEN_EMAIL					33									//电子邮件
#define LEN_USER_NOTE				256									//用户备注
#define LEN_SEAT_PHONE				33									//固定电话
#define LEN_MOBILE_PHONE			12									//移动电话
#define LEN_PASS_PORT_ID			19									//证件号码
#define LEN_COMPELLATION			16									//真实名字
#define LEN_DWELLING_PLACE			128									//联系地址
#define LEN_OPENID					33									//开放平台账号
#define LEN_REAL_NAME               16                                  //真实姓名
//机器标识
#define LEN_NETWORK_ID				13									//网卡长度
#define LEN_MACHINE_ID				33									//序列长度

#pragma pack()