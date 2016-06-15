/******************************************************************
* Copyright (c) 2006,�Ϻ�ʢ������
* All rights reserved.
* 
* �ļ����ƣ�audiogroup.h
* ժ    Ҫ����Ƶ����������Ϸ�������乫�����ݽṹ
* 
* ��ǰ�汾��1.0
* ��    �ߣ�sunwanxun
* �������ڣ�2007-7-18
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

//��Ƶ���ݴ���Ȩ��
enum AudioRight
{
	ar_listening		=0x00,	//������Ƶ����
	ar_sendsinging		=0x02,	//���Է����ݳ���Ƶ����
	ar_sendspeaking		=0x20,	//���Է���˵������
	ar_sendcomment		=0x40,	//���Է��͵�������
};

//Sketch map of audio packet
/*<---m--->|<---i--->|<------h----->|<-------d1------>|<--------d2------->*/
struct AudioInfoDesc
{
	WORD	AudioType;	//enum AudioRight
	WORD	AudioToken;//��ҷ�����Ƶ���ݵ�����
	WORD	HeadLen;	//pHead����
	WORD	AudioLen;	//pAudio����
	WORD	MusicLen;	//pMusic����
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
	UINT		recvtime;//�յ�ʱ��:����
	AudioInfo	info;
	char		data[0x600-sizeof(UINT)-sizeof(AudioInfo)];
};

//��Ƶ����������ҵ�״̬
struct AudioClient
{
	CharaID		nCharaID;	//���ID
	WORD		nRight;		//��Ƶ����Ȩ��, AudioRight
	CharaID		nWatchWho;	//�ۿ�˭���ݳ��� =0ʱ���ۿ��κ��˵��ݳ���!=0ʱ��ֻ���ܴ�CharaID��ҵ�ԭʼ��Ƶ���ݣ������˵���Ƶ���ݲ�����ͣ�
	WORD		nAudioToken;//�ͻ��˷�����Ƶ���ݵ�����
	UINT		nIP;		//���ͻ������Ƶ���ݵ�IP��ַ
	WORD		nPort;		//���ͻ������Ƶ���ݵĶ˿�
	bool		bAskMusicData;//�Ƿ���Ҫ�ݳ��ߵİ�������
};

inline bool operator==(const AudioClient& client, CharaID nCharaID)
{
	return client.nCharaID == nCharaID;
}

struct AudioGroup
{
	RoomID						nRoomID;			//�ҽӵ��ķ���ID, Ҳ����Ƶ��ID
	map<CharaID, AudioClient>	mClients;			//��Ƶ���е��������
	map<AudioPackID, AudioBuffer*> mAudioPacks;		//���Խ����ط����ݳ���Ƶ����
	vector<CharaID>				vReplayedSinger;	//���طŵĸ���ID

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
	UINT			nProtoVersion;	//Э��汾��
	UINT			nListenIP;		//��Ƶ��������IP
	WORD			nListenPort;	//��Ƶ����������˿�
	UINT			nMaxClients;	//�����Ը�������ҷ���
	BOOL			bAdminBlockNew;	//��ֹ����ҽ���, �ɹ���Ա����
};


struct AudioSvrEntry :
	public AudioSvrConfig
{
	ServerID					nServerID;
	char						sIPAddress[32];	//������IP
	UINT						nHoldenClients;	//�Ѿ����ɵ��������, = sum( for_each(mAudioGroups.mClients.size() ))
	map<RoomID, AudioGroup>		mAudioGroups;	//���������Ѿ���������Ƶ��

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
