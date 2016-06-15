#include "stdafx.h"
#include <time.h>
#include <iostream>
#include <algorithm>

#include "GameRoot.h"
#include "msg_def.h"
#include "CCharaDate.h"
#include "CPropInfo.h"
#include "Common.h"

#define RECV_PRIVATE
#define RECV_MSG(cmd,fun)	case cmd:	fun(buf,nLen);	 break;

#define DETECT_RETCODE(code)			if (code != 0)												\
										{															\
											ENTER_STATE(enum_state_idle);							\
											return;													\
										}

#define CHECK_RETCODE(ret,code)			if (code != 0)												\
										{															\
											ENTER_STATE(enum_state_idle);							\
											LOGFILE(code)													\
											if(ret) return;											\
										}
GameRoot::GameRoot()
	: _netmgr(this)
{
	mNetMgr = &_netmgr;
	mLog = &_log;
	mCharaInfo = &_charinfo;
}

void GameRoot::NetMsgLoop(float dt)
{
	if(mNetMgr)
		mNetMgr->MainLoop(dt);
}

bool GameRoot::MessageCallback(const char* buf,int nLen)
{
	if(nLen < sizeof(MSGHEADER))
	{
		LOGFILE(999)
		mLog->WriteLog(strLog,"recv data package len error,len=[%d]",nLen);
		//MsgBox("","recv data package len error,len=[%d]");
		return false;
	}
	MSGHEADER* pMsgHeader = (MSGHEADER*)buf;
	int nReserved = pMsgHeader->nReserved;

	time_t tmNow = time(NULL);
	switch(pMsgHeader->wCmd)
	{
	case LOGIN_S_NOT_HEART:
		if(nLen >= (sizeof(MSGHEADER)+sizeof(time_t)))
		{
			char* pCharTime =(char*)(buf+sizeof(MSGHEADER));
			time_t* pTime = (time_t*)pCharTime;
			tmNow = *pTime;
		}	
		break;
#include "MsgRecvDeclare.inl"
	default:
		{
			break;
		}
	}
// 	CCLog("[%s]recv2 data package,len=[%d],Cmd=[0x%08X],wRetCode=[%d],nReserved=[%d]",ctime(&tmNow),pMsgHeader->wLength,pMsgHeader->wCmd,pMsgHeader->wRetCode,pMsgHeader->nReserved);

	return true;
}

void GameRoot::EnterState(GameState state)
{
//  	MsgBox("",(int)state);
	if(state == enum_state_waiting)
		SetWaitTick();
	
	if(_state == enum_state_waiting && state == enum_state_idle)
	{
		SetMsgTick();
		if(GetRobotID() % 1000 == 0)
		{
			cout<<"["<<GetRobotName()<<"]:"<<GetEventID()<<","<<GetMsgTick()<<endl;
		}
	}

	_state = state;
}

int GameRoot::_randomx(int min,int max)
{
	return min + rand() % (max - min + 1);
}
//////////////////////////////////////////////////////////////
// GameServer Message Handler
//////////////////////////////////////////////////////////////

//错误消息与服务器断开了连接
void GameRoot::MSG_Disconnect(const char* buf,int nLen)
{
	ENTER_STATE(enum_state_offline);

	/////日志//////
// 	LOGFILE
// 	mLog->WriteLog(strLog,"断开连接，协议：0x%x",MS_S_ACK_DISCONNECT);
}

//接收登录是否成功，包头里的wRetCode为0是成功，1则是失败，成功并获得组服务器列表
void GameRoot::MSG_EnterGame(const char* buf,int nLen)
{
	MSGHEADER* pMsgHeader = (MSGHEADER*)buf;
	int offset = sizeof(MSGHEADER);
	//CHECK_RETCODE(false,pMsgHeader->wRetCode)
	if (pMsgHeader->wRetCode != 0)												
	{															
		ENTER_STATE(enum_state_offline);							
		LOGFILE(pMsgHeader->wRetCode)													
// 		mLog->WriteLog(strLog,__FUNCTION__"[%s] Fail:%ld",GetRobotName().c_str(), pMsgHeader->wRetCode);	
		if(1) return;											
	}

	if(!pMsgHeader->wRetCode)
	{
		if (pMsgHeader->nReserved == 1)
		{
			tagServerList *Server = new tagServerList;
			memcpy(Server,buf+offset,sizeof(tagServerList));
			offset+=sizeof(tagServerList);

			INT64  linkID;
			memcpy(&linkID,buf+offset,sizeof(INT64));
#if (1) //CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
			char temp[32];
			sprintf(temp,"%I64d/%d",linkID,Server->dwServerID);
#else
			char temp[32];
			sprintf(temp,"%lld/%d",linkID,Server->dwServerID);
#endif
			mNetMgr->SendSelgroup(temp);
			///////记录日志////////////
// 			LOGFILE
// 				mLog->WriteLog(strLog,"接收到一组服务器，组名/linkID/serverID:%s/%s,协议:0x%x",Server->ServerName,temp,LOGIN_S_ACK_AUTH);
		}else
		{
			tagServerList *Server = new tagServerList[pMsgHeader->nReserved];
			INT64  linkID;	

			for (int i=0;i<pMsgHeader->nReserved;i++)
			{
				memcpy(&Server[i],buf+offset,sizeof(tagServerList));
				offset+=sizeof(tagServerList);
			}

			memcpy(&linkID,buf+offset,sizeof(INT64));
			offset+=sizeof(INT64);

			///////记录日志////////////
			char temp[256]={0};
			char temp1[1024]={0};
			for (int i=0;i<pMsgHeader->nReserved;i++)
			{
				sprintf(temp,"serverName/serverID:%s/%d;",Server[i].ServerName,Server[i].dwServerID);
				strcpy(&temp1[strlen(temp1)],temp);
			}
#if (1) //CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
			sprintf(&temp1[strlen(temp1)],"linkID:%I64d",linkID);
#else
			sprintf(&temp1[strlen(temp1)],"linkID:%lld",linkID);
#endif
// 			LOGFILE
// 			mLog->WriteLog(strLog,"接收到%d组服务器;%s,协议：协议:0x%x",pMsgHeader->nReserved,temp1,LOGIN_S_ACK_AUTH);
		}
	}else
	{
		MsgBox("登录失败","请确认账号和密码\n是否正确");
	}
}
//接收服务器组IP,端口号，linkID
void GameRoot::MSG_GroupList(const char* buf,int nLen)
{
	mNetMgr->SendDisLogoSever();
	mNetMgr->DisConnect();
	mNetMgr->setIsSendHert(false);

	MSGHEADER* pMsgHeader = (MSGHEADER*)buf;
	int msgHead = sizeof(MSGHEADER);
	//CHECK_RETCODE(false,pMsgHeader->wRetCode)
	if (pMsgHeader->wRetCode != 0)												
	{															
		ENTER_STATE(enum_state_offline);							
		LOGFILE(pMsgHeader->wRetCode)													
// 		mLog->WriteLog(strLog,__FUNCTION__" fail:%ld",pMsgHeader->wRetCode);	
		if(1) return;											
	}

	UINT  severIp = 0;
	memcpy(&severIp,buf+msgHead,sizeof(UINT));
	msgHead+=sizeof(UINT);

	unsigned short  port = 0;
	memcpy(&port,buf + msgHead,sizeof(unsigned short));
	msgHead += sizeof(unsigned short);

	INT64  linkID;
	memcpy(&linkID,buf+msgHead,sizeof(INT64));

	struct in_addr stIp;
	stIp.S_un.S_addr = severIp;

	char* strip = inet_ntoa(stIp);
	//if(!mNetMgr->Initialize(strip,port,MessageCallbackRoot))
	if(!mNetMgr->Initialize(strip,port,NULL))
	{
		MsgBox("连接失败","网络异常");
	}

	char  temp[256];
	char  temp1[32]="5236254";//TODO
	sprintf(temp,"%s/%I64d/%s",_account.c_str(),linkID,temp1);
	mNetMgr->SendGetRole(temp);

	///////记录日志////////////
// 	LOGFILE
// 	mLog->WriteLog(strLog,"接收到服务器组IP/linkID/端口:%s,协议:0x%x",temp,LOGIN_S_ACK_SELGROUP);
}

//接收角色列表
void GameRoot::MSG_GetRoleList(const char* buf,int nLen)
{
	MSGHEADER* pMsgHeader = (MSGHEADER*)buf;
	int index = sizeof(MSGHEADER);
	//CHECK_RETCODE(1,pMsgHeader->wRetCode)
	if (pMsgHeader->wRetCode != 0)												
	{															
		ENTER_STATE(enum_state_offline);							
		LOGFILE(pMsgHeader->wRetCode)													
// 		mLog->WriteLog(strLog,__FUNCTION__" fail:%ld",pMsgHeader->wRetCode);	
		if(1) return;											
	}

	if(pMsgHeader->nReserved == 1)
	{
		///////记录日志////////////
// 		LOGFILE
// 		mLog->WriteLog(strLog,"连接gs成功，有角色,协议:0x%x",GS_CHARA_S_NOT_CHARALIST);
	}else
	{
		ENTER_STATE(enum_state_create);
		///////记录日志////////////
// 		LOGFILE
// 		mLog->WriteLog(strLog,"连接gs成功，但没有角色，需进入创建角色界面创建,协议:0x%x",GS_CHARA_S_NOT_CHARALIST);
	}
}

//接收随机角色名
// void GameRoot::MSG_GetRandCharaName(const char* buf,int nLen)
// {
// 	MSGHEADER* pMsgHeader = (MSGHEADER*)buf;
// 	int index = sizeof(MSGHEADER);
// 	CHECK_RETCODE(false,pMsgHeader->wRetCode)
// 
// 	char charaName[256]={0};
// 	if(nLen-index>0)
// 		memcpy(charaName,buf+index,nLen-index);
// 	else
// 	{
// 		LOGFILE
// 		mLog->WriteLog(strLog,"获得的角色名为空");
// 		return;
// 	}
// 
// 	///////记录日志////////////
// 	LOGFILE
// 	mLog->WriteLog(strLog,"获得随机角色名：%s,协议：0x%x",charaName,GS_CHARA_S_ACK_CHARNAME);
// }

//创建角色返回成功信息
void GameRoot::MSG_CreateCharaCallBack(const char* buf,int nLen)
{
	MSGHEADER* pMsgHeader = (MSGHEADER*)buf;
	int index = sizeof(MSGHEADER);
	//CHECK_RETCODE(false,pMsgHeader->wRetCode)
	if (pMsgHeader->wRetCode != 0)												
	{															
		ENTER_STATE(enum_state_offline);							
		LOGFILE(pMsgHeader->wRetCode)													
// 		mLog->WriteLog(strLog,__FUNCTION__" fail:%ld",pMsgHeader->wRetCode);	
		if(1) return;											
	}

	//GS_CHARA_S_ACK_CREATE  
	ENTER_STATE(enum_state_idle);

	///////记录日志////////////
// 	LOGFILE
// 	mLog->WriteLog(strLog,"创建角色成功协议：0x%x",GS_CHARA_S_ACK_CREATE);
}

//接收心跳消息
void GameRoot::MSG_ResiveHert(const char* buf,int nLen)
{
	MSGHEADER* pMsgHeader = (MSGHEADER*)buf;
	int index = sizeof(MSGHEADER);
	CHECK_RETCODE(false,pMsgHeader->wRetCode)

	////////记录日志////////
// 	LOGFILE
// 	mLog->WriteLog(strLog,"接收GS心跳，协议：0x%x",GS_S_NOT_HEART);

	mNetMgr->SendHertMsg();
}

//接收角色完整数据
void GameRoot::MSG_GetCharaAllInfo(const char* buf,int nLen)
{
	MSGHEADER* pMsgHeader = (MSGHEADER*)buf;
	int index = sizeof(MSGHEADER);
	//CHECK_RETCODE(1,pMsgHeader->wRetCode)
	if (pMsgHeader->wRetCode != 0)												
	{															
		ENTER_STATE(enum_state_offline);							
		LOGFILE(pMsgHeader->wRetCode)													
// 		mLog->WriteLog(strLog,__FUNCTION__" fail:%ld",pMsgHeader->wRetCode);	
		if(1) return;											
	}
	//GS_S_ACK_AUTH 
	unsigned char len  = 0;
	memcpy(&len,buf+index,sizeof(unsigned char));
	index+=sizeof(unsigned char);

	char temp[256];
	memcpy(temp,buf+index,sizeof(temp));
	index+=len;
	mCharaInfo->SetCardData(buf+index);

	////////记录日志////////
// 	LOGFILE
// 	mLog->WriteLog(strLog,"接收角色完整数据，协议：0x%x",GS_S_ACK_AUTH);
}

//接收人物背包信息
void GameRoot::MSG_GetPlayerPackInfo(const char* buf,int nLen)
{
	MSGHEADER* pMsgHeader = (MSGHEADER*)buf;
	int index = sizeof(MSGHEADER);
	//CHECK_RETCODE(false,pMsgHeader->wRetCode)
	if (pMsgHeader->wRetCode != 0)												
	{															
		ENTER_STATE(enum_state_offline);							
		LOGFILE(pMsgHeader->wRetCode)											
// 		mLog->WriteLog(strLog,__FUNCTION__" fail:%ld",pMsgHeader->wRetCode);	
		if(1) return;											
	}

	//count(int) + 
	//PropInfo * count

	int count = 0;
	memcpy(&count,buf+index,sizeof(int));
	index += sizeof(int);

	_itemlist.clear();
	for (int i = 0; i < count; i++)
	{
		PropInfo pif;
		memcpy(&pif,buf+index,sizeof(PropInfo));
		_itemlist.push_back(pif);
		index += sizeof(PropInfo);
	}

	ENTER_STATE(enum_state_idle);

	//OnLogin
	if(GetRobotID() % 100 == 0)
	{
		cout<<"["<<GetRobotName()<<"]: Login Success"<<":"<<GetMsgTick()<<endl;
	}

	////////记录日志////////
// 	LOGFILE
// 	mLog->WriteLog(strLog,"成功接收包裹数据，协议：0x%x",GS_S_PLAYER_ITEM_ACK);
}

//接收商城道具列表
void  GameRoot::MSG_GetShoppingPropList(const char* buf,int nLen)
{
	MSGHEADER* pMsgHeader = (MSGHEADER*)buf;
	int index = sizeof(MSGHEADER);

	CHECK_RETCODE(false,pMsgHeader->wRetCode)

	//count(int) + 
	//ShoppingPropInfo * count

	int count = 0;
	memcpy(&count,buf+index,sizeof(int));
	index += sizeof(int);

	_shoplist.clear();
	for (int i = 0;i<count;i++)
	{
		ShoppingPropInfo pif;
		memcpy(&pif,buf+index,sizeof(ShoppingPropInfo));
		_shoplist.push_back(pif);
		index += sizeof(ShoppingPropInfo);
	}

// 	ENTER_STATE(enum_state_shopping);

	////////记录日志////////
// 	LOGFILE
// 	mLog->WriteLog(strLog,"成功接收商城道具数据，协议：0x%x",GS_S_SHOP_ITEM_ACK);

	if(_shoplist.empty()) return;

	count = _shoplist.size();
	int i = _randomx(0,count -1);
	int num = _randomx(1,5);

	_netmgr.SendBuyProp(_shoplist[i].mPropID,num);
}

//商城购买道具
void  GameRoot::MSG_BuyProp(const char* buf,int nLen)
{
	MSGHEADER* pMsgHeader = (MSGHEADER*)buf;
	int index = sizeof(MSGHEADER);

	CHECK_RETCODE(false,pMsgHeader->wRetCode)

	//剩余货币(header.nReserved) +
	//剩余兑换券（uint）

	mCharaInfo->SetRollCall(pMsgHeader->nReserved);
	int num = 0;
	memcpy(&num,buf + index,sizeof(int));
	mCharaInfo->SetHoodleRoll(num);

	ENTER_STATE(enum_state_idle);

	////////记录日志////////
// 	LOGFILE
// 	mLog->WriteLog(strLog,"购买道具成功，协议：0x%x",GS_S_SHOP_BUY_ITEM_ACK);
}

//添加和更新道具
void GameRoot::MSG_AddAndSetProp(const char* buf,int nLen)
{
	MSGHEADER* pMsgHeader = (MSGHEADER*)buf;
	int index = sizeof(MSGHEADER);
	CHECK_RETCODE(false,pMsgHeader->wRetCode)

	//Count(int) + 
	//PropInfo * count
	//若propInfo.num = 0，客户端应删除相应instID

	int count = 0;
	memcpy(&count,buf + index,sizeof(int));
	index += sizeof(int);

	vector<PropInfo>::iterator it;
	for (int i = 0;i < count;i++)
	{
		PropInfo pif;
		memcpy(&pif,buf + index,sizeof(PropInfo));
		index += sizeof(PropInfo);

		it = std::find(_itemlist.begin(),_itemlist.end(),pif.mID);
		if(it != _itemlist.end())
		{
			if(pif.mPropNum == 0)
				_itemlist.erase(it);
			else
				*it = pif;
		}
	}
	////////记录日志////////
// 	LOGFILE
// 	mLog->WriteLog(strLog,"添加和更新道具，协议：0x%x",GS_S_PLAYER_ITEM_IND);
}

//使用道具
void GameRoot::MSG_UseProp(const char* buf,int nLen)
{
	MSGHEADER* pMsgHeader = (MSGHEADER*)buf;
	int index = sizeof(MSGHEADER);
	//CHECK_RETCODE(false,pMsgHeader->wRetCode)

	ENTER_STATE(enum_state_idle);

	////////记录日志////////
// 	LOGFILE
// 	mLog->WriteLog(strLog,"使用道具:result=%ld",pMsgHeader->wRetCode);
}

//获取排行榜信息
void GameRoot::MSG_GetLadderInfo(const char* buf,int nLen)
{
	MSGHEADER* pMsgHeader = (MSGHEADER*)buf;
	int index = sizeof(MSGHEADER);
	CHECK_RETCODE(false,pMsgHeader->wRetCode)

	//	玩家排名(Header.nReserved)
	//	+ int(累计积分清除剩余时间)
	//	+ count(int) 
	//	+ Count* GLOBALE_LADDER_INFO_CFG

	mCharaInfo->SetIntegralLadder(pMsgHeader->nReserved);
	int mTime = 0;
	memcpy(&mTime,buf+index,sizeof(int));
	index += sizeof(int);

	int count = 0;
	memcpy(&count,buf+index,sizeof(int));
	index += sizeof(int);

	// GLOBALE_LADDER_INFO_CFG list
	ENTER_STATE(enum_state_idle);

	////////记录日志////////
// 	LOGFILE
// 	mLog->WriteLog(strLog,"获得排行信息，协议：0x%x",GS_S_TOP500_INFO_ACK);
}

//常驻公告信息
void GameRoot::MSG_GetGongGaoInfo(const char* buf,int nLen)
{
	MSGHEADER* pMsgHeader = (MSGHEADER*)buf;
	int index = sizeof(MSGHEADER);
	CHECK_RETCODE(false,pMsgHeader->wRetCode)
	
	//	Count（header.nReserved）+
	//	strlen(int) + 公告1 +
	//	strlen(int) + 公告2 +
	//	…

	int count = pMsgHeader->nReserved;

	for (int i = 0;i < count;i++)
	{
		int len = 0;
		memcpy(&len,buf + index,sizeof(int));
		index += sizeof(int);
		char temp[256]={0};
		memcpy(temp,buf + index,len);
		index += len;
	}

	////////记录日志////////
// 	LOGFILE
// 	mLog->WriteLog(strLog,"常驻公告信息，协议：0x%x",GS_S_BOARDNOTICE_IND);
}

//玩家相关公告信息
void GameRoot::MSG_GetPlayerGongGaoInfo(const char* buf,int nLen)
{
	MSGHEADER* pMsgHeader = (MSGHEADER*)buf;
	int index = sizeof(MSGHEADER);
	CHECK_RETCODE(false,pMsgHeader->wRetCode)

	int mType = pMsgHeader->nReserved;
	
	//type:1-新手奖励 2-每日奖励
	//type(header.nReserved）+
	//count(int)+count * AwardPropInfo

	int count = 0;
	memcpy(&count,buf+index,sizeof(int));
	index += sizeof(int);

	for (int i = 0;i < count;i++)
	{
		AwardPropInfo  info;
		memcpy(&info,buf+index,sizeof(AwardPropInfo));
		index += sizeof(AwardPropInfo);
	}

	////////记录日志////////
// 	LOGFILE
// 	mLog->WriteLog(strLog,"玩家公告信息，协议：0x%x",GS_S_BOARDNOTICE_IND);
}

//接收积分兑换商城列表
void GameRoot::MSG_GetExchangePropList(const char* buf,int nLen)
{
	MSGHEADER* pMsgHeader = (MSGHEADER*)buf;
	int index = sizeof(MSGHEADER);
	CHECK_RETCODE(false,pMsgHeader->wRetCode)

	//Count(int) + 
	//ShoppingPropInfo * count

	int count = 0;
	memcpy(&count,buf+index,sizeof(int));
	index += sizeof(int);

	_exchangelist.clear();
	for(int i = 0; i < count; i++)
	{
		ShoppingPropInfo exchangeinfo;
		memcpy(&exchangeinfo,buf + index, sizeof(exchangeinfo));
		index += sizeof(exchangeinfo);
		_exchangelist.push_back(exchangeinfo);
	}

// 	ENTER_STATE(enum_state_exchanging);

	////////记录日志////////
// 	LOGFILE
// 	mLog->WriteLog(strLog,"成功接收积分兑换商城道具数据，协议：0x%x",GS_S_EXCHANGE_ITEM_ACK);

	if(_exchangelist.empty()) return;

	count = _exchangelist.size();
	int i = _randomx(0,count -1);
	int num = _randomx(1,5);

	OrderItemReq req = {0,0,"robot.name","robot.email","robot.address","robot.postcode"};
	req.itemid = _exchangelist[i].mPropID;
	req.itemnum = num;
	_netmgr.SendOrderProp(&req);
}

//兑换游戏内道具
// void GameRoot::MSG_ExchangeProp(const char* buf,int nLen)
// {
// 	MSGHEADER* pMsgHeader = (MSGHEADER*)buf;
// 	int index = sizeof(MSGHEADER);
// 	CHECK_RETCODE(false,pMsgHeader->wRetCode)
// 
// 	mCharaInfo->SetIntegral(pMsgHeader->nReserved);
// 
// 	////////记录日志////////
// 	LOGFILE
// 	mLog->WriteLog(strLog,"兑换游戏内道具成功，协议：0x%x",GS_S_EXCHANGE_ACK);
// }

//兑换实物道具
void GameRoot::MSG_OrderProp(const char* buf,int nLen)
{
	MSGHEADER* pMsgHeader = (MSGHEADER*)buf;
	int index = sizeof(MSGHEADER);
	CHECK_RETCODE(false,pMsgHeader->wRetCode)

	//剩余积分(header.nReserved)
	mCharaInfo->SetIntegral(pMsgHeader->nReserved);

	ENTER_STATE(enum_state_idle);

	////////记录日志////////
// 	LOGFILE
// 	mLog->WriteLog(strLog,"兑换实物道具成功，协议：0x%x",GS_S_ORDER_ACK);
}

//获得一个BUFF
void GameRoot::MSG_AddBuff(const char* buf,int nLen)
{
	MSGHEADER* pMsgHeader = (MSGHEADER*)buf;
	int index = sizeof(MSGHEADER);
	CHECK_RETCODE(false,pMsgHeader->wRetCode)

	//buffid（header.nReserved）
	//count(int) +
	//count * AcquireBuffInfo
	int  count = 0;
	memcpy(&count,buf+index,sizeof(int));
	index += sizeof(int);

	for (int i = 0;i < count; i++)
	{
		AcquireBuffInfo buff;
		memcpy(&buff,buf+index,sizeof(AcquireBuffInfo));
		index += sizeof(AcquireBuffInfo);
		_bufflist[buff.index] = buff;
	}

	////////记录日志////////
// 	LOGFILE
// 	mLog->WriteLog(strLog,"添加一个BUFF，协议：0x%x",GS_S_BUFF_ACQUIRE_IND);
}

//失去一个BUFF
void GameRoot::MSG_LoseBuff(const char* buf,int nLen)
{
	MSGHEADER* pMsgHeader = (MSGHEADER*)buf;
	int index = sizeof(MSGHEADER);
	CHECK_RETCODE(false,pMsgHeader->wRetCode)

	//buffid（header.nReserved）
	//index(int)

	int bufIndex = 0;
	memcpy(&bufIndex,buf+index,sizeof(int));
	index += sizeof(int);

	bufIndex = pMsgHeader->nReserved;

	map<int,AcquireBuffInfo>::iterator it = _bufflist.find(bufIndex);
	if(it != _bufflist.end())
		_bufflist.erase(it);

	////////记录日志////////
// 	LOGFILE
// 	mLog->WriteLog(strLog,"失去一个BUFF，协议：0x%x",GS_S_BUFF_LOST_IND);
}

//获得选择关卡列表
void GameRoot::MSG_GetChoseMapList(const char* buf,int nLen)
{
	MSGHEADER* pMsgHeader = (MSGHEADER*)buf;
	int index = sizeof(MSGHEADER);
	CHECK_RETCODE(false,pMsgHeader->wRetCode)

	int count = 0;
	memcpy(&count,buf+index,sizeof(int));
	index += sizeof(int);
	
	//Count (int) + 
	//Count * StageInfo
	_stagelist.clear();
	for(int i = 0; i < count; i++)
	{
		ChoseMapListST stage;
		memcpy(&stage,buf + index,sizeof(ChoseMapListST));
		_stagelist.push_back(stage);
		index += sizeof(ChoseMapListST);
	}
	
// 	ENTER_STATE(enum_state_playing);

	////////记录日志////////
// 	LOGFILE
// 	mLog->WriteLog(strLog,"获得选择场景列表，协议：0x%x",GS_S_STAGE_LIST_ACK);

	if(_stagelist.empty()) return;

	count = _stagelist.size();
	int i = _randomx(0,count -1);

	_stageid = _stagelist[i].mMapID;
	_netmgr.SendGetMapInfo(_stagelist[i].mMapID);

}

//获得场景数据
void GameRoot::MSG_GetMapInfo(const char* buf,int nLen)
{
	MSGHEADER* pMsgHeader = (MSGHEADER*)buf;
	int index = sizeof(MSGHEADER);
	CHECK_RETCODE(1,pMsgHeader->wRetCode)

	//Scene_date +
	//Count * Ball_Data
	//Count * BlockObj
	int len = nLen - index;
	memcpy(&_scenedata,buf + index,sizeof(Scene_date));
	index += sizeof(Scene_date);
	int   mIndex = index + sizeof(BlockObj);

	_balllist.clear();
	if (1)
	{
		Ball_date ball;
		memcpy(&ball,buf + index,sizeof(Ball_date));
		index += sizeof(Ball_date);
		mIndex = index + sizeof(Ball_date);
		_balllist.push_back(ball);
	}
	if (1)
	{
		Ball_date ball;
		memcpy(&ball,buf + index,sizeof(Ball_date));
		index += sizeof(Ball_date);
		mIndex = index + sizeof(Ball_date);
		_balllist.push_back(ball);
	}
	if (1)
	{
		Ball_date ball;
		memcpy(&ball,buf + index,sizeof(Ball_date));
		index += sizeof(Ball_date);
		mIndex = index + sizeof(Ball_date);
		_balllist.push_back(ball);
	}

	_blocklist.clear();
	while (mIndex <= len)
	{
		BlockObj blockobj;
		memcpy(&blockobj,buf + index, sizeof(BlockObj));
		index += sizeof(BlockObj);
		_blocklist.push_back(blockobj);
		mIndex = index + sizeof(BlockObj);
	}

	////////记录日志////////
// 	LOGFILE
// 	mLog->WriteLog(strLog,"获得场景数据，协议：0x%x",GS_S_STAGE_START_ACK);

	//选择小球
	if(_balllist.empty()) return;

	int count = _balllist.size();
	int i = _randomx(0,count - 1);
	if(_balllist[i].mBallID != 0)
	_netmgr.SendChoseBall(_balllist[i].mBallID);
}

//发射小球返回
void GameRoot::MSG_SendBallReback(const char* buf,int nLen)
{
	MSGHEADER* pMsgHeader = (MSGHEADER*)buf;
	int index = sizeof(MSGHEADER);
	CHECK_RETCODE(0,pMsgHeader->wRetCode)

	//剩余弹球兑换券(header.nReserved)
	mCharaInfo->SetHoodleRoll(pMsgHeader->nReserved);

	////////记录日志////////
// 	LOGFILE
// 	mLog->WriteLog(strLog,"发射小球成功，协议：0x%x",GS_S_STAGE_BALLSEL_ACK);

	{
		if(_blocklist.empty()) return;

		int count = _blocklist.size();
		int i = _randomx(0,count - 1);
		_netmgr.SendComputerCore(_stageid,_blocklist[i].iteamID);
	}
}

//获得游戏结算结果
void GameRoot::MSG_ComputerCore(const char* buf,int nLen)
{
	MSGHEADER* pMsgHeader = (MSGHEADER*)buf;
	int index = sizeof(MSGHEADER);
	CHECK_RETCODE(0,pMsgHeader->wRetCode)

	//Score(header.nReserved)	//本次得分
	//CurScore（int）			//当前积分
	//Allscore（int）			//累计积分

	int  num = 0;
	memcpy(&num,buf+index,sizeof(int));
	index += sizeof(int);
	mCharaInfo->SetIntegral(num);

	memcpy(&num,buf+index,sizeof(int));
	index += sizeof(int);
	mCharaInfo->SetPileIntegral(num);

	ENTER_STATE(enum_state_idle);

	////////记录日志////////
// 	LOGFILE
// 	mLog->WriteLog(strLog,"游戏结算成功成功，协议：0x%x",GS_S_STAGE_SCORE_ACK);
}

//获得充值界面列表
void GameRoot::MSG_GetBuyRollCallList(const char* buf,int nLen)
{
	MSGHEADER* pMsgHeader = (MSGHEADER*)buf;
	int index = sizeof(MSGHEADER);
	CHECK_RETCODE(false,pMsgHeader->wRetCode)
	
	//count（int） +
	//sizeof(ShoppingPropInfo) * count

	int count = 0;
	memcpy(&count,buf+index,sizeof(int));
	index += sizeof(int);

	for (int i = 0;i < count; i++)
	{
		ShoppingPropInfo prop;
		memcpy(&prop,buf + index,sizeof(ShoppingPropInfo));
		index += sizeof(ShoppingPropInfo);
	}

	ENTER_STATE(enum_state_idle);
	////////记录日志////////
// 	LOGFILE
// 	mLog->WriteLog(strLog,"获得充值列表，协议：0x%x",GS_S_COST_LIST_ACK);
}