/******************************************************************************
* Copyright (c) 2013,qiqu
* All rights reserved.
* 
* �ļ����ƣ�msg_def.h
* ժ    Ҫ����Ϣ�����������ݽṹ����
* 
* ��ǰ�汾��1.0
* ��    �ߣ�clauschen
* �������ڣ�2013��7��25��
**********************************************************************************/

#pragma once 

#define MAX_CHAR_NAME		24

#pragma pack(push ,1)

//=======================================
// ��ϢID
//=======================================
enum SS_MSG_DEFINE
{
	////////////////////////////ͨ����Ϣ////////////////////////
	MS_CHAR_SVR_MSG_START					=0x1000,
	MS_S_ACK_DISCONNECT,							//�ͻ����ϵ�	

	/////////////��������������Ϣ/////////////////////////////////////
	GS_LOGIN_SVR_MSG_START						=0x2000,
	LOGIN_S_NOT_HEART,								//����ά���ź�(�����ֶ�: ���)DWORD CurPing
	LOGIN_C_ACK_HEART,								//��������ά���ź�(�����ֶ�: ���)

	LOGIN_C_REQ_AUTH,								//�û���½���� ��ͷ�����ֶηŰ汾��version(4�ֽ�) ����:userid/passwd/mibao  ����:mlgm020/123456/1234567890
	LOGIN_S_ACK_AUTH,								//ok-Я����list��Ϣ��Linkid   ; fail-
	//��ͷwRetCode-��ųɹ���ʧ�� �ɹ�ʱ��nReserved-����������� ����ṹ:nReserved*sizeof(tagServerList)+Linkid(int64)
	//ʧ��ʱ���ް�����Ϣ
	LOGIN_C_REQ_SELGROUP,							//�û�ѡ������ Linkid/groupindex(��serverid)
	LOGIN_S_ACK_SELGROUP,							//����Linkid/group_gsip/port
	//	
	LOGIN_C_REQ_LEAVEGAME,							//�ͻ������˳���Ϸ		
	///////////////////////////////////////////////////
	GS_GAME_SVR_MSG_START						=0x3000,
	GS_S_NOT_HEART,								//����ά���ź�(�����ֶ�: ���)DWORD CurPing
	GS_C_ACK_HEART,								//��������ά���ź�(�����ֶ�: ���)	

	GS_C_REQ_AUTH,								//�û���½��֤((�����ֶ�:��ɫID)DWORD �ͻ��˰汾��;BYTE ��½���ݳ���;��½����; userinfo(PTID/lsTransID/macAddr))
	GS_S_ACK_AUTH,								//��֤�ɹ�������壺BYTE byLoginDataLen+pLoginData+CHARACTER_INFO CharInfo(+��������Ҫʱ��׷��)

	GS_C_REQ_LEAVEGAME,							//�ͻ��˳���Ϸ
	//
	GS_CHARA_SVR_MSG_START						=0x300A,
	GS_CHARA_S_NOT_CHARALIST,						//��ɫ�б� �����ֶη�0-�޽�ɫ(�ް���) 1-�н�ɫ����ֱ�ӽ���������
	GS_CHARA_C_REQ_CHARNAME,						//�����ɫ��
	GS_CHARA_S_ACK_CHARNAME,						//����� ����Ľ�ɫ��
	GS_CHARA_C_REQ_CREATE,							//������ɫ  �����ֶη� ��ɫ����  ���������Ľ�ɫ��
	GS_CHARA_S_ACK_CREATE,							//��Ӧ������ɫ ���� ����ʾ��Ϣ;�ɹ� �����������

	GS_CHARA_SVR_MSG_END						=0x302A,

	GS_C_REQ_ITEM_LIST,							//��������б�
	GS_S_ACK_ITEM_LIST,					

	//�������
	GS_C_REQ_CARD_HELP,						//������Ԯ��
	GS_C_ACK_CARD_HELP,
	GS_C_REQ_SET_LINEUP,					//�������ó�������
	GS_C_ACK_SET_LINEUP,
	//
	GS_C_REQ_JOIN_DUPMAP,					//������븱��
	GS_C_ACK_JOIN_DUPMAP,
	//
	GS_C_REQ_USE_GM_COMMAND,				//������gm���� string sCommand
	GS_S_ACK_USE_GM_COMMAND,				//����gm������� string

	GS_S_NOT_SYSADMINLEVEL,					//֪ͨ�������ǰ��ϵͳ����Ա�ļ���(�����ֶ�: ��ǰ����(-1Ϊ�ǹ���Ա, 0Ϊ���))

	//�������
	GS_C_TOP500_INFO_REQ,					//����ǰ500������Ϣ
	GS_C_MYLADDER_INFO_REQ,					//�ҵ�����
	GS_S_TOP500_INFO_ACK,					//������������
	GS_C_GAME_OVER_IND,						//һ����Ϸ�������跢�͵÷֣���������

	//�������
	GS_C_PLAYER_ITEM_REQ,					//�����ɫ����
	GS_S_PLAYER_ITEM_ACK,					//�ظ���ɫ����
	GS_C_USE_ITEM_REQ,						//ʹ�õ���
	GS_S_USE_ITEM_ACK,						//�ظ�ʹ�õ���
	GS_S_PLAYER_ITEM_IND,					//��ɫ���߱仯֪ͨ

	//�̳� 
	GS_C_SHOP_ITEM_REQ,						//�����̳ǵ���
	GS_S_SHOP_ITEM_ACK,						//�ظ�ϵͳ����
	GS_C_SHOP_BUY_ITEM_REQ,					//�����̳ǵ���
	GS_S_SHOP_BUY_ITEM_ACK,					//�����̳ǵ��߻ظ�

	//�һ�
	GS_C_ORDER_REQ,							//�һ�ʵ�����
	GS_S_ORDER_ACK,							//����ʵ����߻ظ�
	GS_C_EXCHANGE_REQ,						//�һ���Ϸ����
	GS_S_EXCHANGE_ACK,						//�һ���Ϸ���߻ظ�
	GS_C_EXCHANGE_ITEM_REQ,					//����һ������б�
	GS_S_EXCHANGE_ITEM_ACK,					//����һ������б�ظ�

	//����
	GS_S_BOARDNOTICE_IND,					//ϵͳ������Ϣ
	GS_S_PLAYER_BOARDNOTICE_IND,			//��ҹ�����Ϣ

	//Buff
	GS_S_BUFF_ACQUIRE_IND,					//���ĳ��Buff����ص�Ч������
	GS_S_BUFF_LOST_IND,						//ʧȥĳ��Buff����Ч

	//�ؿ�
	GS_C_STAGE_LIST_REQ,					//����ؿ��б�
	GS_S_STAGE_LIST_ACK,					//�ؿ��б�ظ�
	GS_C_STAGE_START_REQ,					//����ؿ�����
	GS_S_STAGE_START_ACK,					//�ظ��ؿ�����
	GS_C_STAGE_UNLOCK_REQ,					//��������ؿ�
	GS_S_STAGE_UNLOCK_ACK,					//�ظ������ؿ�
	GS_C_STAGE_SCORE_REQ,					//�������÷�
	GS_S_STAGE_SCORE_ACK,					//�ظ��÷�
	GS_C_STAGE_BALLSEL_REQ,					//ѡ��ĵ���
	GS_S_STAGE_BALLSEL_ACK,					//ѡ��ĵ���ظ�

	//��ֵ
	GS_C_COST_BUY_REQ,						//��ȯ��ֵ
	GS_S_COST_BUY_ACK,						//�ظ���ȯ��ֵ
	GS_C_COST_LIST_REQ,						//�����ֵ�б�
	GS_S_COST_LIST_ACK,						//�ظ���ֵ�б�

	GS_S_SCORE_CHANGE_IND,					//���ָı�֪ͨ

	GS_C_COST_VERIFY_REQ,					//ƻ����ֵ��֤
	GS_S_COST_VERIFY_ACK,					//ƻ����ֵ��֤����

};

//������Ϣ
typedef enum EMSRetCode
{
	//��������
	eMSRetSuccess = 0,											//�����ɹ�
	eMSRetUnknow,												//δ֪����
	eMSRetDBProcessError,										//���ݿ��������
	eMSRetSLKProcessError,										//SLK��������
	eMSRetAuthenFailed,											//��֤ʧ��
	eMSRetPTIDLengthError,										//ƽ̨�˺ų��Ȳ���,6~10
	eMSRetPTPwdLengthError,										//ƽ̨���볤�Ȳ���,6~10
	eMSRetPTPswError,											//ƽ̨�ʻ����벻ƥ��
	eMSRetPTInfoEmpty,											//ƽ̨�����ʺŻ�����Ϊ��
	eMSRetPTIDInvalid,											//ƽ̨���ʺ� ���ʺ���Ч
	eMSRetPTError,												//ƽ̨����

	eMSRetInvalidCharName,										//��ɫ������
	eMSRetCharaNotExist,										//��ɫ������
	eMSRetCharaExist,											//��ɫ�Ѵ���

	eMSRetCharaCountFull,										//��ɫ��Ŀ�Ѵ�����

	eMSRetServerGroupNotExist,									//�������鲻����
	eMSRetMemoryError,											//�ڴ����
	eMSRetComponentError,										//�������

	eMsRetMoreInvalidCharName,									//��ʹ�õ���Ч��ɫ���������࣬�����µ�¼

	eMSRetSysNotCommentParaError,								//ϵͳ�Ƿ�����ѡ�����
	eMSRetGameModalError,										//��Ϸģʽ����

	eMSRetShowDateTypeError,									//�����������ʹ���
	eMSRetChatChannelCountExc,									//��������Ƶ��������
	eMSRetPeerChatChannelCountExc,								//�Է�����Ƶ����������
	eMSRetChatDestClientNotInvalid,								//����Ŀ���û���Ч	
	eMSRetCharSrcClientNotInvalid,								//����Դ�û���Ч
	eMSRetDestClientChatPeer,									//Ŀ���û�������ͬ��
	eMSRetDestClientNotChatPeer,								//Ŀ���û���������ͬ��
	eMSReteChatNotInvited,										//�������������ͻ�
	eMSRetChatDestClientNotAgreen,								//����Ŀ���û���ͬ��
	eMSRetChatContentTooLong,									//��������̫��
	eMSRetTetgetPlayerOffline,									//�Է�������
	eMSRetNotGM,												//����GM
	eMSRetGM,													//GM
	eMSRetItemTargetError,										//����Ŀ����Ҳ�����
	eMSRetItemNotExist,											//���߲�����
	eMSRetNotEnoughItem,										//û���㹻�ĵ���
	eMSRetTargetMyself,											//Ŀ�����Լ�
	eMSRetItemDecFailed,										//���ٵ���ʧ��

	eMSRetParamError,											//��������

	eMSRetErrorPlayer,											//����Ľ�ɫ
	eMSRetErrorPlayerStatus,									//��������״̬
	eMSRetHaveSelectSupport,									//�Ѿ�ѡ���֧������

	eMSRetNotInMiniGame,										//û����С��Ϸ��
	eMSRetNoEnoughVirtualMoney,									//û���㹻�������

	eMSRetNoData,												//û������
	eMSRetAlreadyInFriend,										//�Ѿ��Ǻ�����
	eMSRetInvalidFriendGroup,									//�Ƿ��ĺ��ѷ���
	eMSRetNotInFriend,											//���Ǻ�����

	eMSRetRefuseFriend,											//�ܾ���Ϊ����
	eMSRetOfflineMsgFull,										//������Ϣ����
	eMSRetNoOperationRight,										//û�в���Ȩ��

	eMSRetRoleFull,												//��ɫ�Ѵ�����
	eMSRetEquipPosError,										//װ��λ�ò���ȷ
	eMSRetNoMatchEquipCondition,								//װ������������
	eMSRetNoMatchUseCondition,									//ʹ������������
	eMSRetInvalidItem,											//�Ƿ��ĵ���

	//GM�������
	eMSRetGmParamCountError,						//GM���������������ȷ
	eMSRetInvalidTimeFormat,						//ʱ���ʽ��Ч
	eMSRetTimeHasPast,								//��ͼʹ�õ�ʱ���ѹ���

	eMSRetInvalidOrderType,							//�������ʹ���
	eMSRetNotFoundBanPlayer,						//δ�ҵ�����ҵ���ֹ��Ϣ
	eMSRetDontInputHere,							//������Ҫ�ڴ˴���������

	//�ʺ�����
	eMSRetPTIDIsBlocked,							//���ʺű�ͣ�û��߲������½�˷�����

	eMSDisConnectNoError = 0x1000,
	eMSDisConnectUnknow,										//δ֪����
	eMSDisConnectMsgLenLessThanMsgHeader,						//������С�ڰ�ͷ
	eMSDisConnectNotAuthenedClient,								//δ��֤�ͻ�
	eMSDisConnectSendInvalidMsg,								//����δ֪��Ϣ
	eMSDisConnectAlreadyAuthened,								//�Ѿ���֤
	eMSDisConnectLoginDataError,								//��½���ݴ���
	eMSDisConnectPacketFormatError,								//���ݰ���ʽ����
	eMSDisConnectAuthenFailed,									//��֤ʧ��
	eMSDisConnectKickUser,										//��������
	eMSDisConnectCommuError,									//ͨѶ����
	eMSDisConnectLongNotSendAthen,								//��ʱ��û�з��͵�½��
	eMSDisConnectPlatformError,									//ƽ̨����
	eMSDisConnectSeriousError,									//���ش���
	eMSDisConnectVersionError,									//�汾����
	eMSDisConnectClientFull,									//�������ͻ������ﵽ����
	eMSDisConnectTimeOut,										//�������ӳ�ʱ
	eMSDisConnectAbuseAuditor,									//�ķ�������˰�
	eMSDisConnectEstopLogin,									//����ұ���ֹ��½
	eMSDisConnectSamePTIDLogin,									//ͬһ�ʺ��ظ���½
	eMSDisConnectPTIDIsBlocked,									//���ʺű�ͣ�û��߲������½�˷�����

	//����
	eMSRetItemQueryFail,						//���߲�ѯʧ��
	eMSRetItemNotFound,							//û�иõ���
	eMSRetItemAddFail,							//��ӵ���ʧ��
	eMSRetItemReachMax,							//���߱�����
	eMSRetItemIDIlegal,							//����ID�Ƿ�
	eMSRetItemNumNotEnough,						//������������
	eMSRetItemFuncQueryFail,					//���߹��ܲ�ѯʧ��
	eMSRetItemFuncNotFound,						//���߹���δ����
	eMSRetItemCannotUse,						//�����޷�ʹ��
	eMSRetItemCannotEquip,						//�����޷�װ��
	eMSRetItemInstNotFound,						//����ʵ��δ�ҵ�
	eMSRetExchangeItemQueryFail,				//�һ�ʵ����߲�ѯʧ��
	eMSRetExchangeItemNotFound,					//�һ�ʵ�����û������
	eMSRetExchangeOrderFail,					//�һ�ʵ��DB����ʧ��
	eMSRetExchangeItemForbid,					//û�����ü۸񣬹ʲ��ܶһ�
	eMSRetItemNumCannotBeZero,					//��������Ϊ0

	//���� 4133
	eMSRetScoreNotEnough,						//���ֲ���

	//����
	eMSRetBonusQueryFail,						//������ѯʧ��
	eMSRetBonusNotFound,						//����δ����

	//Buff
	eMSRetBuffQueryFail,						//Buff��ѯʧ��
	eMSRetBuffNotFound,							//Buff IDδ�ҵ�

	//�̳�
	eMSRetBuyItemForbid,						//�۸�Ϊ0�����ܹ���
	eMSRetPointNotEnough,						//��ȯ����

	//�ؿ� 4140
	eMSRetNoStageUnlockCard,					//û�йؿ�������
	eMSRetStageIdNotFound,						//û���ҵ��ùؿ�ID
	eMSRetStageAlreadyUnlock,					//�ùؿ�ID�Ѿ�����
	eMSRetStageLocked,							//�ùؿ�IDδ����
	eMSRetStageBlockItemNotFound,				//�ؿ����������û�ҵ�
	eMSRetGetBallCostFail,						//С������û�ҵ�
	eMSRetGetBlockScoreFail,					//С���÷�û�ҵ�

	//����
	eMSRetMoneyAddFail,
	eMSRetMoneyValueIllegal,					//���ӵ�ȯֵ<=0
	eMSRetMoneyNotEnough,						//��ȯ����
	eMSRetExchangePointAddFail,
	eMSRetExchangePointValueIllegal,			//����һ�ȯ��Ŀ<=0
	eMSRetExchangePointNotEnough,				//�һ�ȯ����
	eMSRetCostItemIdNotFound,					//��ֵ��IDû�ҵ�

	eMSRetHeartBeatBreak,						//������Ϣ�ղ���

	eMSRetCostVerifyFail,						//appstore��֤ʧ��
	eMSRetCostVerifyNetError,					//appstore��֤�������
	eMSRetCostVerifyDataInvalid,				//��ֵ��֤�ַ���û�յ�
	eMSRetCostBuyPending,						//�ȴ���ֵ��¼д���ݿ�

	eMSRetMax,
}MSRETCODE;

#if 0

//============================================================================
// �������
//============================================================================

#define		MAX_ITEM_COUNT			50					//���������������

#define		MAX_STR					24					//��������
#define		MAX_PROPDEPICT			256					//��������

enum ItemPropType
{
	enum_item_prop_game = 0,		//��Ϸ���߷��� 0~99
	enum_item_prop_object = 100		//ʵ����߷��� 100~255
};

// ��ɫ��õ������ݹ���
// struct AwardPropInfo
// {
// 	unsigned int	mNumber;
// 	char            mPropName[MAX_STR];					//��������
// };

// ���ظ��ͻ��˵Ľ�ɫ��������
// struct PropInfo                                         
// {
// 	unsigned int    mID;                                //��̬ʵ��ID��Ψһ��ʶ
// 	unsigned int    mPropID;							//����ID
// 	unsigned short	mImageID;							//ͼƬ����
// 	unsigned char   mType;                              //��������
// 	unsigned char   mPropNum;							//��������
// 	unsigned char	mEquipped;							//�Ƿ���װ��
// 	unsigned int    mEquipable;                         //�Ƿ��װ��  ���߼�ֵ
// 	unsigned int    mTime;								//����ʱЧʱ��	
// 	char            mPropName[MAX_STR];					//��������
// 	char            mPropDepict[MAX_PROPDEPICT];        //��������
// };

// ���ظ��ͻ��˵��̳ǵ�������
// struct ShoppingPropInfo                                 //�̳ǵ������ݽṹ
// {
// 	unsigned int    mPropID;							//����ID
// 	unsigned int    mMoney;                             //���߼�ֵ
// 	unsigned short	mImageID;							//ͼƬ����
// 	unsigned char   mType;                              //���߷���
// 	char            mPropName[MAX_STR];					//��������
// 	char            mPropDepict[MAX_PROPDEPICT];        //��������
// };

// �������
// struct BuyItemReq							
// {
// 	unsigned int id;			//����ID
// 	unsigned int num;			//��������
// 	unsigned int source;		//������Դ
// };

// ʹ�õ���
// struct ClickItemReq							
// {
// 	unsigned int id;			//����ID
// 	unsigned int num;			//��������
// 	unsigned int instID;		//����ʵ��ID
// };

// ��������
// struct OrderItemReq
// {
// 	unsigned int itemid;
// 	unsigned int itemnum;
// 	char		 name[20];
// 	char		 email[60];
// 	char		 address[400];
// 	char		 postcode[20];
// };

// �һ�����
// struct ExchangeItemReq
// {
// 	unsigned int itemid;
// 	unsigned int itemnum;
// };

// ��������
//typedef ExchangeItemReq ShopBuyItemReq;

//============================================================================
// �������
//============================================================================

// ��������
typedef struct GLOBALE_LADDER_INFO 
{
	uint32			uIconID;					//���ͷ��
	uint32			uAllScore;					//�ۼƻ���
	char			szCharName[MAX_CHAR_NAME];	//��ɫ����	
}GLOBALE_LADDER_INFO_CFG,*PGLOBALE_LADDER_INFO;

//============================================================================
// BUFF���
//============================================================================
#define MAX_BUFF_DESC_LEN	100

// Buff��һ��Ч��������
// struct AcquireBuffInfo
// {
// 	int index;						//Ч������	
// 	int duration;					//����ʱ��
// 	int buffid;						//buff����
// 	float value;					//buff��ֵ
// 	unsigned int icon;				//Ч��ͼ��
// 	char desc[MAX_BUFF_DESC_LEN];	//����
// };
#endif

#pragma pack(pop)
