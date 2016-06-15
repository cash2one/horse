#pragma once

#pragma pack(push ,1)

///与Center Server通信的协议消息头结构
typedef struct tagGM2COMProtHeader
{
	WORD	length;			//数据包长度(head长度+数据长度)
	WORD	type;			//事务类型
	DWORD	reserved;		//保留字段
}GM2COMProtHeader;


//notice通知的最大长度是2M
#define COMMUTY_NOTICE_MAXSIZE	2*1024*1024


//------------------------------------------------------------------------------------------------
//game server和community server之间的消息内容格式采用以下的格式传送
//<key> <bytes> <value>\r\n<key> <bytes> <value>\r\n
//消息内容包含一到多个key-value的组合
//key:元信息的名称,为不允许为空格的字符串
//bytes:元信息的长度
//value:元信息的内容
//示例：一个玩家上线的通知
//charid 8 10789222\r\n
//servertime 17 2009:7:4:18:30:20\r\n
//action 6 online\r\n


//////////////////////////////////////////////////////////////////////////
//一般情况下，一个value只有一个属性字段，因此也就是简单的字符串
//
//有些情况下，一个value有多个属性时（例如一个房间可能包含多个属性，如房间名，房间ID，房间类型）
//#<AttribName>=<AttribValue>\r\n<AttribName>=<AttribValue>\r\n
//
//属性集之前需要用#来进行标识
//房间value 示例：   "#roomname=测试房间\r\nroomid=100045\r\nroomtype=systemroom"
//
//////////////////////////////////////////////////////////////////////////

//-------------------------------------------------------------------------------------------------
//event类型:歌曲演唱记录
//协议数据:
//key=charid		value=<int32>		*角色ID*
//key=digitalid		value=<int64>		*角色数字ID*
//key=charname		value=<string>		*角色昵称*
//key=songid		value=<int32>		*歌曲ID*
//key=songname		value=<string>		*歌曲名*
//key=totalscore	value=<int32>		*总得分*
//key=singscore		value=<int32>		*演唱得分*
//key=itemscore		value=<int32>		*道具加分*
//-------------------------------------------------------------------------------------------------
#define GM2COM_EVENT_SINGRECORD				0x6001

//-------------------------------------------------------------------------------------------------
//event类型:玩家上线
//协议数据:
//key=digitalid		value=<int64>		*角色数字ID*
//key=serverid		value=<int32>		*服务器ID*
//key=servername	value=<string>		*服务器名称*
#define GM2COM_EVENT_USERONLINE				0x6002

//-------------------------------------------------------------------------------------------------
//event类型:玩家离线
//协议数据:
//key=digitalid		value=<int64>		*角色数字ID*
//key=serverid		value=<int32>		*服务器ID*
//key=servername	value=<string>		*服务器名称*
#define GM2COM_EVENT_USEROFFLINE			0x6003


//-------------------------------------------------------------------------------------------------
//event类型:玩家位置
//协议数据:
//key=digitalid		value=<int64>		*角色数字ID*
//key=location		value=<string>		*位置名称*	
//当玩家在大厅时,value=lobby
//当玩家在房间时,value=room#name=<room's name>\r\nid=<room's id>\r\nroomtype=<room's type>\r\n
#define GM2COM_EVENT_USERLOCATION			0x6004

//-------------------------------------------------------------------------------------------------
//event类型:玩家加好友
//协议数据:
//key=user1		value=<int32>		*玩家角色ID*
//key=user2		value=<int32>		*另一位玩家角色ID*	
#define GM2COM_EVENT_MAKEFRIEND				0x6005

//-------------------------------------------------------------------------------------------------
//event类型:玩家之间使用道具互动
//协议数据:
//key=srcid			value=<int64>		*玩家数字ID*
//key=targetid		value=<int64>		*目标玩家角色ID*	
//key=itemtype		value=<int32>		*道具类型  1->游戏币道具 2->人民币道具*
//key=itemname		value=<string>		*道具名称*
//key=itemprice		value=<int32>		*道具价值*
#define GM2COM_EVENT_USEITEM				0x6006

#pragma  pack(pop)

