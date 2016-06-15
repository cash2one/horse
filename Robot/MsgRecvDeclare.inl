//接收消息的宏
RECV_PRIVATE
	//LOGO_SEVER消息
	RECV_MSG(MS_S_ACK_DISCONNECT          ,MSG_Disconnect)          /*错误消息与服务器断开了连接*/
	RECV_MSG(LOGIN_S_ACK_AUTH             ,MSG_EnterGame)           /*接收登录是否成功，包头里的wRetCode为0是成功，1则是失败*/
	RECV_MSG(LOGIN_S_ACK_SELGROUP         ,MSG_GroupList)           /*接收服务器组IP,端口号，linkID*/
	//GAME_GS消息
	RECV_MSG(GS_S_NOT_HEART               ,MSG_ResiveHert)          //接收心跳消息
	RECV_MSG(GS_CHARA_S_NOT_CHARALIST     ,MSG_GetRoleList)         /*接收角色列表*/
// 	RECV_MSG(GS_CHARA_S_ACK_CHARNAME	  ,MSG_GetRandCharaName)     /*接收随机角色名*/
	RECV_MSG(GS_CHARA_S_ACK_CREATE        ,MSG_CreateCharaCallBack)  /*创建角色返回成功信息*/
	RECV_MSG(GS_S_ACK_AUTH                ,MSG_GetCharaAllInfo)      //接收人物完整信息
	//道具相关
	RECV_MSG(GS_S_PLAYER_ITEM_ACK         ,MSG_GetPlayerPackInfo)	 //接收人物背包信息  直接保存在包裹，不需要更新界面
	RECV_MSG(GS_S_PLAYER_ITEM_IND         ,MSG_AddAndSetProp)        //背包道具更新
	RECV_MSG(GS_S_USE_ITEM_ACK            ,MSG_UseProp)              //使用道具
	//商城
	RECV_MSG(GS_S_SHOP_ITEM_ACK           ,MSG_GetShoppingPropList)  //接收商城道具列表
	RECV_MSG(GS_S_SHOP_BUY_ITEM_ACK       ,MSG_BuyProp)              //购买道具
	//排行数据消息
	RECV_MSG(GS_S_TOP500_INFO_ACK         ,MSG_GetLadderInfo)        //获取排行榜信息
	//公告
	RECV_MSG(GS_S_BOARDNOTICE_IND         ,MSG_GetGongGaoInfo)       //常驻系统公告
	RECV_MSG(GS_S_PLAYER_BOARDNOTICE_IND  ,MSG_GetPlayerGongGaoInfo) //玩家公告消息
	//积分兑换
	RECV_MSG(GS_S_EXCHANGE_ITEM_ACK       ,MSG_GetExchangePropList)   //获得积分商城列表
// 	RECV_MSG(GS_S_EXCHANGE_ACK            ,MSG_ExchangeProp)          //兑换游戏内道具
	RECV_MSG(GS_S_ORDER_ACK               ,MSG_OrderProp)             //兑换实物道具
	//BUFF相关
	RECV_MSG(GS_S_BUFF_ACQUIRE_IND        ,MSG_AddBuff)                //获得一个BUFF
	RECV_MSG(GS_S_BUFF_LOST_IND           ,MSG_LoseBuff)               //失去一个buff
	//关卡相关
	RECV_MSG(GS_S_STAGE_LIST_ACK          ,MSG_GetChoseMapList)        //获得选择关卡列表
	RECV_MSG(GS_S_STAGE_START_ACK         ,MSG_GetMapInfo)             //获得场景数据
	RECV_MSG(GS_S_STAGE_BALLSEL_ACK       ,MSG_SendBallReback)         //发射小球返回
	RECV_MSG(GS_S_STAGE_SCORE_ACK         ,MSG_ComputerCore)           //获得游戏结算结果
	//充值相关 
	RECV_MSG(GS_S_COST_LIST_ACK           ,MSG_GetBuyRollCallList)      //获得充值界面列表