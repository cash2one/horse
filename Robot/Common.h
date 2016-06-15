#ifndef _COMMON_H_
#define _COMMON_H_
/*
	����ļ����ȫ�ֵĳ��� Э���
	2013/05/14
*/

#define INT64   __int64

#define  MAX_CARD_NAME   24
#define  MAX_PTID   24
#define  MAX_CHAR_NAME   24
#define  MAX_STR         24
#define  MAX_PROPDEPICT  256

#define PTM_RATIO               32.0
#define RECTANGLE_WIDTH         40.0
#define RECTANGLE_HEIGHT        20.0
#define CIRCLE_RADIUS           32.0

#pragma pack(push,1)
struct BlockObj 
{
	unsigned char  mType;                //������������ͣ���Ҫ��3�֣�b2_staticBody:��ֹ�������b2_kinematicBody:�˶�ѧ�����b2_dynamicBody:��̬���
	unsigned int   mShape;               //�����״����
	unsigned int   imageID;				 //ͼƬ����
	float          mDensity;			 //������ܶ�
	float          mRestitution;         //����Ļظ�ֵ 0��1֮���ֵ
	float          mFriction;			 //Ħ����
	float		    MotorTorque;         //���ֵ���ڰ�����ת�ؽڵ��������������ֵ������Ҫ���ùؽ���
	float			mLineSpeed;          //���ֵ���ڿ����������������˶��������
	float			mAngleSpeed;         //��ת�ٶȣ����ڴ�����ת�����
	float           mAngle;              //��ʼ��ת�Ƕ�
	float           mX;                  //X����
	float           mY;                  //Y����
	char           iteamID[MAX_STR];
	BlockObj()
	{
		mShape = 0;
		mType = 0;
		mDensity = 0.0f;
		mRestitution = 0.0f;
		mFriction = 0.0f;
		iteamID[0] = 0;
		imageID = 0;
		MotorTorque = 0.0f;
		mLineSpeed  = 0.0f;
		mAngleSpeed = 0.0f;
		mX = 0.0f;
		mY = 0.0f;
		mAngle = 0.0f;
	}
};
struct Scene_date
{
	unsigned  char    mSceneID;				 //����ID�������ֱ�ʾ
	unsigned  char    mSceneShape;           //������״
// 	unsigned  char    mBall1;                // 1����
// 	unsigned  char    mBall2;                // 1����
// 	unsigned  char    mBall3;                // 1����
	float			  mGravity;              //����ֵ
	unsigned   int    mMinImage;			 //����ͼƬ����
	unsigned   int    mMaxImage;			 //����ͼƬ����
	char              mSceneName[MAX_STR];   //��������
	Scene_date()
	{
		mSceneID = 0;
		mSceneShape = 0;
// 		mBall1 = 0;
// 		mBall2 = 0;
// 		mBall3 = 0;
		mGravity = 0.0f;
		mSceneName[0] = 0;
		mMinImage = 0;
		mMaxImage = 0;
	}
};
struct Ball_date
{
	unsigned char mBallID;           //С��ID
	unsigned int  mImageID;          //С��ͼƬ����
	float         mDensity;			 //С���ܶ�
	float         mFriction;         //С��Ħ����
	float         mRestitution;      //С�������ظ���
	Ball_date()
	{
		mBallID = 0;
		mImageID = 0;
		mDensity = 0;
		mFriction = 0;
		mRestitution = 0;
	}
};

//��¼IP���������˿ڵĽṹ��
struct  choseIP
{
	char name[MAX_STR];			//��������
	char ip[MAX_STR];				//ip��ַ
	unsigned int	port;		//�˿ں�
	unsigned char   state;		//������״̬ 
	unsigned char	isgroom;	//�Ƿ��Ƽ�
	choseIP()
	{
		port = 0;
		state = 0;
		isgroom = 0;
	}
};
//��¼�˺ź�����Ľṹ
struct  ZhangHao
{
	char  mZhang[MAX_STR];
	char  mPassWord[MAX_STR];
};
//������Ϣ�ṹ��
struct BaseCharaInfo
{	
	char					szPTID[MAX_PTID];						//ƽ̨ID
	INT64					iDigitalPTID;                           //����ID

	char					szCharName[MAX_CHAR_NAME];				//��ɫ����	
	unsigned int            mIntegral;								//��ǰ����
	unsigned int            mPileIntegral;                          //�ۼƻ���
	unsigned int			mRollCall;								//���
	unsigned int            mHoodleRoll;                            //����һ���
	unsigned int 			mIntegralLadder;						//��������	
	unsigned int            mTimeVIP;                               //VIPʣ��ʱ��
	unsigned short			mMaxPack;								//�����������
	unsigned int 			nReserv;								//����
	unsigned int			nCreateTime;

	BaseCharaInfo()
	{		
		szPTID[0]=0;
		iDigitalPTID=0;
		szCharName[0]=0;
		mIntegral = 0;
		mPileIntegral = 0;
		mRollCall=0;
		mHoodleRoll = 0;
		mIntegralLadder = 0;
		mMaxPack = 0;
		nReserv=0;
		nCreateTime = 0;
		mTimeVIP = 0;
	}
};
struct CHARACTER_INFO
	:public BaseCharaInfo
{
	unsigned int			nClubID;                                    //����ID
	short			sTitle1;											//�ƺ�1
	short			sTitle2;											//�ƺ�2		
	unsigned char			nDifficultyLevel;									//�Ѷȵȼ�
	unsigned char			nShopDiscount;										//�����ۿ�,�ȵ�
	unsigned short			wReserv	;											//	����
	unsigned int			dwBtnStatus;										//������button����

	CHARACTER_INFO()
		:nClubID(0)
	{
		sTitle1=0;			
		sTitle2=0;			

		nDifficultyLevel = 0;
		nShopDiscount = 0;

		dwBtnStatus = 0;
	}
};
// ��������
enum ItemFuncType
{
	enum_item_func_type_start = 0x00,
	enum_item_func_score_bonus = 0x01,					//�౶���ֿ�
	enum_item_func_ball_exchange,						//��õ���һ�ȯ
	enum_item_func_stage_unlock,						//��������
	enum_item_func_vip_card,							//VIP��
	enum_item_func_type_num
};

//���߽ṹ
struct PropInfo
{
	unsigned int    mID;                                //��̬ID��Ψһ��ʯ
	unsigned int    mPropID;							//����ID
	unsigned short	mImageID;							//ͼƬ����   
	unsigned char   mType;                              //��������    2���౶���ֿ���3������һ���4��������������5��VIP��
	unsigned char   mPropNum;							//��������
	unsigned char   mIsDress;                           //�Ƿ�װ���У�  0��û�У�1��װ����
	unsigned int    mDress;                           //�Ƿ�����װ��  0������װ����1����װ��
	unsigned int    mTime;								//����ʱЧʱ��
	char            mPropName[MAX_STR];					//��������
	char            mPropDepict[MAX_PROPDEPICT];        //��������
	PropInfo()
	{
		mID = 0;
		mPropID = 0;
		mImageID = 0;
		mType = 0;
		mPropNum = 0;
		mDress = 0;
		mTime = 0;
		mPropName[0] = 0;
		mPropDepict[0] = 0;
	}
};

inline bool operator == (const PropInfo& plyItemInfo,unsigned int instID)
{
	return plyItemInfo.mID == instID;
}

//�̳ǵ������ݽṹ
struct ShoppingPropInfo
{
	unsigned int    mPropID;							//����ID
	unsigned int    mMoney;                             //���߼�ֵ
	unsigned short	mImageID;							//ͼƬ����
	unsigned char   mType;                              //���߷���
	char            mPropName[MAX_STR];					//��������
	char            mPropDepict[MAX_PROPDEPICT];        //��������
	ShoppingPropInfo()
	{
		mPropID = 0;
		mMoney = 0;
		mImageID = 0;
		mType = 0;
		mPropName[0] = 0;
		mPropDepict[0] = 0;
	}
};
//���а���Ϣ�ṹ
struct LadderInfo
{
	unsigned int    mLadder;                            //����
	unsigned int	mPileIntegral;                      //����ۼƻ���
	char			szCharName[MAX_CHAR_NAME];			//��ɫ����	
	LadderInfo()
	{
		mLadder = 0;
		mPileIntegral = 0;
		szCharName[0] = 0;
	}
};
//����ѡ����Ϣ����
struct ChoseMapListST
{
	unsigned int    mMapID;                             //�������ID
	unsigned int    mImageID;                           //ѡ�񳡾�ͼƬ���
	unsigned char   mLock;                              //�Ƿ�����
	unsigned char	mBall[3];
	char            mName[MAX_STR];                     //��������
};
// ��ɫ��õ������ݹ���
struct AwardPropInfo
{
	unsigned int	mNumber;
	char            mPropName[MAX_STR];					//��������
};
#define MAX_BUFF_DESC_LEN	100

enum BuffType
{
	enum_buff_type_start = 0x00,
	enum_buff_score_bonus,			//�౶���ֿ�
	enum_buff_vip_level,			//VIP��	
	enum_buff_type_num
};
// Buff��һ��Ч��������
struct AcquireBuffInfo
{
	int index;						//Ч������	
	int duration;					//����ʱ��
	int mType;                      //buff����
	float    mValue;                //���ֱ���
	unsigned int icon;				//Ч��ͼ��
	char desc[MAX_BUFF_DESC_LEN];	//����
};

// ��������
struct OrderItemReq
{
	unsigned int itemid;
	unsigned int itemnum;
	char		 name[20];
	char		 email[60];
	char		 address[400];
	char		 postcode[20];
// 	OrderItemReq()
// 	{
// 		itemid = 0;
// 		itemnum = 0;
// 		name[0] = 0;
// 		email[0] = 0;
// 		address[0] = 0;
// 		postcode[0] = 0;
// 	}
};

// �һ�����
struct ExchangeItemReq
{
	unsigned int itemid;
	unsigned int itemnum;
};
// �������
struct BuyItemReq							
{
	unsigned int id;			//����ID
	unsigned int num;			//��������
	unsigned int source;		//������Դ
	BuyItemReq()
	{
		id = 0;
		num = 0;
		source = 0;
	}
};
#pragma pack(pop)

//����ģʽ
enum OPT_MODE {
	OPT_MODE_ADD_CROSS ,		//���ʮ�����
	OPT_MODE_ADD_CIRCLE ,       //���Բ�����
	OPT_MODE_ADD_WINDMILL,      //��ӷ糵���
	OPT_MODE_ADD_GEAR,          //��ӳ������
	OPT_MODE_ADD_CIRCLE_BALLHOLE = 1000,      //Բ����
	OPT_MODE_ADD_BALLHOLE1,        //��Բ���򶴣�ֻ��ֱ�������ܽ�ȥ��
	OPT_MODE_MAX
};
enum SCENE_GROUND				//��������
{
	GROUND_ellipse,             //��Բ����
	GROUND_AABB,                //���ε���
	GROUND_MAX
};
enum  SceneInterface
{
	LOADING_SCENE = 0,             //logo����
	ENTER_SCENE,                   //��¼����
	CHOSEGROUP_SCENE,              //ѡ�����������
	CREATECHARA_SCENE,             //������ɫ����
	FIGHT_SCENE,                   //ս������
	FUNCTION_SCENE,                //�����ܳ���
	SCENE_MAX
};
enum MesBoxState
{
	OK_CANCEL = 0,				/////ȷ���͹ر�//////
	SELL_BACK,					/////���ۺͷ���//////
	EMBATTLE_CONTINUE,			/////����ͼ���//////
	BUYSNAP_BACK				/////��������ͷ���/////
};
enum ItemPropType
{
	enum_item_prop_game = 0,		//��Ϸ���߷��� 0~99
	enum_item_prop_object = 100		//ʵ����߷��� 100~255
};
#endif