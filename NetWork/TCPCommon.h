#include <wtypes.h>
#pragma pack(1)
//��¼����
#define MDM_MB_LOGON				100									//�㳡��¼

//��¼ģʽ
#define SUB_MB_LOGON_GAMEID			1									//I D ��¼
#define SUB_MB_LOGON_ACCOUNTS		2									//�ʺŵ�¼
#define SUB_MB_REGISTER_ACCOUNTS	3									//ע���ʺ�
#define SUB_MB_LOGON_OTHERPLATFORM  4									//������½
#define SUB_MB_CHECK_VERSION		5									//�汾���
#define SUB_MB_FILL_ACCOUNT			6									//��д�˺�
#define SUB_MB_USER_REQUEST_FAILURE 7                                   //�û�����ʧ��

//��¼���
#define SUB_MB_LOGON_SUCCESS		100									//��¼�ɹ�
#define SUB_MB_LOGON_FAILURE		101									//��¼ʧ��
#define SUB_MB_DISABLE_VISITOR_LOGON 106                                //�����ο͵�½

//������ʾ
#define SUB_MB_UPDATE_NOTIFY		200									//������ʾ
//��������
#define LEN_MD5						33									//��������
#define LEN_USERNOTE				32									//��ע����
#define LEN_ACCOUNTS				33 									//�ʺų���
#define LEN_NICKNAME				33									//�ǳƳ���
#define LEN_PASSWORD				33									//���볤��
#define LEN_GROUP_NAME				32									//��������
#define LEN_UNDER_WRITE				32									//����ǩ��
#define LEN_GUID					50									//
#define LEN_LASTLOGONTIME			50									//����¼ʱ��
#define LEN_PRIVATE_TABLE_NAME      32                                  //˽�з�������
#define LEN_PRIVATE_TABLE_ID        7                                   //˽�з���ID
#define LEN_PRIVATE_TABLE_RULE		1024								//�������ݴ�С
//���ݳ���
#define LEN_QQ						16									//Q Q ����
#define LEN_EMAIL					33									//�����ʼ�
#define LEN_USER_NOTE				256									//�û���ע
#define LEN_SEAT_PHONE				33									//�̶��绰
#define LEN_MOBILE_PHONE			12									//�ƶ��绰
#define LEN_PASS_PORT_ID			19									//֤������
#define LEN_COMPELLATION			16									//��ʵ����
#define LEN_DWELLING_PLACE			128									//��ϵ��ַ
#define LEN_OPENID					33									//����ƽ̨�˺�
#define LEN_REAL_NAME               16                                  //��ʵ����
//������ʶ
#define LEN_NETWORK_ID				13									//��������
#define LEN_MACHINE_ID				33									//���г���

#pragma pack()