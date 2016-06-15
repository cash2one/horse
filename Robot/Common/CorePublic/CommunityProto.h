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
	WORD	length;	//协议数据长度
	WORD	type;	//协议标识, eCmmMessageType
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
//	UINT			nCurPlayers;	//当前玩家数量
//	float			fCycleTime;		//响应时间
//	float			fTcpRecvFlow;	//接受的tcp流量
//	float			fTcpSendFlow;	//发送的tcp流量
//	float			fUdpRecvFlow;	//接受的udp流量
//	float			fUdpSendFlow;	//发送的udp流量
//};

//监控服务返回的错误码
enum eCmmProtoResult
{
	cmm_ret_sucess,		//操作成功
	cmm_ret_exception,  //操作出现异常
	cmm_ret_webversion,	//web interface的版本号不匹配
	cmm_ret_invalidpack,//请求包的格式不正确
	cmm_ret_dberror,	//数据库访问出错
	cmm_ret_cantfindchar,//角色信息未找到
	cmm_ret_nocommunityserver,//当前没有可用的社区服务器
	cmm_ret_nomemory,	//服务器无足够内存处理此请求
	cmm_ret_cmmtimeout,	//社区服务器操作超时
	cmm_ret_cmmfailed,  //社区服务器操作失败
	cmm_ret_invalidarea,//指定的服务器区号无效
	cmm_ret_invaliddigitalid,//指定的用户数字ID无效
	cmm_ret_invalidcmmuserid,//指定的社区用户ID无效
	cmm_ret_invalidcharid,//指定的游戏角色ID无效
	cmm_ret_invalidrowcount, //指定的数据行数无效
	cmm_ret_invalidserverid,//指定的游戏服务器ID无效
	cmm_ret_useroffline,    //用户不在线
	cmm_ret_noidolchar, //此玩家无偶像
	cmm_ret_nomorefans,//角色找不到更多fans
	cmm_ret_nomorefriend,//角色找不到更多好友
	cmm_ret_serverrequest,//转发服务器请求失败
	cmm_ret_emptyroomlist,//房间列表为空
	cmm_ret_cantfindroom, //指定的房间未找到
};

//监控服务器的协议类型
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
