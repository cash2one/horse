#ifndef _COMMON_H_
#define _COMMON_H_
/*
	这个文件存放全局的常量 协议等
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
	unsigned char  mType;                //物理物件的类型，主要有3种，b2_staticBody:静止的组件。b2_kinematicBody:运动学组件。b2_dynamicBody:动态组件
	unsigned int   mShape;               //物件形状类型
	unsigned int   imageID;				 //图片索引
	float          mDensity;			 //物件的密度
	float          mRestitution;         //物件的回复值 0到1之间的值
	float          mFriction;			 //摩擦力
	float		    MotorTorque;         //这个值用于绑有旋转关节的物件，如果这个有值，则需要设置关节马达。
	float			mLineSpeed;          //这个值用于可以在世界里来回运动的物件。
	float			mAngleSpeed;         //旋转速度，用于创建旋转的物件
	float           mAngle;              //初始旋转角度
	float           mX;                  //X坐标
	float           mY;                  //Y坐标
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
	unsigned  char    mSceneID;				 //场景ID，用数字表示
	unsigned  char    mSceneShape;           //场景形状
// 	unsigned  char    mBall1;                // 1号球
// 	unsigned  char    mBall2;                // 1号球
// 	unsigned  char    mBall3;                // 1号球
	float			  mGravity;              //重力值
	unsigned   int    mMinImage;			 //场景图片名字
	unsigned   int    mMaxImage;			 //场景图片名字
	char              mSceneName[MAX_STR];   //场景名字
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
	unsigned char mBallID;           //小球ID
	unsigned int  mImageID;          //小球图片名字
	float         mDensity;			 //小球密度
	float         mFriction;         //小球摩擦力
	float         mRestitution;      //小球弹力，回复度
	Ball_date()
	{
		mBallID = 0;
		mImageID = 0;
		mDensity = 0;
		mFriction = 0;
		mRestitution = 0;
	}
};

//记录IP，大区，端口的结构体
struct  choseIP
{
	char name[MAX_STR];			//大区名字
	char ip[MAX_STR];				//ip地址
	unsigned int	port;		//端口号
	unsigned char   state;		//服务器状态 
	unsigned char	isgroom;	//是否推荐
	choseIP()
	{
		port = 0;
		state = 0;
		isgroom = 0;
	}
};
//记录账号和密码的结构
struct  ZhangHao
{
	char  mZhang[MAX_STR];
	char  mPassWord[MAX_STR];
};
//人物信息结构体
struct BaseCharaInfo
{	
	char					szPTID[MAX_PTID];						//平台ID
	INT64					iDigitalPTID;                           //数字ID

	char					szCharName[MAX_CHAR_NAME];				//角色名称	
	unsigned int            mIntegral;								//当前积分
	unsigned int            mPileIntegral;                          //累计积分
	unsigned int			mRollCall;								//点卷
	unsigned int            mHoodleRoll;                            //弹珠兑换卷
	unsigned int 			mIntegralLadder;						//积分排名	
	unsigned int            mTimeVIP;                               //VIP剩余时间
	unsigned short			mMaxPack;								//包裹最大容量
	unsigned int 			nReserv;								//保留
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
	unsigned int			nClubID;                                    //工会ID
	short			sTitle1;											//称号1
	short			sTitle2;											//称号2		
	unsigned char			nDifficultyLevel;									//难度等级
	unsigned char			nShopDiscount;										//购物折扣,等等
	unsigned short			wReserv	;											//	保留
	unsigned int			dwBtnStatus;										//主界面button控制

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
// 道具类型
enum ItemFuncType
{
	enum_item_func_type_start = 0x00,
	enum_item_func_score_bonus = 0x01,					//多倍积分卡
	enum_item_func_ball_exchange,						//获得弹珠兑换券
	enum_item_func_stage_unlock,						//解锁场景
	enum_item_func_vip_card,							//VIP卡
	enum_item_func_type_num
};

//道具结构
struct PropInfo
{
	unsigned int    mID;                                //动态ID，唯一标石
	unsigned int    mPropID;							//道具ID
	unsigned short	mImageID;							//图片索引   
	unsigned char   mType;                              //道具类型    2：多倍积分卡，3：弹珠兑换卷，4：场景解锁卡，5：VIP卡
	unsigned char   mPropNum;							//道具数量
	unsigned char   mIsDress;                           //是否装备中，  0：没有，1：装备中
	unsigned int    mDress;                           //是否是能装备  0：不能装备，1：能装备
	unsigned int    mTime;								//道具时效时间
	char            mPropName[MAX_STR];					//道具名称
	char            mPropDepict[MAX_PROPDEPICT];        //道具描述
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

//商城道具数据结构
struct ShoppingPropInfo
{
	unsigned int    mPropID;							//道具ID
	unsigned int    mMoney;                             //道具价值
	unsigned short	mImageID;							//图片索引
	unsigned char   mType;                              //道具分类
	char            mPropName[MAX_STR];					//道具名称
	char            mPropDepict[MAX_PROPDEPICT];        //道具描述
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
//排行榜信息结构
struct LadderInfo
{
	unsigned int    mLadder;                            //排行
	unsigned int	mPileIntegral;                      //玩家累计积分
	char			szCharName[MAX_CHAR_NAME];			//角色名称	
	LadderInfo()
	{
		mLadder = 0;
		mPileIntegral = 0;
		szCharName[0] = 0;
	}
};
//场景选择信息排行
struct ChoseMapListST
{
	unsigned int    mMapID;                             //场景编号ID
	unsigned int    mImageID;                           //选择场景图片编号
	unsigned char   mLock;                              //是否锁定
	unsigned char	mBall[3];
	char            mName[MAX_STR];                     //场景名字
};
// 角色获得道具数据公告
struct AwardPropInfo
{
	unsigned int	mNumber;
	char            mPropName[MAX_STR];					//道具名称
};
#define MAX_BUFF_DESC_LEN	100

enum BuffType
{
	enum_buff_type_start = 0x00,
	enum_buff_score_bonus,			//多倍积分卡
	enum_buff_vip_level,			//VIP卡	
	enum_buff_type_num
};
// Buff的一个效果的数据
struct AcquireBuffInfo
{
	int index;						//效果索引	
	int duration;					//持续时间
	int mType;                      //buff类型
	float    mValue;                //积分倍数
	unsigned int icon;				//效果图标
	char desc[MAX_BUFF_DESC_LEN];	//描述
};

// 订购请求
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

// 兑换请求
struct ExchangeItemReq
{
	unsigned int itemid;
	unsigned int itemnum;
};
// 购买道具
struct BuyItemReq							
{
	unsigned int id;			//道具ID
	unsigned int num;			//道具数量
	unsigned int source;		//道具来源
	BuyItemReq()
	{
		id = 0;
		num = 0;
		source = 0;
	}
};
#pragma pack(pop)

//操作模式
enum OPT_MODE {
	OPT_MODE_ADD_CROSS ,		//添加十字组件
	OPT_MODE_ADD_CIRCLE ,       //添加圆形组件
	OPT_MODE_ADD_WINDMILL,      //添加风车组件
	OPT_MODE_ADD_GEAR,          //添加齿轮组件
	OPT_MODE_ADD_CIRCLE_BALLHOLE = 1000,      //圆形球洞
	OPT_MODE_ADD_BALLHOLE1,        //半圆形球洞，只有直线区域能进去。
	OPT_MODE_MAX
};
enum SCENE_GROUND				//场景地形
{
	GROUND_ellipse,             //椭圆地形
	GROUND_AABB,                //矩形地形
	GROUND_MAX
};
enum  SceneInterface
{
	LOADING_SCENE = 0,             //logo场景
	ENTER_SCENE,                   //登录场景
	CHOSEGROUP_SCENE,              //选择服务器场景
	CREATECHARA_SCENE,             //创建角色场景
	FIGHT_SCENE,                   //战斗场景
	FUNCTION_SCENE,                //主功能场景
	SCENE_MAX
};
enum MesBoxState
{
	OK_CANCEL = 0,				/////确定和关闭//////
	SELL_BACK,					/////出售和返回//////
	EMBATTLE_CONTINUE,			/////布阵和继续//////
	BUYSNAP_BACK				/////购买活力和返回/////
};
enum ItemPropType
{
	enum_item_prop_game = 0,		//游戏道具分类 0~99
	enum_item_prop_object = 100		//实物道具分类 100~255
};
#endif