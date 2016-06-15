//////////发送logo_sever消息
void SendEnterGame(const char* name,const char* password);          //发送登录请求
void SendLeaveGame();												//发送登录请求
void SendSelgroup(const char* buf);                                 //发送获取游戏组列表
void SendDisLogoSever();											//请求与logo_sever断开

//////////发送game_sever消息
void SendHertMsg();                                                //发送心跳消息
void SendGetRole(const char* buf);                                 //请求登录验证
void SendGetRandCharaName();                                       //请求获得随机角色名
void SendCreateChara(const char* charaName);					   //请求创建角色

//////////发送道具相关消息
void SendGetPlayerPackInfo();                    //请求包裹数据，完整数据
void SendGetShoppingPropList();                  //请求获得商城列表
void SendBuyProp(int id,int num);                //商城购买道具
void SendUseProp(int propID,int num,int id);     //使用道具

//////////排行数据请求消息
void SendGetLadder();                            //请求获得排行数据前100位

//////////积分兑换数据信息
void SendGetExchangePropLisg();								//请求获得积分兑换商城列表
// void SendExchangeProp(unsigned int id,unsigned int num);	//兑换游戏内道具
void SendOrderProp(OrderItemReq  *item);					//兑换实物道具

//////////关卡
void SendGetChoseMapList();									//请求获得选择场景列表
void SendGetMapInfo(int id);								//请求场景数据
void SendChoseBall(int id);									//发送选择发射的小球
void SendComputerCore(int senceID,const char* str);         //结束游戏，请求结算

//////////充值
void SendGetBuyRollCallList();                    //获得充值界面列表
