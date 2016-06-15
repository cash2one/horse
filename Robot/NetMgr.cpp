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
// 		mLog->WriteLog(strLog,"GS������m_pClientSocket new");

		m_pClientSocket = new CGameSocket;
		if(NULL == m_pClientSocket)
			return false;
	}else
	{
// 		LOGFILE
// 		mLog->WriteLog(strLog,"GS������m_pClientSocket != NULL");

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
	// ������Ϣ�����ŵ���Ϸ��ѭ���У�ÿ֡����
	if (!m_pClientSocket) {
		return;
	}
	//����������Ϣ
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
		// ������
		//onConnectionAbort();
		return;
	}

	// �������ݣ��������������Ϣ��
	m_pClientSocket->Flush();

	// �������ݣ�ȡ�û������е�������Ϣ��ֱ��������Ϊ�գ�
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

			//������ⲿ������
			if(mGameRoot)
			{
				/// TEST echo Ϊ�ι���ʱ����ˣ�û���շ�������
				//if(pReceiveMsg->wCmd == 101) SendEnterGame("", "");

				//m_pTcpEncryption->DecryptPack(pbufMsg, pReceiveMsg->wLength);
				//mGameRoot->MessageCallback((const char*)pReceiveMsg, pReceiveMsg->wLength);//error C2064: ������Ϊ���� 2 �������ĺ���
			}

// 			if(m_pOnMessageCallback)
// 			{
// 				m_pTcpEncryption->DecryptPack(pbufMsg, pReceiveMsg->wLength);
// 				m_pOnMessageCallback((const char*)pReceiveMsg, pReceiveMsg->wLength);//error C2064: ������Ϊ���� 2 �������ĺ���
// 				//�޸�Ϊ:
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
// 			mLog->WriteLog(strLog,"����LOGO������Э�飺%d",LOGIN_C_ACK_HEART);
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
{//loginserver�л���gameserverʱ��Ҫ
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


//���͵�¼����
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
//  		mLog->WriteLog(strLog,"��¼�����˺ţ�%s;����%s;Э�飺0x%x",name,password,LOGIN_C_REQ_AUTH);
// 		LOGFILE
// 		sprintf(strLog,"�˺ţ�%s;����%s;Э�飺0x%x \n",name,password,LOGIN_C_REQ_AUTH);
// 		OutputDebugStringA(strLog);
	}	
}

//�����Ͽ�
void NetMngr::SendLeaveGame()
{
	if(m_pClientSocket)
	{
		SetMsgHeader(GS_C_REQ_LEAVEGAME,0);
		SendClientMsg();

// 		LOGFILE
// 		mLog->WriteLog(strLog,"LeaveGame��0x%x",GS_C_REQ_LEAVEGAME);
	}	
}

//����ѡȡ�ķ�������
void NetMngr::SendSelgroup(const char* buf)
{
	if(m_pClientSocket)
	{
		SetMsgHeader(LOGIN_C_REQ_SELGROUP,0,0);
		m_strCliPacket.append(buf,strlen(buf));
		SetMsgLength();
		SendClientMsg();

// 		LOGFILE
// 		mLog->WriteLog(strLog,"����ѡȡ�ķ�������;linkID/serverID:%s,Э�飺0x%x",buf,LOGIN_C_REQ_SELGROUP);
	}
}
//������logo_sever�Ͽ�
void NetMngr::SendDisLogoSever()
{
	if(m_pClientSocket)
	{
		SetMsgHeader(LOGIN_C_REQ_LEAVEGAME,0,0);
		SendClientMsg();

// 		LOGFILE
// 		mLog->WriteLog(strLog,"������LOGO_SERVER�Ͽ�,Э�飺0x%x",LOGIN_C_REQ_LEAVEGAME);
	}
}
//�����¼��֤
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
// 			mLog->WriteLog(strLog,"����gs��֤�����ͣ��˺�/linkID/macID : %s,Э�飺0x%x",buf,GS_C_REQ_AUTH);
	}
}
//�����������ɫ��
void NetMngr::SendGetRandCharaName()
{
	if(m_pClientSocket)
	{
		SetMsgHeader(GS_CHARA_C_REQ_CHARNAME,0,0);
		SendClientMsg();

// 		LOGFILE
// 			mLog->WriteLog(strLog,"���������ɫ����Э�飺0x%x",GS_CHARA_C_REQ_CHARNAME);
	}
}
//���󴴽���ɫ
void NetMngr::SendCreateChara(const char* charaName)
{
	if(!m_pClientSocket)
		return;

	SetMsgHeader(GS_CHARA_C_REQ_CREATE,0,1001);
	m_strCliPacket.append(charaName,strlen(charaName));
	SetMsgLength();
	SendClientMsg();

// 	LOGFILE
// 		mLog->WriteLog(strLog,"���󴴽���ɫ,��ɫ��:%s,Э��:0x%x",charaName,GS_CHARA_C_REQ_CREATE);
}
//����������Ϣ
void NetMngr::SendHertMsg()
{
	if(!m_pClientSocket)
	{
// 		LOGFILE
// 			mLog->WriteLog(strLog,"����GS������m_pClientSocket = null");
		return;
	}

	SetMsgHeader(GS_C_ACK_HEART,0,0);
	SendClientMsg();

// 	LOGFILE
// 		mLog->WriteLog(strLog,"����GS������Э�飺0x%x",GS_C_ACK_HEART);
}
//����������ݣ���������
void NetMngr::SendGetPlayerPackInfo()
{
	if(!m_pClientSocket)
		return;
	SetMsgHeader(GS_C_PLAYER_ITEM_REQ,0,0);
	SendClientMsg();

// 	LOGFILE
// 		mLog->WriteLog(strLog,"���ͻ�ð������ݣ�Э�飺0x%x",GS_C_PLAYER_ITEM_REQ);
}
//�������̳��б�
void NetMngr::SendGetShoppingPropList()
{
	if(!m_pClientSocket)
		return;
	SetMsgHeader(GS_C_SHOP_ITEM_REQ,0,0);
	SendClientMsg();

// 	LOGFILE
// 		mLog->WriteLog(strLog,"���ͻ���̳��б�Э�飺0x%x",GS_C_SHOP_ITEM_REQ);
}
//�̳ǹ������
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
// 		mLog->WriteLog(strLog,"������ߣ�Э�飺0x%x",GS_C_SHOP_BUY_ITEM_REQ);
}
//��������������ǰ100λ
void NetMngr::SendGetLadder()
{
	if(!m_pClientSocket)
		return;
	SetMsgHeader(GS_C_TOP500_INFO_REQ,0,0);
	SendClientMsg();

// 	LOGFILE
// 		mLog->WriteLog(strLog,"�������а����ݣ�Э�飺0x%x",GS_C_TOP500_INFO_REQ);
}
//ʹ�õ���
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
// 	mLog->WriteLog(strLog,"ʹ�õ��ߣ�Э�飺0x%x",GS_C_USE_ITEM_REQ);
}
//�����û��ֶһ��̳��б�
void NetMngr::SendGetExchangePropLisg()
{
	if(!m_pClientSocket)
		return;
	SetMsgHeader(GS_C_EXCHANGE_ITEM_REQ,0,0);
	SendClientMsg();

// 	LOGFILE
// 		mLog->WriteLog(strLog,"���ͻ�û��ֶһ��̳��б�Э�飺0x%x",GS_C_EXCHANGE_ITEM_REQ);
}

//�һ���Ϸ�ڵ���
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
// 		mLog->WriteLog(strLog,"���Ͷһ���Ϸ�ڵ��ߣ�Э�飺0x%x",GS_C_EXCHANGE_REQ);
// }

//�һ�ʵ�����
void NetMngr::SendOrderProp(OrderItemReq  *item)
{
	if(!m_pClientSocket)
		return;
	SetMsgHeader(GS_C_ORDER_REQ,0,0);
	m_strCliPacket.append((char*)item,sizeof(OrderItemReq));
	SetMsgLength();
	SendClientMsg();

// 	LOGFILE
// 		mLog->WriteLog(strLog,"���Ͷһ�ʵ����ߣ�Э�飺0x%x",GS_C_ORDER_REQ);
}
//������ѡ�񳡾��б�
void NetMngr::SendGetChoseMapList()
{
	if(!m_pClientSocket)
		return;
	SetMsgHeader(GS_C_STAGE_LIST_REQ,0,0);
	SendClientMsg();

// 	LOGFILE
// 		mLog->WriteLog(strLog,"���ͻ��ѡ�񳡾��б�Э�飺0x%x",GS_C_STAGE_LIST_REQ);
}
//���󳡾�����
void NetMngr::SendGetMapInfo(int id)
{
	if(!m_pClientSocket)
		return;
	SetMsgHeader(GS_C_STAGE_START_REQ,0,id);
	SendClientMsg();

// 	LOGFILE
// 	mLog->WriteLog(strLog,"���ͻ�ó������ݣ�Э�飺0x%x",GS_C_STAGE_START_REQ);
}
//����ѡ�����С��
void NetMngr::SendChoseBall(int id)
{
	if(!m_pClientSocket)
		return;
	SetMsgHeader(GS_C_STAGE_BALLSEL_REQ,0,id);
	SendClientMsg();

// 	LOGFILE
// 		mLog->WriteLog(strLog,"����ѡ��С��Э�飺0x%x",GS_C_STAGE_BALLSEL_REQ);
}
//������Ϸ���������
void NetMngr::SendComputerCore(int senceID,const char* str)
{
	if(!m_pClientSocket)
		return;
	SetMsgHeader(GS_C_STAGE_SCORE_REQ,0,senceID);
	m_strCliPacket.append(str,strlen(str));
	SetMsgLength();
	SendClientMsg();

// 	LOGFILE
// 	mLog->WriteLog(strLog,"������Ϸ���㣬Э�飺0x%x",GS_C_STAGE_SCORE_REQ);
}

//��ó�ֵ�����б�
void NetMngr::SendGetBuyRollCallList()
{
	if(!m_pClientSocket)
		return;

	SetMsgHeader(GS_C_COST_LIST_REQ,0,0);
	SendClientMsg();
	
	////////
// 	LOGFILE
// 	mLog->WriteLog(strLog,"���ͻ�ó�ֵ�����б�Э�飺0x%x",GS_C_COST_LIST_REQ);
}