/******************************************************************************
* Copyright (c) 2013,qiqu
* All rights reserved.
* 
* 文件名称：msg_def.h
* 摘    要：消息定义和相关数据结构定义
* 
* 当前版本：1.0
* 作    者：clauschen
* 建立日期：2013年7月25日
**********************************************************************************/

#pragma once 

#define MAX_CHAR_NAME		24

#pragma pack(push ,1)

//=======================================
// 消息ID
//=======================================
enum SS_MSG_DEFINE
{
	////////////////////////////通用消息////////////////////////
	MS_CHAR_SVR_MSG_START					=0x1000,
	MS_S_ACK_DISCONNECT,							//客户被断掉	

	/////////////服务器服务器消息/////////////////////////////////////
	GS_LOGIN_SVR_MSG_START						=0x2000,
	LOGIN_S_NOT_HEART,								//连接维持信号(保留字段: 序号)DWORD CurPing
	LOGIN_C_ACK_HEART,								//返回连接维持信号(保留字段: 序号)

	LOGIN_C_REQ_AUTH,								//用户登陆请求 包头保留字段放版本号version(4字节) 包体:userid/passwd/mibao  例子:mlgm020/123456/1234567890
	LOGIN_S_ACK_AUTH,								//ok-携带组list信息及Linkid   ; fail-
	//包头wRetCode-存放成功或失败 成功时：nReserved-服务器组个数 包体结构:nReserved*sizeof(tagServerList)+Linkid(int64)
	//失败时，无包体信息
	LOGIN_C_REQ_SELGROUP,							//用户选组请求 Linkid/groupindex(即serverid)
	LOGIN_S_ACK_SELGROUP,							//返回Linkid/group_gsip/port
	//	
	LOGIN_C_REQ_LEAVEGAME,							//客户主动退出游戏		
	///////////////////////////////////////////////////
	GS_GAME_SVR_MSG_START						=0x3000,
	GS_S_NOT_HEART,								//连接维持信号(保留字段: 序号)DWORD CurPing
	GS_C_ACK_HEART,								//返回连接维持信号(保留字段: 序号)	

	GS_C_REQ_AUTH,								//用户登陆验证((保留字段:角色ID)DWORD 客户端版本号;BYTE 登陆数据长度;登陆数据; userinfo(PTID/lsTransID/macAddr))
	GS_S_ACK_AUTH,								//认证成功，则包体：BYTE byLoginDataLen+pLoginData+CHARACTER_INFO CharInfo(+其它，需要时在追加)

	GS_C_REQ_LEAVEGAME,							//客户退出游戏
	//
	GS_CHARA_SVR_MSG_START						=0x300A,
	GS_CHARA_S_NOT_CHARALIST,						//角色列表 保留字段放0-无角色(无包体) 1-有角色，则直接进入主界面
	GS_CHARA_C_REQ_CHARNAME,						//随机角色名
	GS_CHARA_S_ACK_CHARNAME,						//包体放 随机的角色名
	GS_CHARA_C_REQ_CREATE,							//创建角色  保留字段放 角色索引  包体放填入的角色名
	GS_CHARA_S_ACK_CREATE,							//响应创建角色 错误 则提示信息;成功 则进入主界面

	GS_CHARA_SVR_MSG_END						=0x302A,

	GS_C_REQ_ITEM_LIST,							//请求道具列表
	GS_S_ACK_ITEM_LIST,					

	//布阵相关
	GS_C_REQ_CARD_HELP,						//请求卡牌援助
	GS_C_ACK_CARD_HELP,
	GS_C_REQ_SET_LINEUP,					//请求设置出阵阵型
	GS_C_ACK_SET_LINEUP,
	//
	GS_C_REQ_JOIN_DUPMAP,					//请求进入副本
	GS_C_ACK_JOIN_DUPMAP,
	//
	GS_C_REQ_USE_GM_COMMAND,				//请求发送gm命令 string sCommand
	GS_S_ACK_USE_GM_COMMAND,				//返回gm命令处理结果 string

	GS_S_NOT_SYSADMINLEVEL,					//通知玩家他当前的系统管理员的级别(保留字段: 当前级别(-1为非管理员, 0为最高))

	//排行相关
	GS_C_TOP500_INFO_REQ,					//请求前500排行信息
	GS_C_MYLADDER_INFO_REQ,					//我的排名
	GS_S_TOP500_INFO_ACK,					//返回排行数据
	GS_C_GAME_OVER_IND,						//一局游戏结束后需发送得分，更新排名

	//道具相关
	GS_C_PLAYER_ITEM_REQ,					//请求角色道具
	GS_S_PLAYER_ITEM_ACK,					//回复角色道具
	GS_C_USE_ITEM_REQ,						//使用道具
	GS_S_USE_ITEM_ACK,						//回复使用道具
	GS_S_PLAYER_ITEM_IND,					//角色道具变化通知

	//商城 
	GS_C_SHOP_ITEM_REQ,						//请求商城道具
	GS_S_SHOP_ITEM_ACK,						//回复系统道具
	GS_C_SHOP_BUY_ITEM_REQ,					//购买商城道具
	GS_S_SHOP_BUY_ITEM_ACK,					//购买商城道具回复

	//兑换
	GS_C_ORDER_REQ,							//兑换实物道具
	GS_S_ORDER_ACK,							//订购实物道具回复
	GS_C_EXCHANGE_REQ,						//兑换游戏道具
	GS_S_EXCHANGE_ACK,						//兑换游戏道具回复
	GS_C_EXCHANGE_ITEM_REQ,					//请求兑换道具列表
	GS_S_EXCHANGE_ITEM_ACK,					//请求兑换道具列表回复

	//公告
	GS_S_BOARDNOTICE_IND,					//系统公告消息
	GS_S_PLAYER_BOARDNOTICE_IND,			//玩家公告消息

	//Buff
	GS_S_BUFF_ACQUIRE_IND,					//获得某个Buff及相关的效果描述
	GS_S_BUFF_LOST_IND,						//失去某个Buff的特效

	//关卡
	GS_C_STAGE_LIST_REQ,					//请求关卡列表
	GS_S_STAGE_LIST_ACK,					//关卡列表回复
	GS_C_STAGE_START_REQ,					//请求关卡数据
	GS_S_STAGE_START_ACK,					//回复关卡数据
	GS_C_STAGE_UNLOCK_REQ,					//请求解锁关卡
	GS_S_STAGE_UNLOCK_ACK,					//回复解锁关卡
	GS_C_STAGE_SCORE_REQ,					//请求计算得分
	GS_S_STAGE_SCORE_ACK,					//回复得分
	GS_C_STAGE_BALLSEL_REQ,					//选择的弹球
	GS_S_STAGE_BALLSEL_ACK,					//选择的弹球回复

	//充值
	GS_C_COST_BUY_REQ,						//点券充值
	GS_S_COST_BUY_ACK,						//回复点券充值
	GS_C_COST_LIST_REQ,						//请求充值列表
	GS_S_COST_LIST_ACK,						//回复充值列表

	GS_S_SCORE_CHANGE_IND,					//积分改变通知

	GS_C_COST_VERIFY_REQ,					//苹果充值验证
	GS_S_COST_VERIFY_ACK,					//苹果充值验证返回

};

//返回信息
typedef enum EMSRetCode
{
	//错误种类
	eMSRetSuccess = 0,											//操作成功
	eMSRetUnknow,												//未知错误
	eMSRetDBProcessError,										//数据库操作错误
	eMSRetSLKProcessError,										//SLK操作错误
	eMSRetAuthenFailed,											//验证失败
	eMSRetPTIDLengthError,										//平台账号长度不对,6~10
	eMSRetPTPwdLengthError,										//平台密码长度不对,6~10
	eMSRetPTPswError,											//平台帐户密码不匹配
	eMSRetPTInfoEmpty,											//平台请求帐号或密码为空
	eMSRetPTIDInvalid,											//平台无帐号 或帐号无效
	eMSRetPTError,												//平台错误

	eMSRetInvalidCharName,										//角色名错误
	eMSRetCharaNotExist,										//角色不存在
	eMSRetCharaExist,											//角色已存在

	eMSRetCharaCountFull,										//角色数目已达上限

	eMSRetServerGroupNotExist,									//服务器组不存在
	eMSRetMemoryError,											//内存错误
	eMSRetComponentError,										//组件错误

	eMsRetMoreInvalidCharName,									//您使用的无效角色名次数过多，请重新登录

	eMSRetSysNotCommentParaError,								//系统是否不评分选项错误
	eMSRetGameModalError,										//游戏模式错误

	eMSRetShowDateTypeError,									//表现数据类型错误
	eMSRetChatChannelCountExc,									//超过聊天频道的限制
	eMSRetPeerChatChannelCountExc,								//对方聊天频道超过限制
	eMSRetChatDestClientNotInvalid,								//聊天目标用户无效	
	eMSRetCharSrcClientNotInvalid,								//聊天源用户无效
	eMSRetDestClientChatPeer,									//目标用户是聊天同伴
	eMSRetDestClientNotChatPeer,								//目标用户不是聊天同伴
	eMSReteChatNotInvited,										//不是邀请的聊天客户
	eMSRetChatDestClientNotAgreen,								//聊天目标用户不同意
	eMSRetChatContentTooLong,									//聊天内容太长
	eMSRetTetgetPlayerOffline,									//对方不在线
	eMSRetNotGM,												//不是GM
	eMSRetGM,													//GM
	eMSRetItemTargetError,										//道具目标玩家不存在
	eMSRetItemNotExist,											//道具不存在
	eMSRetNotEnoughItem,										//没有足够的道具
	eMSRetTargetMyself,											//目标是自己
	eMSRetItemDecFailed,										//减少道具失败

	eMSRetParamError,											//参数错误

	eMSRetErrorPlayer,											//错误的角色
	eMSRetErrorPlayerStatus,									//错误的玩家状态
	eMSRetHaveSelectSupport,									//已经选择过支持者了

	eMSRetNotInMiniGame,										//没有在小游戏中
	eMSRetNoEnoughVirtualMoney,									//没有足够的虚拟币

	eMSRetNoData,												//没有数据
	eMSRetAlreadyInFriend,										//已经是好友了
	eMSRetInvalidFriendGroup,									//非法的好友分组
	eMSRetNotInFriend,											//不是好友了

	eMSRetRefuseFriend,											//拒绝成为好友
	eMSRetOfflineMsgFull,										//离线消息已满
	eMSRetNoOperationRight,										//没有操作权限

	eMSRetRoleFull,												//角色已达上限
	eMSRetEquipPosError,										//装备位置不正确
	eMSRetNoMatchEquipCondition,								//装备条件不符合
	eMSRetNoMatchUseCondition,									//使用条件不符合
	eMSRetInvalidItem,											//非法的道具

	//GM命令相关
	eMSRetGmParamCountError,						//GM命令参数个数不正确
	eMSRetInvalidTimeFormat,						//时间格式无效
	eMSRetTimeHasPast,								//企图使用的时间已过期

	eMSRetInvalidOrderType,							//排序类型错误
	eMSRetNotFoundBanPlayer,						//未找到此玩家的阻止信息
	eMSRetDontInputHere,							//请您不要在此处输入文字

	//帐号限制
	eMSRetPTIDIsBlocked,							//此帐号被停用或者不允许登陆此服务器

	eMSDisConnectNoError = 0x1000,
	eMSDisConnectUnknow,										//未知错误
	eMSDisConnectMsgLenLessThanMsgHeader,						//包长度小于包头
	eMSDisConnectNotAuthenedClient,								//未认证客户
	eMSDisConnectSendInvalidMsg,								//发送未知消息
	eMSDisConnectAlreadyAuthened,								//已经认证
	eMSDisConnectLoginDataError,								//登陆数据错误
	eMSDisConnectPacketFormatError,								//数据包格式错误
	eMSDisConnectAuthenFailed,									//验证失败
	eMSDisConnectKickUser,										//被踢下线
	eMSDisConnectCommuError,									//通讯错误
	eMSDisConnectLongNotSendAthen,								//长时间没有发送登陆包
	eMSDisConnectPlatformError,									//平台错误
	eMSDisConnectSeriousError,									//严重错误
	eMSDisConnectVersionError,									//版本错误
	eMSDisConnectClientFull,									//服务器客户端数达到上限
	eMSDisConnectTimeOut,										//网络连接超时
	eMSDisConnectAbuseAuditor,									//滥发作弊审核包
	eMSDisConnectEstopLogin,									//此玩家被禁止登陆
	eMSDisConnectSamePTIDLogin,									//同一帐号重复登陆
	eMSDisConnectPTIDIsBlocked,									//此帐号被停用或者不允许登陆此服务器

	//道具
	eMSRetItemQueryFail,						//道具查询失败
	eMSRetItemNotFound,							//没有该道具
	eMSRetItemAddFail,							//添加道具失败
	eMSRetItemReachMax,							//道具背包满
	eMSRetItemIDIlegal,							//道具ID非法
	eMSRetItemNumNotEnough,						//道具数量不够
	eMSRetItemFuncQueryFail,					//道具功能查询失败
	eMSRetItemFuncNotFound,						//道具功能未定义
	eMSRetItemCannotUse,						//道具无法使用
	eMSRetItemCannotEquip,						//道具无法装备
	eMSRetItemInstNotFound,						//道具实例未找到
	eMSRetExchangeItemQueryFail,				//兑换实物道具查询失败
	eMSRetExchangeItemNotFound,					//兑换实物道具没有配置
	eMSRetExchangeOrderFail,					//兑换实物DB保存失败
	eMSRetExchangeItemForbid,					//没有设置价格，故不能兑换
	eMSRetItemNumCannotBeZero,					//数量不能为0

	//积分 4133
	eMSRetScoreNotEnough,						//积分不够

	//奖励
	eMSRetBonusQueryFail,						//奖励查询失败
	eMSRetBonusNotFound,						//奖励未配置

	//Buff
	eMSRetBuffQueryFail,						//Buff查询失败
	eMSRetBuffNotFound,							//Buff ID未找到

	//商城
	eMSRetBuyItemForbid,						//价格为0，不能购买
	eMSRetPointNotEnough,						//点券不够

	//关卡 4140
	eMSRetNoStageUnlockCard,					//没有关卡解锁卡
	eMSRetStageIdNotFound,						//没有找到该关卡ID
	eMSRetStageAlreadyUnlock,					//该关卡ID已经解锁
	eMSRetStageLocked,							//该关卡ID未解锁
	eMSRetStageBlockItemNotFound,				//关卡数据中物件没找到
	eMSRetGetBallCostFail,						//小球消费没找到
	eMSRetGetBlockScoreFail,					//小洞得分没找到

	//货币
	eMSRetMoneyAddFail,
	eMSRetMoneyValueIllegal,					//增加点券值<=0
	eMSRetMoneyNotEnough,						//点券不足
	eMSRetExchangePointAddFail,
	eMSRetExchangePointValueIllegal,			//购买兑换券数目<=0
	eMSRetExchangePointNotEnough,				//兑换券不足
	eMSRetCostItemIdNotFound,					//充值项ID没找到

	eMSRetHeartBeatBreak,						//心跳消息收不到

	eMSRetCostVerifyFail,						//appstore验证失败
	eMSRetCostVerifyNetError,					//appstore验证网络错误
	eMSRetCostVerifyDataInvalid,				//充值验证字符串没收到
	eMSRetCostBuyPending,						//等待充值记录写数据库

	eMSRetMax,
}MSRETCODE;

#if 0

//============================================================================
// 道具相关
//============================================================================

#define		MAX_ITEM_COUNT			50					//包裹道具最大数量

#define		MAX_STR					24					//道具名称
#define		MAX_PROPDEPICT			256					//道具描述

enum ItemPropType
{
	enum_item_prop_game = 0,		//游戏道具分类 0~99
	enum_item_prop_object = 100		//实物道具分类 100~255
};

// 角色获得道具数据公告
// struct AwardPropInfo
// {
// 	unsigned int	mNumber;
// 	char            mPropName[MAX_STR];					//道具名称
// };

// 返回给客户端的角色道具数据
// struct PropInfo                                         
// {
// 	unsigned int    mID;                                //动态实例ID，唯一标识
// 	unsigned int    mPropID;							//道具ID
// 	unsigned short	mImageID;							//图片索引
// 	unsigned char   mType;                              //道具类型
// 	unsigned char   mPropNum;							//道具数量
// 	unsigned char	mEquipped;							//是否已装备
// 	unsigned int    mEquipable;                         //是否可装备  道具价值
// 	unsigned int    mTime;								//道具时效时间	
// 	char            mPropName[MAX_STR];					//道具名称
// 	char            mPropDepict[MAX_PROPDEPICT];        //道具描述
// };

// 返回给客户端的商城道具数据
// struct ShoppingPropInfo                                 //商城道具数据结构
// {
// 	unsigned int    mPropID;							//道具ID
// 	unsigned int    mMoney;                             //道具价值
// 	unsigned short	mImageID;							//图片索引
// 	unsigned char   mType;                              //道具分类
// 	char            mPropName[MAX_STR];					//道具名称
// 	char            mPropDepict[MAX_PROPDEPICT];        //道具描述
// };

// 购买道具
// struct BuyItemReq							
// {
// 	unsigned int id;			//道具ID
// 	unsigned int num;			//道具数量
// 	unsigned int source;		//道具来源
// };

// 使用道具
// struct ClickItemReq							
// {
// 	unsigned int id;			//道具ID
// 	unsigned int num;			//道具数量
// 	unsigned int instID;		//道具实例ID
// };

// 订购请求
// struct OrderItemReq
// {
// 	unsigned int itemid;
// 	unsigned int itemnum;
// 	char		 name[20];
// 	char		 email[60];
// 	char		 address[400];
// 	char		 postcode[20];
// };

// 兑换请求
// struct ExchangeItemReq
// {
// 	unsigned int itemid;
// 	unsigned int itemnum;
// };

// 购买请求
//typedef ExchangeItemReq ShopBuyItemReq;

//============================================================================
// 排行相关
//============================================================================

// 排行数据
typedef struct GLOBALE_LADDER_INFO 
{
	uint32			uIconID;					//玩家头像
	uint32			uAllScore;					//累计积分
	char			szCharName[MAX_CHAR_NAME];	//角色名称	
}GLOBALE_LADDER_INFO_CFG,*PGLOBALE_LADDER_INFO;

//============================================================================
// BUFF相关
//============================================================================
#define MAX_BUFF_DESC_LEN	100

// Buff的一个效果的数据
// struct AcquireBuffInfo
// {
// 	int index;						//效果索引	
// 	int duration;					//持续时间
// 	int buffid;						//buff类型
// 	float value;					//buff数值
// 	unsigned int icon;				//效果图标
// 	char desc[MAX_BUFF_DESC_LEN];	//描述
// };
#endif

#pragma pack(pop)
