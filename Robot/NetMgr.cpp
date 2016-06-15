#include "stdafx.h"
#include "NetMgr.h"
#include "GameRoot.h"
#include <time.h>

using namespace std;


NetMngr::NetMngr()
{
	m_pClientSocket = NULL;
	m_pTcpEncryption = NULL;
	isSendHert = true;
	m_bInit = false;
	memset(g_sEncryptPack,0,sizeof(g_sEncryptPack));
}

NetMngr::NetMngr(GameRoot* root)
	:mGameRoot(root)
{
	m_pClientSocket = NULL;
	m_pTcpEncryption = NULL;
	isSendHert = true;
}

NetMngr::~NetMngr()
{
	CC_SAFE_DELETE(m_pClientSocket);
	CC_SAFE_DELETE(m_pTcpEncryption);
}

bool NetMngr::Initialize(const char* szIP,int nPort,PMessageCallbackRoot pOnMessageCallback)
{
	m_pOnMessageCallback = pOnMessageCallback;

	if(m_pClientSocket == NULL)
	{
// 		LOGFILE
// 		mLog->WriteLog(strLog,"GS心跳，m_pClientSocket new");

		m_pClientSocket = new CGameSocket;
		if(NULL == m_pClientSocket)
			return false;
	}else
	{
// 		LOGFILE
// 		mLog->WriteLog(strLog,"GS心跳，m_pClientSocket != NULL");

		return false;
	}

	//
	if(NULL == m_pTcpEncryption)
	{
		m_pTcpEncryption = new CTcpEncryption;
		if(NULL == m_pTcpEncryption)
			return false;
		m_pTcpEncryption->InitEncrypt(sizeof(unsigned short));
	}
	//
	m_strCliPacket.clear();
	m_LastHeartTime = time(NULL);
	//
	m_bInit = m_pClientSocket->Create(szIP,nPort);
	return m_bInit;
}
//
void NetMngr::MainLoop(float dt)
{
	// 接收消息处理（放到游戏主循环中，每帧处理）
	if (!m_pClientSocket) {
		return;
	}
	//发送心跳消息
	time_t tmNow = time(NULL);
	if((tmNow - m_LastHeartTime) > 10)
	{
		if(isSendHert)
		{
			NoticeHeartTest();
			m_LastHeartTime = tmNow;
		}
	}
	//
	if (!m_pClientSocket->Check()) {
		m_pClientSocket = NULL;
		// 掉线了
		//onConnectionAbort();
		return;
	}

	// 发送数据（向服务器发送消息）
	m_pClientSocket->Flush();

	// 接收数据（取得缓冲区中的所有消息，直到缓冲区为空）
	while (true)
	{
		char buffer[_MAX_MSGSIZE] = { 0 };
		int nSize = sizeof(buffer);
		char* pbufMsg = buffer;
		if(m_pClientSocket == NULL)
		{
			break;
		}
		if (!m_pClientSocket->ReceiveMsg(pbufMsg, nSize)) 
			break;

		while (true)
		{
			MSGHEADER* pReceiveMsg = (MSGHEADER*)(pbufMsg);
			unsigned short	dwCurMsgSize = pReceiveMsg->wLength;			

			if((int)dwCurMsgSize > nSize || dwCurMsgSize <= 0) 
			{	// broken msg
				break;
			}

			//这个是外部处理函数
			if(mGameRoot)
			{
				/// TEST echo 为何过段时间断了，没有收发数据了
				//if(pReceiveMsg->wCmd == 101) SendEnterGame("", "");

				//m_pTcpEncryption->DecryptPack(pbufMsg, pReceiveMsg->wLength);
				//mGameRoot->MessageCallback((const char*)pReceiveMsg, pReceiveMsg->wLength);//error C2064: 项不会计算为接受 2 个参数的函数
			}

// 			if(m_pOnMessageCallback)
// 			{
// 				m_pTcpEncryption->DecryptPack(pbufMsg, pReceiveMsg->wLength);
// 				m_pOnMessageCallback((const char*)pReceiveMsg, pReceiveMsg->wLength);//error C2064: 项不会计算为接受 2 个参数的函数
// 				//修改为:
// 				//(this->*m_pOnMessageCallback)((const char*)pReceiveMsg, pReceiveMsg->wLength);
// 			}

			pbufMsg	+= dwCurMsgSize;
			nSize	-= dwCurMsgSize;
			if(nSize <= 0) 
				break;
		}
	}
}
//
void NetMngr::SendClientMsg(bool bIsIndirect /*= true*/)
{
	if(m_strCliPacket.size())
	{
		//m_pTcpEncryption->EncryptPack(m_strCliPacket.c_str(),g_sEncryptPack,(int)m_strCliPacket.size());
		if(bIsIndirect)
			m_pClientSocket->SendIndirectMsg((void*)m_strCliPacket.data(),(int)m_strCliPacket.size());
// 			m_pClientSocket->SendIndirectMsg(g_sEncryptPack,(int)m_strCliPacket.size());
		else
			m_pClientSocket->SendMsg(g_sEncryptPack,(int)m_strCliPacket.size());
	}	
}
//
void NetMngr::NoticeHeartTest()
{	
	if(m_pClientSocket)
	{
		/// TEST
//		SetMsgHeader(LOGIN_C_ACK_HEART,0,999);		
// 		SendClientMsg();
// 		LOGFILE
// 			mLog->WriteLog(strLog,"发送LOGO心跳，协议：%d",LOGIN_C_ACK_HEART);
	}	
}

bool NetMngr::OnMessage(const char* buf,int nLen)
{	
	//
	if(mGameRoot)
		return mGameRoot->MessageCallback(buf,nLen);
	return false;
}
//
void NetMngr::DisConnect()
{//loginserver切换到gameserver时需要
	if (m_pClientSocket)
	{
		m_pClientSocket->Reset();
	}
	//
	CC_SAFE_DELETE(m_pClientSocket);
	CC_SAFE_DELETE(m_pTcpEncryption);
}

bool NetMngr::isCannet()
{
	if (m_pClientSocket)
		return true;
	else
		return false;
}


//发送登录请求
void NetMngr::SendEnterGame(const char* name,const char* password)
{
	if(m_pClientSocket)
	{
		SetMsgHeader(100/*LOGIN_C_REQ_AUTH*/,0,100);

// 		char temp[32];
// 		sprintf(temp,"%s/%s/0",name,password);
// 		m_strCliPacket.append(temp,strlen(temp));
		string msg(4*1024, 'x');
		m_strCliPacket.append(msg.data(),msg.length());

		SetMsgLength();
		SendClientMsg();

//  		LOGFILE
//  		mLog->WriteLog(strLog,"登录请求，账号：%s;密码%s;协议：0x%x",name,password,LOGIN_C_REQ_AUTH);
// 		LOGFILE
// 		sprintf(strLog,"账号：%s;密码%s;协议：0x%x \n",name,password,LOGIN_C_REQ_AUTH);
// 		OutputDebugStringA(strLog);
	}	
}

//主动断开
void NetMngr::SendLeaveGame()
{
	if(m_pClientSocket)
	{
		SetMsgHeader(GS_C_REQ_LEAVEGAME,0);
		SendClientMsg();

// 		LOGFILE
// 		mLog->WriteLog(strLog,"LeaveGame，0x%x",GS_C_REQ_LEAVEGAME);
	}	
}

//发送选取的服务器组
void NetMngr::SendSelgroup(const char* buf)
{
	if(m_pClientSocket)
	{
		SetMsgHeader(LOGIN_C_REQ_SELGROUP,0,0);
		m_strCliPacket.append(buf,strlen(buf));
		SetMsgLength();
		SendClientMsg();

// 		LOGFILE
// 		mLog->WriteLog(strLog,"发送选取的服务器组;linkID/serverID:%s,协议：0x%x",buf,LOGIN_C_REQ_SELGROUP);
	}
}
//请求与logo_sever断开
void NetMngr::SendDisLogoSever()
{
	if(m_pClientSocket)
	{
		SetMsgHeader(LOGIN_C_REQ_LEAVEGAME,0,0);
		SendClientMsg();

// 		LOGFILE
// 		mLog->WriteLog(strLog,"发送与LOGO_SERVER断开,协议：0x%x",LOGIN_C_REQ_LEAVEGAME);
	}
}
//请求登录验证
void NetMngr::SendGetRole(const char* buf)
{
	if(m_pClientSocket)
	{
		extern int GameVersion;
		SetMsgHeader(GS_C_REQ_AUTH,0,0);
		unsigned int banben = GameVersion;//claus
		m_strCliPacket.append((char*)&banben,sizeof(unsigned int));
		char temp[256]="robotclient";
		char  dwSize = strlen(temp);
		m_strCliPacket.append((char*)&dwSize,sizeof(char));
		m_strCliPacket.append(temp,dwSize);
		m_strCliPacket.append(buf,strlen(buf));
		SetMsgLength();
		SendClientMsg();

// 		LOGFILE
// 			mLog->WriteLog(strLog,"请求gs验证，发送：账号/linkID/macID : %s,协议：0x%x",buf,GS_C_REQ_AUTH);
	}
}
//请求获得随机角色名
void NetMngr::SendGetRandCharaName()
{
	if(m_pClientSocket)
	{
		SetMsgHeader(GS_CHARA_C_REQ_CHARNAME,0,0);
		SendClientMsg();

// 		LOGFILE
// 			mLog->WriteLog(strLog,"请求随机角色名，协议：0x%x",GS_CHARA_C_REQ_CHARNAME);
	}
}
//请求创建角色
void NetMngr::SendCreateChara(const char* charaName)
{
	if(!m_pClientSocket)
		return;

	SetMsgHeader(GS_CHARA_C_REQ_CREATE,0,1001);
	m_strCliPacket.append(charaName,strlen(charaName));
	SetMsgLength();
	SendClientMsg();

// 	LOGFILE
// 		mLog->WriteLog(strLog,"请求创建角色,角色名:%s,协议:0x%x",charaName,GS_CHARA_C_REQ_CREATE);
}
//发送心跳消息
void NetMngr::SendHertMsg()
{
	if(!m_pClientSocket)
	{
// 		LOGFILE
// 			mLog->WriteLog(strLog,"发送GS心跳，m_pClientSocket = null");
		return;
	}

	SetMsgHeader(GS_C_ACK_HEART,0,0);
	SendClientMsg();

// 	LOGFILE
// 		mLog->WriteLog(strLog,"发送GS心跳，协议：0x%x",GS_C_ACK_HEART);
}
//请求包裹数据，完整数据
void NetMngr::SendGetPlayerPackInfo()
{
	if(!m_pClientSocket)
		return;
	SetMsgHeader(GS_C_PLAYER_ITEM_REQ,0,0);
	SendClientMsg();

// 	LOGFILE
// 		mLog->WriteLog(strLog,"发送获得包裹数据，协议：0x%x",GS_C_PLAYER_ITEM_REQ);
}
//请求获得商城列表
void NetMngr::SendGetShoppingPropList()
{
	if(!m_pClientSocket)
		return;
	SetMsgHeader(GS_C_SHOP_ITEM_REQ,0,0);
	SendClientMsg();

// 	LOGFILE
// 		mLog->WriteLog(strLog,"发送获得商城列表，协议：0x%x",GS_C_SHOP_ITEM_REQ);
}
//商城购买道具
void NetMngr::SendBuyProp(int id,int num)
{
	if(!m_pClientSocket)
		return;
	SetMsgHeader(GS_C_SHOP_BUY_ITEM_REQ,0,0);
	BuyItemReq  item;
	item.id = id;
	item.num = num;
	item.source = 0;
	m_strCliPacket.append((char*)&item,sizeof(BuyItemReq));
	SetMsgLength();
	SendClientMsg();

// 	LOGFILE
// 		mLog->WriteLog(strLog,"购买道具，协议：0x%x",GS_C_SHOP_BUY_ITEM_REQ);
}
//请求获得排行数据前100位
void NetMngr::SendGetLadder()
{
	if(!m_pClientSocket)
		return;
	SetMsgHeader(GS_C_TOP500_INFO_REQ,0,0);
	SendClientMsg();

// 	LOGFILE
// 		mLog->WriteLog(strLog,"请求排行榜数据，协议：0x%x",GS_C_TOP500_INFO_REQ);
}
//使用道具
void NetMngr::SendUseProp(int propID,int num,int id)
{
	if(!m_pClientSocket)
		return;
	SetMsgHeader(GS_C_USE_ITEM_REQ,0,0);
	m_strCliPacket.append((char*)&propID,sizeof(int));
	m_strCliPacket.append((char*)&num,sizeof(int));
	m_strCliPacket.append((char*)&id,sizeof(int));
	SetMsgLength();
	SendClientMsg();

// 	LOGFILE
// 	mLog->WriteLog(strLog,"使用道具，协议：0x%x",GS_C_USE_ITEM_REQ);
}
//请求获得积分兑换商城列表
void NetMngr::SendGetExchangePropLisg()
{
	if(!m_pClientSocket)
		return;
	SetMsgHeader(GS_C_EXCHANGE_ITEM_REQ,0,0);
	SendClientMsg();

// 	LOGFILE
// 		mLog->WriteLog(strLog,"发送获得积分兑换商城列表，协议：0x%x",GS_C_EXCHANGE_ITEM_REQ);
}

//兑换游戏内道具
// void NetMngr::SendExchangeProp(unsigned int id,unsigned int num)
// {
// 	if(!m_pClientSocket)
// 		return;
// 
// 	SetMsgHeader(GS_C_EXCHANGE_REQ,0,0);
// 	ExchangeItemReq   item;
// 	item.itemid = id;
// 	item.itemnum = num;
// 	m_strCliPacket.append((char*)&item,sizeof(ExchangeItemReq));
// 	SetMsgLength();
// 	SendClientMsg();
// 
// 	LOGFILE
// 		mLog->WriteLog(strLog,"发送兑换游戏内道具，协议：0x%x",GS_C_EXCHANGE_REQ);
// }

//兑换实物道具
void NetMngr::SendOrderProp(OrderItemReq  *item)
{
	if(!m_pClientSocket)
		return;
	SetMsgHeader(GS_C_ORDER_REQ,0,0);
	m_strCliPacket.append((char*)item,sizeof(OrderItemReq));
	SetMsgLength();
	SendClientMsg();

// 	LOGFILE
// 		mLog->WriteLog(strLog,"发送兑换实物道具，协议：0x%x",GS_C_ORDER_REQ);
}
//请求获得选择场景列表
void NetMngr::SendGetChoseMapList()
{
	if(!m_pClientSocket)
		return;
	SetMsgHeader(GS_C_STAGE_LIST_REQ,0,0);
	SendClientMsg();

// 	LOGFILE
// 		mLog->WriteLog(strLog,"发送获得选择场景列表，协议：0x%x",GS_C_STAGE_LIST_REQ);
}
//请求场景数据
void NetMngr::SendGetMapInfo(int id)
{
	if(!m_pClientSocket)
		return;
	SetMsgHeader(GS_C_STAGE_START_REQ,0,id);
	SendClientMsg();

// 	LOGFILE
// 	mLog->WriteLog(strLog,"发送获得场景数据，协议：0x%x",GS_C_STAGE_START_REQ);
}
//发送选择发射的小球
void NetMngr::SendChoseBall(int id)
{
	if(!m_pClientSocket)
		return;
	SetMsgHeader(GS_C_STAGE_BALLSEL_REQ,0,id);
	SendClientMsg();

// 	LOGFILE
// 		mLog->WriteLog(strLog,"发送选择小球，协议：0x%x",GS_C_STAGE_BALLSEL_REQ);
}
//结束游戏，请求结算
void NetMngr::SendComputerCore(int senceID,const char* str)
{
	if(!m_pClientSocket)
		return;
	SetMsgHeader(GS_C_STAGE_SCORE_REQ,0,senceID);
	m_strCliPacket.append(str,strlen(str));
	SetMsgLength();
	SendClientMsg();

// 	LOGFILE
// 	mLog->WriteLog(strLog,"发送游戏结算，协议：0x%x",GS_C_STAGE_SCORE_REQ);
}

//获得充值界面列表
void NetMngr::SendGetBuyRollCallList()
{
	if(!m_pClientSocket)
		return;

	SetMsgHeader(GS_C_COST_LIST_REQ,0,0);
	SendClientMsg();
	
	////////
// 	LOGFILE
// 	mLog->WriteLog(strLog,"发送获得充值界面列表，协议：0x%x",GS_C_COST_LIST_REQ);
}