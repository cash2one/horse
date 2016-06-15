/******************************************************************************
* Copyright (c) 2003,上海盛大网络研发中心
* All rights reserved.
* 
* 文件名称：ProcessThread.cpp
* 摘    要：SS服务器的通讯线程
* 
* 当前版本：1.0
* 作    者：Xu Chengyong
* 建立日期：2005年12月01日
**********************************************************************************/
#include "StdAfx.h"
#include "../audioctrl/AudioServerMgr.h"
#include ".\produceintergladder.h"
#include "DBTransCheckAwardTable.h"
#include ".\deletestonepicture.h"
#define NOTICE_MAXLENGTH			1024
//#include "Tools.h"
#define cmdline_reloadgameconf		"ReloadGameData"
#define cmdline_reloadgmlist		"ReloadGmList"
#define cmdline_listserver			"ListServer"
#define cmdline_addnotice			"AddNotice"
#define cmdline_clearnotice			"ClearNotice"
#define cmdline_listnotice			"ListNotice"
#define cmdline_setdebuglog			"SetDebugLog"
#define cmdline_settracelog			"SetTraceLog"
#define cmdline_setdblog			"SetDBLog"
#define cmdline_seterrorlog			"SetErrorLog"
#define cmdline_showloglevel		"ShowLogLevel"
#define cmdline_enablescreenlog		"EnableScreenLog"

#ifdef _HAVING_CN_
#define cmdline_enableawardno		"EnableAwardNo"
#define cmdline_showawardnoflag		"ShowAwardNoFlag"
#endif //_HAVING_CN_


CProcessThread*		g_pProcessThread	=	NULL;

CProcessThread::CProcessThread()
{
	SS_TRY_BEGIN;
	m_pKeyWordsThread = NULL;
	m_iGlobalUpdateCount = 2;
	m_pDlls				=	NULL;
	m_pConfig			=	NULL;
	m_pClientManager	=	NULL;
	m_pPipeManager		=	NULL;
	m_pTimerManager		=	NULL;
	m_pLobby			=	NULL;
	m_pRoomManager		=	NULL;
	m_pItemProcessor	=	NULL;
	m_pAudioServerMgr	=	NULL;
	m_pMySQLDatabase	=	NULL;
	m_pTaskSystem		=	NULL;
	m_pSuperActionMgr	=	NULL;
	m_pTitleUtil		=	NULL;
	m_pGPKSvr		=	NULL;
	m_pResourceUsage = NULL;
	m_eServerStatus	=	eServerNotAvailable;
    m_bCanRequireAward = true;

#ifdef _HAVING_FCM_
	m_pFCMSDKBase = NULL;
	InitializeCriticalSection(&m_csFcmLock);
#endif //_HAVING_FCM_

	SS_TRY_END();
}

CProcessThread::~CProcessThread(void)
{
	SS_TRY_BEGIN;

#ifdef _HAVING_FCM_
	DeleteCriticalSection(&m_csFcmLock);
	m_pFCMSDKBase = NULL;
#endif //_HAVING_FCM_
	SS_TRY_END();
}

//线程初始化
bool CProcessThread::InitThread()
{
	SS_TRY_BEGIN_;

	char sPDBFiles[MAX_PATH] = "";
	int nLen = GetPrivateProfileString("PDB", "PDBFiles", "", sPDBFiles, sizeof(sPDBFiles), CONFIG_FILE_NAME);
	if(!nLen || !InitLogFiles(sPDBFiles, LogType_DB))
	{
		MessageBox(NULL, "Initialize log objects failed", "Server initialization", 0);
		return false;
	}
	
	if(!Start())
	{
		return false;
	}

	return true;
	SS_TRY_END_();
	return false;
}

BOOL CProcessThread::OnStart()
{
	SS_TRY_BEGIN;

	m_pDlls	= new CLoadDll();
	if(m_pDlls == NULL)
	{
		MessageBox(NULL, "Create Dll loader object failed", "Server starting", 0);
		return false;
	}

	m_pConfig =	new CSSConfig();
	if(m_pConfig ==	NULL)
	{
		MessageBox(NULL, "Create Config object failed", "Server starting", 0);
		return false;
	}

	m_pMySQLDatabase=new CMySQLDatabase();
	if(m_pMySQLDatabase == NULL)
	{
		MessageBox(NULL, "Create CMySQLDatabase object failed", "Server starting", 0);
		return false;
	}	

	m_pClientManager = new CSSClientManager();	
	if(m_pClientManager	==	NULL)
	{
		MessageBox(NULL, "Create ClientManager object failed", "Server starting", 0);
		return false;
	}
	m_pAudioServerMgr = new CAudioServerMgr;
	if(!m_pAudioServerMgr)
	{
		MessageBox(NULL, "Create AudioServerMgr object failed", "Server starting", 0);
		return false;
	}

	m_pPipeManager = new CSSPipeManager();
	if(m_pPipeManager == NULL)
	{
		MessageBox(NULL, "Create PipeManager object failed", "Server starting", 0);
		return false;
	}

	m_pTimerManager	= new CSSTimerManager();
	if(m_pTimerManager == NULL)
	{
		MessageBox(NULL, "Create TimerManager object failed", "Server starting", 0);
		return false;
	}

	m_pLobby = new CSSLobby();
	if(m_pLobby == NULL)
	{
		MessageBox(NULL, "Create Lobby object failed", "Server starting", 0);
		return false;
	}

	m_pRoomManager = new CSSRoomManager();
	if(m_pRoomManager == NULL)
	{
		MessageBox(NULL, "Create RoomManager object failed", "Server starting", 0);
		return false;
	}

	m_pItemProcessor=new CItemProcessor();
	if(m_pItemProcessor == NULL)
	{
		MessageBox(NULL, "Create ItemProcessor object failed", "Server starting", 0);
		return false;
	}

	m_pTaskSystem=new CTaskSystem();
	if(m_pTaskSystem == NULL)
	{
		MessageBox(NULL, "Create TaskSystem object failed", "Server starting", 0);
		return false;
	}

	m_pSuperActionMgr = new CSuperActionMgr();
	if(!m_pSuperActionMgr)
	{
		MessageBox(NULL, "Create SuperActionMgr object failed", "Server starting", 0);
		return false;
	}

	m_pGmMonitor = new CGmMonitor();
	if(!m_pGmMonitor)
	{
		MessageBox(NULL, "Can't allocate memory for GmMonitor object", "Server starting", 0);
		return false;
	}
	m_pSpaceManager = new CSSSpaceManager();
	if(!m_pSpaceManager)
	{
		MessageBox(NULL, "Can't allocate memory for CSSSpaceManager object", "Server starting", 0);
		return false;
	}
	m_pTitleUtil = new CTitleUtil();
	if(!m_pTitleUtil)
	{
		MessageBox(NULL, "Can't allocate memory for CTitleUtil object", "Server starting", 0);
		return false;
	}

	m_pAutoDispatchMgr = new CAutoDispatchManager;
	if(!m_pAutoDispatchMgr)
	{
		MessageBox(NULL, "Can't allocate memory for CAutoDispatchManager object", "Server starting", 0);
		return false;
	}

	//加载Dll
	if(!m_pDlls->LoadDlls(SDM_NET|SDM_PIPE|SDM_TIMER|SDM_UPLOADREPLAY|SDM_RESUSAGEREPORT))
	{
		return false;
	}
	//加载配置、加载歌曲
	if(!m_pConfig->LoadConfig())
	{
		return false;
	}

	//加载GPK反外挂系统
	if(m_pConfig->GetEnableGPK() == 1)
	{
		m_pGPKSvr = GPKCreateSvrDynCode();
		if(!m_pGPKSvr)
		{
			MessageBox(NULL, "Can't create server dynamic code object", "Server starting", 0);
			return false;
		}
	}
	if(m_pGPKSvr)
	{
		int nBinCount = m_pGPKSvr->LoadBinary("DynCodeBin\\Server" , "DynCodeBin\\Client");
		if(nBinCount == 0 || nBinCount == -1)
		{
			MessageBox(NULL , "Can't initialize server dynamic code object", "Server starting", 0);
			return false;
		}
	}

#ifdef _HAVING_FCM_
	m_pFCMSDKBase = CreateSDKInstance();
	bool bRet = false;
	if(m_pFCMSDKBase)
	{
		bRet = m_pFCMSDKBase->initFCMSDKEASY(&FCMCallBack,&FCMCallBackSwitch,&FCMCallBackQuery);
	}
	if(!bRet)
	{
		//MessageBox(NULL, "Initialize fcm interface failed", "Fcm module", 0);
		//return false;
		ScreenLog("Initialize fcm interface failed, and ignore it");
		//m_iNeedFCM = 0;
	}
#endif //_HAVING_FCM_

	//加载GM权限配置
	if(!m_pGmMonitor->LoadGmConfig())
	{
		MessageBox(NULL, "Load GmConfig info failed!", "Server initialization", 0);
		return false;
	}
	
	if( !InitDbTransManager(m_DBTransationManager,m_pConfig->m_DbConfig))
	{
		return FALSE;
	}

	if( !InitDbTransManager(m_StatisticDbTransManager,m_pConfig->m_StatisticDbConfig))
	{
		return FALSE;
	}

	if(!InitResUsageReportComponent())
	{
		MessageBox(NULL , "Can't initialize resource usage report component!", "Server starting", 0);
		return FALSE;
	}
/* move into CommunityServer
	if( !InitDbTransManager(m_GameEventDbTransManager,m_pConfig->m_GameEventDbConfig))
	{
		return FALSE;
	}
*/

	//初始化组件
	if(!m_pClientManager->InitClientManager() || (!m_pPipeManager->InitPipeManager()) || (!m_pTimerManager->InitTimerManager()))
	{
		return false;
	}

	if(!m_pRoomManager->Init())
	{
		return false;
	}

	m_pKeyWordsThread=new CKeyWordFiltrateThread;
		
	if ( NULL == m_pKeyWordsThread ) return false;
	if(!m_pKeyWordsThread->InitKeyWordsFiltrate(1000))
	{
		SAFE_RELEASE(m_pKeyWordsThread);
		return false;
	}
	const char* pBlackWordNotice = GetConfig()->GetLocalizationText(TextID_BlackNotice, "");
	if( pBlackWordNotice )
	{
		m_pKeyWordsThread->SetTaiWanOneWords(pBlackWordNotice);
	}
	m_pKeyWordsThread->SetWordsSet(GetConfig()->m_setBlackWords);
	CDBTransLoadServFilter::AddTransaction();
	CDBTransLoadWordFilter::AddTransaction();
	

	UpdateAllRoomCrowdedLadders(true);
	UpdateLaddersForDay(true);
	//
#ifdef _ALL_PAY_
	if(!CAllPayCtrl::init())
	{
		MessageBox(NULL, "Load OAConfig info failed!", "Server initialization", 0);
	}
#endif
	//////////////////////////////////////////////////////////////////////////
#ifdef __OTHER_VERSION__
	if(!CRussiaBillingMgr::Init())
		return false;
#endif
	//道具推送
	g_AwardGetItem.Initialize(".\\sotsapi.dll",".\\Config\\ToolsGetConfig.ini",CallBackSotsMsgHandler);
	//
	if(!CServerThread::OnStart())
		return false;

    CDBTansaction *pTrans = new CDBTransCheckAwardTable();
    if(!m_DBTransationManager.AddTransaction(pTrans))
        return false;	
	//
	CStatStonePicturePersonCount::AddTransaction(true);
	//
	pTrans = new COprRoomPictureLoadDb();
	if(!m_DBTransationManager.AddTransaction(pTrans))
		return false;	
	
	Log( "--- OK, Start Server Succeed! ---" );
	ScreenLog( "--- OK, Start Server Succeed! ---" );
	return TRUE;
	SS_TRY_END();
	return FALSE;
}

int CProcessThread::InitDbTransManager(CDBTransationManager& dbTransManager, const AllDbConfig& allDbConfig)
{
	SS_TRY_BEGIN;
	if(!dbTransManager.Init(m_pMySQLDatabase,allDbConfig.m_pDbMap, allDbConfig.m_ThreadNum ))
	{
		ErrorLog(3, "Initialize db manager failed, line=%d", __LINE__);
		MessageBox(NULL, "Initialize db manager failed", "DB initialization", 0);
		return FALSE;
	}

	for( int i =0; i< allDbConfig.m_ThreadNum ; ++i)
	{
		if(  string( allDbConfig.dbConfig[i].szConnectStr) != "" )
		{
			if(!dbTransManager.AddDBThread( allDbConfig.dbConfig[i].m_DbID , allDbConfig.dbConfig[i].szConnectStr, 4800 ))
			{
				ErrorLog(3, "Initialize db thread failed, line=%d, i=%d", __LINE__, i);
				MessageBox(NULL, "Initialize db thread failed", "DB initialization", 0);
				return FALSE;
			}
		}
	}
	return TRUE;
	SS_TRY_END();
	return FALSE;
}

//线程清理
void CProcessThread::FInitThread()
{
	SS_TRY_BEGIN_;
	SafeTerminate(MAX_SERVER_TERMINATE_WAIT_TIME);	
	SS_TRY_END_();
}

void CProcessThread::OnTerminate()
{
	SS_TRY_BEGIN_;	

	m_pSpaceManager->FlushSpaces();

	m_DBTransationManager.Destory();
	// m_StatisticDbTransManager不需要Destroy的
	// m_GameEventDbTransManager.Destory();

	//清理时钟
	if(m_pTimerManager)
		m_pTimerManager->FInitTimerManager();

	//清理客户端管理器
	if(m_pClientManager)
		m_pClientManager->FInitClientManager();

	ReleaseResUsageReportComponent();

	SAFE_DELETE(m_pItemProcessor);
	SAFE_DELETE(m_pRoomManager);
	SAFE_DELETE(m_pAudioServerMgr);
	SAFE_DELETE(m_pLobby);
	SAFE_DELETE(m_pTimerManager);
	SAFE_DELETE(m_pPipeManager);	
	SAFE_DELETE(m_pClientManager);
	SAFE_DELETE(m_pConfig);
	SAFE_DELETE(m_pDlls);
	SAFE_DELETE(m_pMySQLDatabase);
	SAFE_DELETE(m_pTaskSystem);
	SAFE_DELETE(m_pSuperActionMgr);
	SAFE_DELETE(m_pGmMonitor);
	SAFE_DELETE(m_pSpaceManager);
	SAFE_DELETE(m_pTitleUtil);
	SAFE_DELETE(m_pKeyWordsThread);
	SAFE_DELETE(m_pAutoDispatchMgr);

	if(m_pGPKSvr)
		m_pGPKSvr->Release();

	//
	g_AwardGetItem.Terminate();
	m_eServerStatus	=	eServerNotAvailable;

	CServerThread::OnTerminate();
	
	SS_TRY_END_();
}

//获得服务器状态
SERVERSTATUS CProcessThread::GetServerStatus()
{
	SS_TRY_BEGIN;
	if(m_eServerStatus != eServerNotAvailable)
	{
		static wCount = m_pConfig->GetMaxClient() / 4;

		WORD wOnline = m_pClientManager->GetOnlineCount();
		if(wOnline <= wCount)
		{
			m_eServerStatus = eServerIdle;
		}
		else if(wOnline <= 2 * wCount)
		{
			m_eServerStatus = eServerModerate;
		}
		else if(wOnline <= 3 * wCount)
		{
			m_eServerStatus = eServerCrowded;
		}
		else
		{
			m_eServerStatus = eServerFull;
		}
	}
	return m_eServerStatus;
	SS_TRY_END();
	return eServerNotAvailable;
}

BOOL CProcessThread::OnRun()
{
	SS_TRY_BEGIN_;
	int nExeCount = 0;
	if(ExeNetMsg())++nExeCount;
	if(ExeDBMsg())++nExeCount;
	if(ExePipeMsg())++nExeCount;
	if(ExeTimerMsg())++nExeCount;
	if(ExeStatisticDBMsg())++nExeCount;
	if( ExeChatMsg() ) ++nExeCount;

	if(!nExeCount)
	{
		Sleep(SLEEPTIME);
	}
	//if(!ExeNetMsg() && !ExeDBMsg() && !ExePipeMsg() && !ExeTimerMsg() && !ExeStatisticDBMsg())
	//{
	//	Sleep(SLEEPTIME);
	//}

	GetRoomManager()->Update();
	GetClientManager()->Update();
	GetAutoDispatchManager()->Update();

	GetRoomDiamondPicMgr().Update();

	++m_iGlobalUpdateCount;
	uint32 lastValue = m_iGlobalUpdateCount % 40;
	if ( lastValue == 0 )
	{
		GetLobby()->Update();
	}
	else if(lastValue == 13) 
	{
		CheckSystemNotice();
	}
	else if(lastValue == 26)
	{
	#ifdef _HAVING_FCM_
		UpdateFcm();
	#endif //_HAVING_FCM_
	}
	//
#ifdef __OTHER_VERSION__
	CRussiaBillingMgr::Update();
#endif
	//
	if(!CServerThread::OnRun())
		return FALSE;

	SS_TRY_END_();
	return TRUE;
}

//执行Net消息
bool CProcessThread::ExeNetMsg()
{
	SS_TRY_BEGIN_;
	if(GetLoadDll()->GetNetInterface())
	{
		return GetLoadDll()->GetNetInterface()->Run(FETCHNETMSGNUM);
	}
	SS_TRY_END_();
	return false;
}
bool CProcessThread::ExeStatisticDBMsg()
{
	SS_TRY_BEGIN_;
	return m_StatisticDbTransManager.Update(5)>=1;
	SS_TRY_END_();
	return false;
}

//执行DB消息
bool CProcessThread::ExeDBMsg()
{
	SS_TRY_BEGIN_;
	return m_DBTransationManager.Update(5)>=1;
	SS_TRY_END_();
	return false;
}

/* move into CommunityServer
//执行GameEventDB消息
bool CProcessThread::ExeGameEventDBMsg()
{
	SS_TRY_BEGIN_;
	return m_GameEventDbTransManager.Update(5)>=1;
	SS_TRY_END_();
	return false;
}
*/

//执行chat消息
bool CProcessThread::ExeChatMsg()
{
	SS_TRY_BEGIN_;
	int count = 0;
	SaidWords * pTrans = NULL;
	while( (pTrans = m_pKeyWordsThread->PopFinishTransaction()) != NULL )
	{
		if( pTrans->roomID == 0)
		{
			CSSPacketBuilder::NotChat(CHAT_TYPE_LOBBY, pTrans->szInMsg,pTrans->len);
			m_pLobby->Notify();
		}
		else
		{
			CSSRoom* pRoom = m_pRoomManager->FindRoom(pTrans->roomID);
			if( pRoom )
			{
				CSSPacketBuilder::NotChat(CHAT_TYPE_ROOM, pTrans->szInMsg,pTrans->len);
				pRoom->Notify();
				m_pPipeManager->GetBroadcastPipe()->NotifyRoomChat(pRoom, pTrans);
			}
		}
		++count;
		if( count > 10 )
		{
			break;
		}
	}

	return count;
	SS_TRY_END_();
	return false;
}

//只想管道消息
bool CProcessThread::ExePipeMsg()
{
	SS_TRY_BEGIN_;
	if(GetLoadDll()->GetPipeInterface())
	{
		return GetLoadDll()->GetPipeInterface()->Run(5);
	}
	SS_TRY_END_();
	return false;
}

//执行时钟消息
bool CProcessThread::ExeTimerMsg()
{
	SS_TRY_BEGIN_;
	if(GetLoadDll()->GetTimerInterface())
	{
		return GetLoadDll()->GetTimerInterface()->Run(FETCHTIMERMSGNUM);
	}
	SS_TRY_END_();
	return false;
}
//更新排行榜
void CProcessThread::UpdateAllRoomCrowdedLadders(bool IsFirst)
{
	SS_TRY_BEGIN_;
	//更新房间排行榜
	if(m_pRoomManager)
		m_pRoomManager->UpdateRoomCrowdedLadders(IsFirst);

	
	SS_TRY_END_();
}

void CProcessThread::UpdateLaddersForDay(bool IsFirst)
{
	SS_TRY_BEGIN;
	
	//更新歌手排行榜
	{
		/*
		CDBTansaction* pTrans = new CDBTansUpdateSingerLadder(IsFirst);
		if(pTrans == NULL)
		{
			ErrorLog(2, "申请CDBTansUpdateSingerLadder对象内存错误");
		}

		if(!GetDBTransationManager()->AddTransaction(pTrans))
		{
			delete pTrans;
			ErrorLog(3, "CDBTansUpdateSingerLadder DB操作错误");
		}
		*/
		if(IsFirst)
		{
		
			for(int i=MAX_LADDER_TIMES-1;i>=0;i--)
			{
				//CDBTansaction* pTrans = new CDBTansGetSingerLadder(i,bIsNewLadder);
				CDBTansaction* pTrans = new CDBTansGetSingerLadder(i, false);
				if(pTrans == NULL)
				{
					ErrorLog(2, "Create CDBTansGetSingerLadder object failed");
				}
				else if(!g_pProcessThread->GetDBTransationManager()->AddTransaction(pTrans))
				{
					delete pTrans;
					ErrorLog(3, "Add CDBTansGetSingerLadder into manager failed");
				}
			}
		}
		else
		{
			CDBTansaction* pTrans = new CDBTansGetSingerLadder(0, true);
			if(pTrans == NULL)
			{
				ErrorLog(2, "Create CDBTansGetSingerLadder object failed");
			}

			else if(!g_pProcessThread->GetDBTransationManager()->AddTransaction(pTrans))
			{
				delete pTrans;
				ErrorLog(3, "Add CDBTansGetSingerLadder object into manager failed");
			}
		}
	}

/*
	//更新歌曲排行榜
	{
		CDBTansaction* pTrans = new CDBTansUpdateSongLadder();
		if(pTrans == NULL)
		{
			ErrorLog(2, "申请CDBTansUpdateSongLadder对象内存错误");
		}
		m_SongLogs.clear();
		if(!GetDBTransationManager()->AddTransaction(pTrans))
		{
			delete pTrans;
			ErrorLog(3, "CDBTansUpdateSongLadder DB操作错误");
		}

	}
*/
	//if(!IsFirst)
	//	GetConfig()->SetLadderLastUpdateTime(time(NULL));

	SS_TRY_END();
}

void CProcessThread::SetSingerLadder(int Times,SINGER_LADDER_TABLE& Ladder,bool IsNew)
{
	SS_TRY_BEGIN;
	if( IsNew)
	{
		ASSERT(Times == 0);
		for( int i = MAX_LADDER_TIMES-1; i>0; --i )
		{
			m_SingerLadder[i]=m_SingerLadder[i-1];
		}
		m_SingerLadder[0] = Ladder;
	}
	else//第一次运行
	{
		m_SingerLadder[Times]=Ladder;
	}
	if(Times==0 && IsNew)
	{
		for(size_t i=0;i<Ladder.Ladder.size();i++)
		{		
			CSSClient * pClient=GetClientManager()->FindClient(Ladder.Ladder[i].CharID);
			if(pClient)
			{
				ULONG64 FileTime;				
				SystemTimeToFileTime(&Ladder.LadderTime,(FILETIME *)&FileTime);
				FileTime=FileTime/10000000;
				int Time=FileTime/(60*60*24);
				pClient->OnEnterSingerLadder(i,Time);				
			}
			else
			{
				const SINGER_LADDER_EX& subentry = Ladder.Ladder[i];
				//update max singer level ladder
				CDBTansaction* pDBTrans = new CDBTransUpdateMaxSingerLevelLadder(subentry.CharID, i);
				if(pDBTrans == NULL)
				{
					ErrorLog(2, "Create CDBTransUpdateMaxSingerLevelLadder object failed");
				}
				else if(!GetDBTransationManager()->AddTransaction(pDBTrans,subentry.m_iPTID))
				{
					delete pDBTrans;
					ErrorLog(3, "Add CDBTransUpdateMaxSingerLevelLadder object into manager failed");
				}

				//update enter ladder count
				if(i<MAX_SINGER_LADDER_RECORD)
				{
					pDBTrans = new CDBTransUpdateLadderSeqStat(subentry.CharID, LADDER_SINGER_LEVEL, i, 1, false);
					if(pDBTrans == NULL)
					{
						ErrorLog(2, "Create CDBTransUpdateLadderSeqStat object -- singerlevel failed");
					}
					else if(!GetDBTransationManager()->AddTransaction(pDBTrans,subentry.m_iPTID))
					{
						delete pDBTrans;
						ErrorLog(3, "Add CDBTransUpdateLadderSeqStat object -- singerlevel into manager failed");
					}
				}
			}
		}
	}
	SS_TRY_END();
}

const SINGER_LADDER_TABLE* CProcessThread::GetSingerLadder(int Times)
{
	SS_TRY_BEGIN_;
	if(Times>=0&&Times<MAX_LADDER_TIMES)
	{
		return &(m_SingerLadder[Times]);
	}
	SS_TRY_END_();
	return NULL;
}

int CProcessThread::GetClientCount()
{
	SS_TRY_BEGIN_;
	return GetClientManager()->GetOnlineCount();
	SS_TRY_END_();
	return 0;
}

void CProcessThread::GetSongLog(CSSClient * pClient,int SongID)
{
	SS_TRY_BEGIN;
	map<int,SONG_LADDER>::iterator itr;
	itr=m_SongLogs.find(SongID);
	if(itr!=m_SongLogs.end())
	{
		PAGE_INFO PageInfo;
		PageInfo.Type=LADDER_SINGLE_SONG;
		PageInfo.Page=0;
		PageInfo.PageLen=1;
		PageInfo.Param=0;
		SetMsgHeader(GS_S_ACK_LADDER,eMSRetSuccess,*((DWORD *)&PageInfo));
		g_strCliPacket.append((char *)&(itr->second),sizeof(SONG_LADDER));
		SetMsgLength();
		pClient->Send();
	}
	else
	{
		CDBTansaction* pTrans = new CDBTansGetSongLog(pClient->GetCharID(),SongID);
		if(pTrans == NULL)
		{
			ErrorLog(2, "Player[ptid=%s] creates CDBTransGetSongLog object failed", pClient->GetPTID());
		}

		if(!GetDBTransationManager()->AddTransaction(pTrans))
		{
			delete pTrans;
			ErrorLog(3, "Player[ptid=%s] add CDBTransGetSongLog object into manager failed", pClient->GetPTID());
		}
	}
	SS_TRY_END();
}
void CProcessThread::AddSongLog(UINT QueryPlayerID,SONG_LADDER * pSongLog)
{
	SS_TRY_BEGIN_;
	if(pSongLog)
		m_SongLogs[pSongLog->SongID]=*pSongLog;
	CSSClient * pClient=GetClientManager()->FindClient(QueryPlayerID);
	if(pClient)
	{
		PAGE_INFO PageInfo;
		PageInfo.Type=LADDER_SINGLE_SONG;
		PageInfo.Page=0;
		PageInfo.PageLen=0;
		PageInfo.Param=0;
		if(pSongLog)
		{			
			PageInfo.PageLen=1;			
			SetMsgHeader(GS_S_ACK_LADDER,eMSRetSuccess,*((DWORD *)&PageInfo));
			g_strCliPacket.append((char *)pSongLog,sizeof(SONG_LADDER));
			SetMsgLength();
			pClient->Send();
		}
		else
		{			
			SetMsgHeader(GS_S_ACK_LADDER,eMSRetNoData,*((DWORD *)&PageInfo));			
			pClient->Send();
		}
	}
	SS_TRY_END_();
}

void CProcessThread::ExecCommand(LPCTSTR sCommand)
{
	SS_TRY_BEGIN_;
	ScreenLog("Execute: %s",sCommand);
	if(memicmp(sCommand, cmdline_reloadgameconf, SIZEOF_STR(cmdline_reloadgameconf))==0)
	{		
		m_pConfig->LoadGameData();		
	}
	else if(memicmp(sCommand, cmdline_reloadgmlist, SIZEOF_STR(cmdline_reloadgmlist))==0)
	{
		m_pGmMonitor->LoadGmConfig();
	}
	else if(memicmp(sCommand, cmdline_listserver, SIZEOF_STR(cmdline_listserver))==0)
	{
		m_pPipeManager->ListServerLink();
	}
	else if(memicmp(sCommand, cmdline_listnotice, SIZEOF_STR(cmdline_listnotice))==0)
	{
		ScreenLog("Show notices: count=[%d]", m_vSystemNotice.size());
		int n = 1;
		for(vector<GmNotice>::iterator iterNotice=m_vSystemNotice.begin(); iterNotice!=m_vSystemNotice.end(); iterNotice++)
		{
			ScreenLog("%d. StartTime=%s, TimeLen=%d m, Fired=%d, Content=%s"
				, n++, ctime(&iterNotice->nStartTime), iterNotice->nLifeTimeLen, iterNotice->bIsFired, iterNotice->sContent.c_str());
		}
	}
	else if(memicmp(sCommand, cmdline_addnotice, SIZEOF_STR(cmdline_addnotice))==0)
	{
		ScreenLog("Can't Add Notice Here, Please do it in client by gm command!");
		//const char* sContent = sCommand + sizeof(cmdline_addnotice);
		//int nLength = strlen(sContent);
		//if(nLength == 0 || nLength>NOTICE_MAXLENGTH)
		//{
		//	ErrorLog(2, "您添加的通告长度非法，有效长度为大于0且小于%d", NOTICE_MAXLENGTH);
		//	return;
		//}

		//m_sAdminNotice.clear();
		//char* sSysNotice = new char[NOTICE_MAXLENGTH+80];
		//if(sSysNotice)
		//{
		//	_snprintf(sSysNotice, NOTICE_MAXLENGTH+80-1, "<SC:ADM><BODY>%s", sContent);
		//	m_sAdminNotice = sSysNotice;
		//	delete[] sSysNotice;
		//	sSysNotice = NULL;

		//	TraceLog(2, "当前通告已经设置为:%s", m_sAdminNotice.c_str());

		//	m_pClientManager->SendSystemNotice(Notice_System, m_sAdminNotice);
		//}
		//else
		//{
		//	ErrorLog(2, "系统操作失败[不能分配足够内存], 设置通告失败");
		//}
	}
	else if(memicmp(sCommand, cmdline_clearnotice, SIZEOF_STR(cmdline_clearnotice))==0)
	{
		m_vSystemNotice.clear();
		ScreenLog("Notices have been cleared");
	}
	else if(memicmp(sCommand, cmdline_setdebuglog, SIZEOF_STR(cmdline_setdebuglog))==0)
	{
		int nNewLevel = atoi(sCommand + sizeof(cmdline_setdebuglog));
		TraceLog(4, "DebugLog Level Changed From [%d] to [%d]", m_nValidDebugLevel, nNewLevel);
		m_nValidDebugLevel = nNewLevel;
	}
	else if(memicmp(sCommand, cmdline_settracelog, SIZEOF_STR(cmdline_settracelog))==0)
	{
		int nNewLevel = atoi(sCommand + sizeof(cmdline_settracelog));
		TraceLog(4, "TraceLog Level Changed From [%d] to [%d]", m_nValidTraceLevel, nNewLevel);
		m_nValidTraceLevel = nNewLevel;
	}
	else if(memicmp(sCommand, cmdline_setdblog, SIZEOF_STR(cmdline_setdblog))==0)
	{
		int nNewLevel = atoi(sCommand + sizeof(cmdline_setdblog));
		TraceLog(4, "DBLog Level Changed From [%d] to [%d]", m_nValidDBLogLevel, nNewLevel);
		m_nValidDBLogLevel = nNewLevel;
	}
	else if(memicmp(sCommand, cmdline_seterrorlog, SIZEOF_STR(cmdline_seterrorlog))==0)
	{
		int nNewLevel = atoi(sCommand + sizeof(cmdline_seterrorlog));
		TraceLog(4, "ErrorLog Level Changed From [%d] to [%d]", m_nValidErrorLevel, nNewLevel);
		m_nValidErrorLevel = nNewLevel;
	}
	else if(memicmp(sCommand, cmdline_showloglevel, SIZEOF_STR(cmdline_showloglevel))==0)
	{
		ScreenLog("*******Current Valid Log Level*******");
		ScreenLog("***DebugLogLevel=%d", m_nValidDebugLevel);
		ScreenLog("***TraceLogLevel=%d", m_nValidTraceLevel);
		ScreenLog("***DBLogLevel=%d", m_nValidDBLogLevel);
		ScreenLog("***ErrorLogLevel=%d", m_nValidErrorLevel);
		ScreenLog("***EnableScreenLog=%d", m_bEnableScreenLog);
	}
	else if(memicmp(sCommand, cmdline_enablescreenlog, SIZEOF_STR(cmdline_enablescreenlog))==0)
	{
		int nEnable = atoi(sCommand + sizeof(cmdline_enablescreenlog));
		m_bEnableScreenLog = nEnable;
	}
	else if( memicmp( sCommand, "produceladder",strlen("produceladder") ) == 0 ) 
	{
		uint32 starttime = 0;
		uint32 endtime = 0;
		uint32 regulationNo = 0;
		int	isprizeItem = 0;
		char* p = (char*)sCommand+ strlen("produceladder");
		for( ; (*p==' '|| *p == '\t' ) && *p != '\0'; ++p) ;
		
		if( *p != '\0')
		{
			int flag = 0;
			char* q = p;
			for( ; *q!= ' '&& *q!= '\t'&& *q != '\0';++q);
			if( *q != '\0')
			{
				*q = '\0';
				starttime = LocalTimeStringToTime(p);
				p =q +1;
				for( ; (*p==' '|| *p == '\t' ) && *p != '\0'; ++p) ;
				q = p;
				for( ; *q!= ' '&& *q!= '\t'&& *q != '\0';++q); 
				flag = 0;
				if( *q != '\0')
				{
					flag = 1;
					*q = '\0';
				}
				endtime = LocalTimeStringToTime(p);
				if( flag == 1)
				{
					p =q +1;
					for( ; (*p==' '|| *p == '\t' ) && *p != '\0'; ++p) ;
					q = p;
					for( ; *q!= ' '&& *q!= '\t'&& *q != '\0';++q); 
					flag = 0; 
					if( *q != '\0')
					{
						flag = 1;
						*q = '\0';
					}
					regulationNo = atoi( p);

					if( flag == 1)
					{
						p =q +1;
						for( ; (*p==' '|| *p == '\t' ) && *p != '\0'; ++p) ;
						q = p;
						for( ; *q!= ' '&& *q!= '\t'&& *q != '\0';++q); 
						if( *q != '\0')
						{
							*q = '\0';
						}
						isprizeItem = atoi( p);
					}
				}
			}
		}
		if( starttime > 0 && endtime > 0 && regulationNo > 0)
		{
			ProduceIntergLadder::GetStatisticMatchScore(starttime, endtime ,regulationNo,isprizeItem);
		}
	}
	else if(memicmp( sCommand, "deletetotalladder",strlen("deletetotalladder") ) == 0)
	{
		ProduceIntergLadder::DeleteTempTotalMatchLadder();
	}
#ifdef _HAVING_CN_
	else if(memicmp(sCommand, cmdline_enableawardno, SIZEOF_STR(cmdline_enableawardno))== 0)
	{
		int nOldEnableAwardNo = GetConfig()->m_nEnableAwardNoFlag;
		int nNewEnableAwardNo = atoi(sCommand + sizeof(cmdline_enableawardno));
		if(nOldEnableAwardNo!=nNewEnableAwardNo)
		{
			GetConfig()->m_nEnableAwardNoFlag = nNewEnableAwardNo;
			GetClientManager()->NotifyAwardNoFlag(nNewEnableAwardNo);
			TraceLog(4, "EnableAwardNo flag has change to [%d]", nNewEnableAwardNo);
		}
		ScreenLog("EnableAwardNo is %d now", nNewEnableAwardNo);
	}
	else if(memicmp(sCommand, cmdline_showawardnoflag, SIZEOF_STR(cmdline_showawardnoflag))== 0)
	{
		ScreenLog("EnableAwardNo is %d", GetConfig()->m_nEnableAwardNoFlag);
	}
#endif
	else if (memicmp(sCommand, "kickoff", SIZEOF_STR("kickoff"))== 0)
	{//踢玩家下线 kickoff playername  ===>做给GM算了
		//
		char* p = (char*)sCommand+ strlen("kickoff");
		for( ; (*p==' '|| *p == '\t' ) && *p != '\0'; ++p) ;

		if( *p != '\0')
		{//name			
			CSSClient * pTarget=g_pProcessThread->GetClientManager()->FindClientByName(p);
			if (pTarget)
			{
				pTarget->LeaveGame();
				ScreenLog("command[%s] is OK!", sCommand);
			}
			else
				ScreenLog("Invalid command[%s],not found playername in gamesvr", sCommand);
		}
		else
			ScreenLog("Invalid command[%s],not found playername in command", sCommand);
	}
	else
	{
		ScreenLog("Invalid command[%s]", sCommand);
	}
	SS_TRY_END_();
}


BOOL CProcessThread::OnTerminating()
{
	SS_TRY_BEGIN_;	
	
	bool IsIdle=(!ExeNetMsg() && !ExeDBMsg() && !ExePipeMsg() && !ExeTimerMsg());	
	GetRoomManager()->Update();
	GetClientManager()->Update();
	GetRoomManager()->ClearAllRoom();
	return (!IsIdle)||(!GetDBTransationManager()->IsIdle())||(GetClientManager()->GetOnlineCount()>0);
	SS_TRY_END_();
	return FALSE;
}

void CProcessThread::OnBeginTerminate()
{
	SS_TRY_BEGIN_;
	TraceLog(5, "Start to terminate server");
	ScreenLog("Start to terminate server");
	SetServerStatus(eServerNotAvailable);
	GetRoomManager()->SaveAllRoom();
	GetClientManager()->KickAllPlayer();
	SS_TRY_END_();
}

#ifdef _HAVING_FCM_

int CProcessThread::FCMCallBack(const char* szUserID, const char* szRoleID,int iOnlineMinutes,int iOfflineMinutes)
{
	SS_TRY_BEGIN_;

	FcmInfo fcm = {0};
	strncpy_0(fcm.sPTID, szUserID, sizeof(fcm.sPTID));
	strncpy_0(fcm.sCharName, szRoleID, sizeof(fcm.sCharName));
	fcm.nOnlineMinutes = iOnlineMinutes;
	fcm.nOfflineMinutes = iOfflineMinutes;

	EnterCriticalSection(&g_pProcessThread->m_csFcmLock);
	g_pProcessThread->m_listFcmInfo.push_back(fcm);
	LeaveCriticalSection(&g_pProcessThread->m_csFcmLock);
	return 1010;
	SS_TRY_END_();
	return 1000;
}

void CProcessThread::FCMCallBackSwitch(int needFCM)
{
	SS_TRY_BEGIN_;
	//m_iNeedFCM = needFCM;
	SS_TRY_END_();	
}

void CProcessThread::FCMCallBackQuery(const char* szUserID, const char* szRoleID,int iState ,int iOnlineMinutes,int iOfflineMinutes)
{
	SS_TRY_BEGIN_;
	SS_TRY_END_();
}

int CProcessThread::UpdateFcm()
{
	SS_TRY_BEGIN_;
	int nCount = 0;
	if(GetFCM() && TryEnterCriticalSection(&m_csFcmLock))
	{
		bool nErase = true;
		list<FcmInfo>::iterator iterFcm = m_listFcmInfo.begin();
		while( nCount < 10 && iterFcm!=m_listFcmInfo.end())
		{
			nCount++;

			nErase = true;
			CSSClient* pClient=g_pProcessThread->GetClientManager()->FindClientByAccount(iterFcm->sPTID);
			if(pClient && pClient->IsFcmLimited())
			{
				int nGameMinutes = iterFcm->nOnlineMinutes;
				short nFcmID = FCM_TIPID_APPLYFCM;
				short nGameHours = 0;
				if(nGameMinutes>=60*5)
				{
					nFcmID = FCM_TIPID_IMMODERACY;
					pClient->SetFCMFactor(0.0f);
				}
				else if(nGameMinutes>60*3)
				{
					nFcmID = FCM_TIPID_ISTIRING;
					pClient->SetFCMFactor(0.5f);
				}
				else if(nGameMinutes+5>=60)
				{
					nFcmID = FCM_TIPID_PLAYTIME;
					nGameHours = (nGameMinutes +  5)/60;
					if(nGameHours>=3)
					{
						pClient->SetFCMFactor(0.5f);
					}
				}

				nErase = pClient->GetPresentCheckTime()!=0;

				if(nErase && nFcmID!=FCM_TIPID_APPLYFCM)
				{
					CSSPacketBuilder::NotifyFcmMsg(nFcmID, nGameHours);
					pClient->Send();
				}
			}
			if(nErase)iterFcm = m_listFcmInfo.erase(iterFcm);
			else iterFcm++;
		}
		LeaveCriticalSection(&m_csFcmLock);
	}
	return nCount;
	SS_TRY_END_();
	return 0;
}
#endif //_HAVING_FCM_

void CProcessThread::AddSystemNotice(const GmNotice& notice)
{
	SS_TRY_BEGIN_;
	if(notice.nNoticeID>0)
	{
		vector<GmNotice>::iterator iterNotice = std::find(m_vSystemNotice.begin(), m_vSystemNotice.end(), notice.nNoticeID);
		if(iterNotice!=m_vSystemNotice.end())
		{
			iterNotice->nLifeTimeLen = notice.nLifeTimeLen;
			iterNotice->sContent = notice.sContent;
			iterNotice->nRollTimes = notice.nRollTimes;
			iterNotice->nRollInterval = notice.nRollInterval;
			iterNotice->bIsInvalidBo = false;
		}
		else
		{
			m_vSystemNotice.push_back(notice);
		}
	}
	else
	{
		m_vSystemNotice.push_back(notice);
	}
	SS_TRY_END_();
}

bool CProcessThread::RemoveSysNotice(int nIndex)
{
	SS_TRY_BEGIN_;
	if(nIndex>=0 && nIndex<m_vSystemNotice.size())
	{
		m_vSystemNotice.erase(m_vSystemNotice.begin()+nIndex);
		return true;
	}
	SS_TRY_END_();
	return false;
}
void CProcessThread::MarkInvalidBoNotice()
{
	SS_TRY_BEGIN_;
	for(vector<GmNotice>::iterator iterNotice=m_vSystemNotice.begin();
		iterNotice!=m_vSystemNotice.end(); ++iterNotice)
	{
		if(iterNotice->bIsBoNotice)
			iterNotice->bIsInvalidBo = true;
	}
	SS_TRY_END_();
}
void CProcessThread::RemoveAllSysNotice()
{
	SS_TRY_BEGIN_;
	m_vSystemNotice.clear();
	SS_TRY_END_();
}

void CProcessThread::CheckSystemNotice()
{
	SS_TRY_BEGIN_;
	vector<GmNotice>::iterator iterNotice=m_vSystemNotice.begin();
	time_t nCurTime = g_pProcessThread->GetNowTime();
	while(iterNotice!=m_vSystemNotice.end())
	{
		if(iterNotice->bIsInvalidBo)
		{
			if(iterNotice->bIsFired && iterNotice->nNoticeID)
			{
				CSSPacketBuilder::NotRemoveNotice(Notice_System, iterNotice->nNoticeID);
				GetClientManager()->SendSystemMsg();
			}
			iterNotice = m_vSystemNotice.erase(iterNotice);
			continue;
		}
		if(nCurTime>=iterNotice->nStartTime)
		{
			if(!iterNotice->bIsFired)
			{
				iterNotice->bIsFired = true;

				CSSPacketBuilder::NotNotice(Notice_System, iterNotice->nNoticeID, iterNotice->sContent.c_str(), iterNotice->sContent.length()
					, iterNotice->nRollTimes, iterNotice->nRollInterval);
				GetClientManager()->SendSystemMsg();

				if(iterNotice->nLifeTimeLen<=0)
				{
					if(iterNotice->nNoticeID)
					{
						CSSPacketBuilder::NotRemoveNotice(Notice_System, iterNotice->nNoticeID);
						GetClientManager()->SendSystemMsg();
					}
					iterNotice = m_vSystemNotice.erase(iterNotice);
					continue;
				}
			}
			if(iterNotice->nStartTime+iterNotice->nLifeTimeLen<=nCurTime)
			{
				if(iterNotice->nNoticeID)
				{
					CSSPacketBuilder::NotRemoveNotice(Notice_System, iterNotice->nNoticeID);
					GetClientManager()->SendSystemMsg();
				}
				iterNotice = m_vSystemNotice.erase(iterNotice);
				continue;
			}
		}
		iterNotice++;
	}
	SS_TRY_END_();
}

void CProcessThread::CheckSystemNotice(CSSClient* pClient)
{
	SS_TRY_BEGIN_;
	vector<GmNotice>::iterator iterNotice=m_vSystemNotice.begin();
	time_t nCurTime = g_pProcessThread->GetNowTime();
	while(iterNotice!=m_vSystemNotice.end())
	{
		if(nCurTime>=iterNotice->nStartTime)
		{
			if(iterNotice->bIsFired && iterNotice->nStartTime+iterNotice->nLifeTimeLen>=nCurTime)
			{
				CSSPacketBuilder::NotNotice(Notice_System, iterNotice->nNoticeID, iterNotice->sContent.c_str(), iterNotice->sContent.length()
					, iterNotice->nRollTimes, iterNotice->nRollInterval);
				pClient->Send();
			}
		}
		iterNotice++;
	}
	SS_TRY_END_();
}

bool CProcessThread::InitResUsageReportComponent()
{
	pfn_CreateResUsageClass pfnCreatClass = 
		(pfn_CreateResUsageClass)GetProcAddress(m_pDlls->GetResReportModule() , "CreateResUsageClass");
	if(!pfnCreatClass)return false;
	m_pResourceUsage = pfnCreatClass();
	if(!m_pResourceUsage)return false;
	m_pResourceUsage->WarmInit(".\\Log\\");
	int nVersion = m_pResourceUsage->GetClassVersion();
	ASSERT(RESOURCE_USAGE_VERSION == nVersion);
	if(RESOURCE_USAGE_VERSION != nVersion)
		return false;
	return true;
}

void CProcessThread::ReleaseResUsageReportComponent()
{
	if(m_pResourceUsage)
	{
		m_pResourceUsage->Destroy();
		pfn_ReleaseResUsageClass pfnRelease = 
			(pfn_ReleaseResUsageClass)GetProcAddress(m_pDlls->GetResReportModule() , "ReleaseResUsageClass");
		pfnRelease(&m_pResourceUsage);
		m_pResourceUsage = NULL;
	}
}

CLogFile* GetExceptFile()
{
	return g_pProcessThread ? g_pProcessThread->GetExceptFile() : NULL;
}
CLogFile* GetDumpFile()
{
	return g_pProcessThread ? g_pProcessThread->GetDumpFile() : NULL;
}

CLogFile* GetLogFile()
{
	return g_pProcessThread ? g_pProcessThread->GetLogFile() : NULL;
}
CLogFile* GetDBLogFile()
{
	return g_pProcessThread ? g_pProcessThread->GetDBLogFile() : NULL;
}
BOOL IsValidLog(int nLogType, int nLevel)
{
	return g_pProcessThread ? g_pProcessThread->IsValidLog(nLogType, nLevel) : FALSE;
}
BOOL IsLogRoom(UINT nRoomID, int nLevel)
{
	return g_pProcessThread ? g_pProcessThread->IsLogRoom(nRoomID, nLevel) : FALSE;
}
BOOL IsLogClient(UINT nCharaID, int nLevel)
{
	return g_pProcessThread ? g_pProcessThread->IsLogClient(nCharaID, nLevel) : FALSE;
}
BOOL NeedShowScreenLog()
{
	return g_pProcessThread ? g_pProcessThread->NeedShowScreenLog() : FALSE;
}
