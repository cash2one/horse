#pragma once

#define MAKE_PROTO_VERSION(v1,v2,v3,v4)   ((v1<<24)|(v2<<16)|(v3<<8)|(v4))
#define ENCODER_VERSION	MAKE_PROTO_VERSION(0,0,0,3)
#define CHAR_NAME_LEN	24
#define PTID_LEN 24
#define MAX_ROOM_NAME	48
#define Appraise_Max    5
#define MAX_RIP_NOTICEBUF_SIZE	512
struct b2gProtoHeader
{
	WORD	length;	//Э�����ݳ���
	BYTE	type;	//Э���ʶ, b2gProtoType
	BYTE	ret;	//����ֵ, b2gProtoResult
};

enum b2gProtoResult
{
	b2gProtoResult_Success,			//�����ɹ�
	b2gProtoResult_PackFormatErr,	//�������ʽ����
	b2gProtoResult_GameSvrDisconnected,	//��Ϸ�������Ͽ������ˣ�����ʧ��
	b2gProtoResult_SendCmdError,	//���������������
	b2gProtoResult_AudioVersion,	//��Ƶ�������汾��ƥ��
	b2gProtoResult_EncoderVersion,	//����Э��汾��ƥ��
	b2gProtoResult_RoomNotExist,	//��Ҫ���ĵķ��䲻����
	b2gProtoResult_InvalidSingerIdx,//��Ҫ���ĵĸ�����������ȷ
	b2gProtoResult_PracticeRoom,	//���ܶ������跿����Ƶ
	b2gProtoResult_MemoryLack,		//�ڴ治�㣬����Ĳ���ʧ��
	b2gProtoResult_HasSubscribed,	//���棺�˸��ֵ���Ƶ�Ѿ����Ĺ���
	b2gProtoResult_NotSubscribeRoom,//���棺û�ж��Ĺ��˷������Ƶ
	b2gProtoResult_NotSubscribeSinger,//���棺û�ж��Ĺ��˸��ֵ���Ƶ
};


struct SubscribePP
{
	UINT	nRoomID;
	BYTE	nReserved;
};

struct ssCharInfo
{
	UINT64  nDigitalID;
	UINT	nCharID;
	UINT	nClubFlagID;
	char	sCharName[CHAR_NAME_LEN];
	char	sPTID[PTID_LEN];
	BYTE	nSex;	
};

struct ssRoomInfo
{
	char	sRoomName[MAX_ROOM_NAME];
	UINT	nRoomID;
};

enum b2gRoomRegResult
{
	b2gRoomReg_Success,				//�����ɹ�
	b2gRoomReg_RoomTypeError,		//�������ʹ���
	b2gRoomReg_RoomDontExist,		//���䲻����
	b2gRoomReg_RoomRegAlready,		//���䲻����
};

struct ripRoomRegister
{
	UINT	nRoomID;
	char	sRoomName[MAX_ROOM_NAME];
	BYTE	Ret;
};

struct SSUserRipNoticeInfo
{
	UINT nPlyerID;
	char sNoticeMsg[MAX_RIP_NOTICEBUF_SIZE];
};

//���ֽ�������
struct ssSingSquare
{
	UINT64	SingerDigitalID;
	//***********************���ֽ���*****************************//
	int		Score;							//�質����
	int		ItemScore;						//���ߵ÷�
	int		Combo;							//��õ�Combo��(��Combo��ռ�ܾ����Ķ���)
	int		MaxCombo;						//���Combo��
	BYTE	SquareType;						//��������
	BYTE	JudgeType;						//ʤ���ж�
	BYTE	TotalAppraises;					//��������
	BYTE	Appraises[Appraise_Max];		//�����ۼ�
	WORD	AverageGivedScore;				//ƽ������
	float	VipGiveScorePercent;			//��ίϵ��
	UINT	TotalScore;						//�ܵ÷�
};

enum b2gProtocolType
{
	E2G_REQ_SUBSCRIBE_PP=0x10,	//ֱ������������Ϸ������ ���ķ����е���Ƶ��
	/*
		struct
		{
			UINT	audio_version;	//��Ƶ������Э��汾��
			UINT	encoder_version;
			struct	SubscribePP;
		}
	*/

	G2E_ACK_SUBSCRIBE_PP,	//��Ϸ��������ֱ�������� ���ض��Ľ��
	/*
		struct SubscribePP;
	*/

	E2G_REQ_UNSUBSCRIBE_PP=0x16,	//ֱ������������Ϸ������  ȡ�����ķ����е���Ƶ��
	/*
		struct	SubscribePP;
	*/
	G2E_ACK_UNSUBSCRIBE_PP,	//��Ϸ��������ֱ��������  ����ȡ�����ķ����е���Ƶ��
	/*
		struct	SubscribePP;
	*/

	B2GE_NOT_UPDATE_SERVER_LINK=0x18,	//ֱ������������Ϸ��������ֱ������������������  ֪ͨ����������Ͽ���������Ϸ�������Ͽ�
	/*
		struct
		{
			BYTE	nServerType;	//enum SERVER_TYPE   gs <---90--- bs ---10---> es
			bool	bIsConnected;	//true=�������ˣ�false=�Ͽ���
		}
	*/


	G2E_NOT_START_SINGING=0x20,	//��Ϸ��������ֱ�������� ֪ͨ��ҿ�ʼ�ݳ�
	/*
		struct
		{
			struct		SubscribePP;
			ssCharInfo	Singer;
			bool		bIsReplay;
			int			nSongID;
			BYTE		nSongNameLength;
			char*		sSongName;
		}
	*/

	G2E_NOT_FINISH_SINGING,	//��Ϸ��������ֱ�������� ֪ͨ��ҽ����ݳ�
	/*
		struct
		{
			struct	SubscribePP;
		}
	*/

	G2E_NOT_DELETE_ROOM,		//��Ϸ��������ֱ��������  ֪ͨ���䱻ɾ��
	/*
		struct
		{
			UINT	nRoomID;
		}
	*/
	G2E_NOT_UPDATE_ROOMNAME=0x26,	//��Ϸ��������ֱ�������� ֪ͨ���·�������
	/*
		struct
		{
			UINT	nRoomID;		
			BYTE	nRoomNameLength;
			char*	sRoomName;
		}
	*/
	G2E_NOT_UPDATE_EMCEE=0x28,	//��Ϸ��������ֱ�������� ֪ͨ������������Ϣ
	/*
		struct
		{
			UINT		nRoomID;
			ssCharInfo	Emcee;
		}
	*/
	G2E_NOT_UPDATE_COMMENT=0x29,	//��Ϸ��������ֱ�������� ֪ͨ���������������Ϣ
	/*
	struct
	{
		UINT		nRoomID;		
		ssCharInfo	Comment;
	}
	*/
	B2E_NOT_AUDIO_INFO=0x30,	//�㲥��������ֱ�������� ֪ͨ������������Ϣ
	/*
		struct
		{
			struct	SubscribePP;
			UINT	nPlayerID1;//reserved
			UINT	nPlayerID2;//reserved
			struct	AudioInfoDesc;
			BYTE	pAudioHead[];
			BYTE	pAudioData[];
			BYTE	pMusicData[];
		}
	*/
	G2E_NOT_START_RECORD,		//��Ϸ������ֱ֪ͨ�������� ��ʼ¼���ݳ��ļ�
	/*
		struct	SubscribePP;
	*/

	G2E_NOT_CLOSE_RECORD,		//��Ϸ������ֱ֪ͨ�������� �ر�����¼�Ƶ��ݳ��ļ�
	/*
	struct
	{
		struct	SubscribePP;
		bool    bSaveRecord;//�Ƿ���Ҫ�����Ѿ�¼�Ƶ��ļ�
	}
	*/

	G2E_NOT_ROOM_CHAT,      //������������Ϣ
	/*
	struct
	{
		struct		SubscribePP;
		ssCharInfo	ChatSender;
		WORD		nChatLength;
		char*		sChatMsg;
	}
	*/

	G2E_NOT_USE_ITEM,
	/*
	struct
	{
		struct		SubscribePP;
		ssCharInfo	User;
		ssCharInfo	Target;

		UINT		ItemTypeID;
		BYTE		nItemNameLength;
		char*		sItemName;
	}
	*/
	G2E_NOT_SING_SQUARE,
	/*
	struct
	{
		struct		SubscribePP;
		struct		ssSingSquare;
	}
	*/
	E2G_GET_ROOM_LIST=0x60,
	/*
		None
	*/
	G2E_ACK_ROOM_LIST,
	/*
		(multiple)
		struct ssRoomInfo
		{
			char	sRoomName[MAX_ROOM_NAME];
			UINT	nRoomID;
		};
		
	*/

	E2G_USER_NOTICE = 0x70,	//�û���ʾ	
	/*
	(single)
	struct SSUserRipNoticeInfo
	{
	UINT nPlyerID;
	char sNoticeMsg[MAX_RIP_NOTICEBUF_SIZE];
	};
	*/
};