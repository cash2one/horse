/******************************************************************
* Copyright (c) 2006,上海盛大网络
* All rights reserved.
* 
* 文件名称：audiogroup.h
* 摘    要：音频服务器与游戏服务器间公共数据结构
* 
* 当前版本：1.0
* 作    者：sunwanxun
* 建立日期：2007-7-18
********************************************************************/

#pragma once

#include <map>
using namespace std;

#pragma pack(push ,1)
#define MAKE_PROTO_VERSION(v1,v2,v3,v4)   ((v1<<24)|(v2<<16)|(v3<<8)|(v4))
#define AUDIOSERVER_VERSION	MAKE_PROTO_VERSION(0,0,0,5)

typedef UINT	CharaID;
typedef UINT	RoomID;
typedef UINT	ServerID;

//音频数据处理权限
enum AudioRight
{
	ar_listening		=0x00,	//接收音频数据
	ar_sendsinging		=0x02,	//可以发送演唱音频数据
	ar_sendspeaking		=0x20,	//可以发送说话声音
	ar_sendcomment		=0x40,	//可以发送点评声音
};

//Sketch map of audio packet
/*<---m--->|<---i--->|<------h----->|<-------d1------>|<--------d2------->*/
struct AudioInfoDesc
{
	WORD	AudioType;	//enum AudioRight
	WORD	AudioToken;//玩家发送音频数据的令牌
	WORD	HeadLen;	//pHead长度
	WORD	AudioLen;	//pAudio长度
	WORD	MusicLen;	//pMusic长度
};
#define MIN_AUDIODATA_LEN	(sizeof(AudioInfoDesc))

struct MSGHEADER;
struct AudioInfo
{
	MSGHEADER*		pMsgHeader;	// ->m (struct MSGHEADER)
	AudioInfoDesc*	pDesc;		// ->i
	BYTE*			pHead;		// ->h
	BYTE*			pAudio;		// ->d1
	BYTE*			pMusic;		// ->d2
};

union AudioPackID
{
	ULONG64 nPackID;
	struct
	{
		UINT nPlayerID;
		UINT nPackNo;
	};
};
inline bool operator<(const AudioPackID& nID1, const AudioPackID& nID2)
{
	return nID1.nPackID<nID2.nPackID;
}

struct AudioBuffer
{
	UINT		recvtime;//收到时间:毫秒
	AudioInfo	info;
	char		data[0x600-sizeof(UINT)-sizeof(AudioInfo)];
};

//音频服务器上玩家的状态
struct AudioClient
{
	CharaID		nCharaID;	//玩家ID
	WORD		nRight;		//音频数据权限, AudioRight
	CharaID		nWatchWho;	//观看谁的演唱； =0时，观看任何人的演唱；!=0时，只接受此CharaID玩家的原始音频数据，其他人的音频数据拆包后发送；
	WORD		nAudioToken;//客户端发送音频数据的令牌
	UINT		nIP;		//发送或接收音频数据的IP地址
	WORD		nPort;		//发送或接收音频数据的端口
	bool		bAskMusicData;//是否需要演唱者的伴奏数据
};

inline bool operator==(const AudioClient& client, CharaID nCharaID)
{
	return client.nCharaID == nCharaID;
}

struct AudioGroup
{
	RoomID						nRoomID;			//挂接到的房间ID, 也是音频组ID
	map<CharaID, AudioClient>	mClients;			//音频组中的所有玩家
	map<AudioPackID, AudioBuffer*> mAudioPacks;		//可以进行重发的演唱音频数据
	vector<CharaID>				vReplayedSinger;	//被重放的歌手ID

	AudioGroup()
	{
		nRoomID = 0;
	}

	AudioGroup& operator=(const AudioGroup& grp)
	{
		nRoomID = grp.nRoomID;
		mClients = grp.mClients;
		ASSERT(mAudioPacks.size()==0 && grp.mAudioPacks.size()==0);
		vReplayedSinger = grp.vReplayedSinger;

		return *this;
	}

	bool operator==(RoomID nTheRoomID)
	{
		return nRoomID == nTheRoomID;
	}
};


struct AudioSvrConfig
{
	UINT			nProtoVersion;	//协议版本号
	UINT			nListenIP;		//音频服务器的IP
	WORD			nListenPort;	//音频服务器服务端口
	UINT			nMaxClients;	//最多可以给多少玩家服务
	BOOL			bAdminBlockNew;	//阻止新玩家进入, 由管理员设置
};


struct AudioSvrEntry :
	public AudioSvrConfig
{
	ServerID					nServerID;
	char						sIPAddress[32];	//服务器IP
	UINT						nHoldenClients;	//已经容纳的玩家数量, = sum( for_each(mAudioGroups.mClients.size() ))
	map<RoomID, AudioGroup>		mAudioGroups;	//服务器上已经创建的音频组

	AudioSvrEntry()
	{
		nServerID = 0;
		memset(sIPAddress, 0, sizeof(sIPAddress));
		nListenIP = 0;
		nListenPort = 0;
		bAdminBlockNew = FALSE;
		nMaxClients = 0;
		nHoldenClients = 0;
	}

	AudioSvrEntry& operator=(const AudioSvrEntry& entry)
	{
		nServerID = entry.nServerID;
		strncpy(sIPAddress, entry.sIPAddress, sizeof(sIPAddress));
		nListenIP = entry.nListenIP;
		nListenPort = entry.nListenPort;
		bAdminBlockNew = entry.bAdminBlockNew;
		nMaxClients = entry.nMaxClients;
		nHoldenClients = entry.nHoldenClients;
		mAudioGroups = entry.mAudioGroups;

		return *this;
	}
};

#pragma pack(pop)
