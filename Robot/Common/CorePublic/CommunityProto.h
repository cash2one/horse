#pragma once
//#include "CenterTransProto.h"

#pragma pack(push ,1)
#define MAKE_VERSION(v1,v2,v3,v4)   ((v1<<24)|(v2<<16)|(v3<<8)|(v4))

#ifndef SIZEOF_STR
#define SIZEOF_STR(str)	sizeof(str)-1
#endif//SIZEOF_STR

#define WEB_VERSION	MAKE_VERSION(0,1,0,1)
#define CMM_VERSION	MAKE_VERSION(0,1,0,1)

#define CHAR_NAME_LEN	24
#define PTID_LEN		24
#define CHAR_INTRODUCE_LEN	48
#define TRANS_ID_LEN	33
#define ROOM_NAME_LEN	48
#define SERVER_NAME_LEN	48

#define ATTRIBVALUE_NAME_LEN	32
#define MAX_ATTRIB_NUM			64

struct CmmHeader
{
	WORD	length;	//Э�����ݳ���
	WORD	type;	//Э���ʶ, eCmmMessageType
};

struct CmmAuthInfo
{
	UINT			nVersion;
	UINT			nAreaID;
};

//struct CmmCharOnlineEx : public CmmCharOnline
//{
//	UINT			nCurServerID;
//	char            sCurServerName[64];
//};

//#pragma warning(disable:4200)
struct CmmCommonData
{
	char            sTransID[TRANS_ID_LEN];
	DWORD           nErrorCode;//enum eCmmProtoResult
	//char            pData[0];
};
//#pragma warning(default:4200)

struct CharIDDigitalIDPair
{
	UINT64	nDigitalID;
	INT		nCharID;
};

struct CmmDBInfo
{
	char			sHost[64];
	UINT			nPort;
	char			sUserName[64];
	char			sPassword[64];
	char			sSchema[64];
};

struct CmmCharFans
{
	UINT			nFansID;
	UINT64			nFansDigitalID;
	UINT			nCommunityID;
	char			sFansName[CHAR_NAME_LEN];
	float			fFansLevel;
};

struct CmmCharFriend
{
	UINT			nFriendID;
	UINT64			nFriendDigitalID;
	UINT			nCommunityID;
	char			sFriendName[CHAR_NAME_LEN];
	BYTE			nSex;
};

#define EVENT_TARGETNAME_LEN	128
#define EVENT_DETAIL_LEN		1024
struct CmmGameEvent
{
	UINT64	u64DigitalPtId;
	UINT	uEventType;
	UINT64	u64TargetId;
	char	sTargetName[EVENT_TARGETNAME_LEN];
	UINT	wparam;
	UINT	lparam;
	char	sDetail[EVENT_DETAIL_LEN];

	CmmGameEvent()
	{
		u64DigitalPtId	= 0;
		uEventType		= 0;
		u64TargetId		= 0;
		sTargetName[0]	= '\0';
		wparam			= 0;
		sDetail[0]		= '\0';	
	};
};

//struct MOPKG_UPDATE_ACK
//{
//	UINT			nCurPlayers;	//��ǰ�������
//	float			fCycleTime;		//��Ӧʱ��
//	float			fTcpRecvFlow;	//���ܵ�tcp����
//	float			fTcpSendFlow;	//���͵�tcp����
//	float			fUdpRecvFlow;	//���ܵ�udp����
//	float			fUdpSendFlow;	//���͵�udp����
//};

//��ط��񷵻صĴ�����
enum eCmmProtoResult
{
	cmm_ret_sucess,		//�����ɹ�
	cmm_ret_exception,  //���������쳣
	cmm_ret_webversion,	//web interface�İ汾�Ų�ƥ��
	cmm_ret_invalidpack,//������ĸ�ʽ����ȷ
	cmm_ret_dberror,	//���ݿ���ʳ���
	cmm_ret_cantfindchar,//��ɫ��Ϣδ�ҵ�
	cmm_ret_nocommunityserver,//��ǰû�п��õ�����������
	cmm_ret_nomemory,	//���������㹻�ڴ洦�������
	cmm_ret_cmmtimeout,	//����������������ʱ
	cmm_ret_cmmfailed,  //��������������ʧ��
	cmm_ret_invalidarea,//ָ���ķ�����������Ч
	cmm_ret_invaliddigitalid,//ָ�����û�����ID��Ч
	cmm_ret_invalidcmmuserid,//ָ���������û�ID��Ч
	cmm_ret_invalidcharid,//ָ������Ϸ��ɫID��Ч
	cmm_ret_invalidrowcount, //ָ��������������Ч
	cmm_ret_invalidserverid,//ָ������Ϸ������ID��Ч
	cmm_ret_useroffline,    //�û�������
	cmm_ret_noidolchar, //�������ż��
	cmm_ret_nomorefans,//��ɫ�Ҳ�������fans
	cmm_ret_nomorefriend,//��ɫ�Ҳ����������
	cmm_ret_serverrequest,//ת������������ʧ��
	cmm_ret_emptyroomlist,//�����б�Ϊ��
	cmm_ret_cantfindroom, //ָ���ķ���δ�ҵ�
};

//��ط�������Э������
enum eCmmMessageType
{
	CMM_MESSAGE_START,

	CMM_REQ_AUTH_INFO,
	CMM_ACK_AUTH_INFO,

	CMM_REQ_CHARA_INFO_BYID,//digital id
	CMM_REQ_CHARA_INFO_BYPT,//ptid
	CMM_ACK_CHARA_INFO,

	CMM_REQ_CHARA_DETAIL_BYCHARID,
	CMM_REQ_CHARA_DETAIL_INFO,
	CMM_ACK_CHARA_DETAIL_INFO,

	CMM_REQ_BIND_COMMUNITY_USER,
	CMM_ACK_BIND_COMMUNITY_USER,

	CMM_REQ_UNBIND_COMMUNITY_USER,
	CMM_ACK_UNBIND_COMMUNITY_USER,

	CMM_REQ_GET_CHARA_IDOL_INFO,
	CMM_ACK_GET_CHARA_IDOL_INFO,

	CMM_REQ_GET_CHARA_FANS_INFO,
	CMM_ACK_GET_CHARA_FANS_INFO,

	CMM_REQ_GET_CHARA_FRIENDLIST,
	CMM_ACK_GET_CHARA_FRIENDLIST,

	CMM_REQ_COMMUNITY_USER_STATUS,
	CMM_ACK_COMMUNITY_USER_STATUS,

	CMM_REQ_GET_ROOM_INFO,
	CMM_ACK_GET_ROOM_INFO,

	CMM_MESSAGE_END,
};

#pragma pack(pop)
