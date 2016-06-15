/******************************************************************************
* Copyright (c) 2003,�Ϻ�ʢ�������з�����
* All rights reserved.
* 
* �ļ����ƣ�ProcessThread.h
* ժ    Ҫ��SS��������ͨѶ�߳�
* 
* ��ǰ�汾��1.0
* ��    �ߣ�Xu Chengyong
* �������ڣ�2005��12��01��
**********************************************************************************/
#pragma once


#include ".\gamedbtransmanager.h"
#include ".\roomdiamondpicmgr.h"
#include <list>
struct AllLastWeekMatchLadder
{
	uint32	m_CreateTime;
	
	
	map<uint32,vector<ItemOfLastWeekMatchLadder> >	m_mapLastWeekMatchLadder;

	
};
struct ItemOfLastWeekMatchLadderExt: public ItemOfLastWeekMatchLadder
{
	uint32	m_unCharID;

	ItemOfLastWeekMatchLadderExt()
		:m_unCharID(0)
	{
	}
};
struct AllTotalMatchLadder
{
	map<uint32,ItemOfTotalMatchLadder > m_mapTotalMatchLadder;
};

struct ItemOfTotalMatchLadderExt: public ItemOfTotalMatchLadder
{
	uint32	m_unCharID;
	uint32  m_unLaskWeekMatchAverageScore;
	ItemOfTotalMatchLadderExt()
		:m_unCharID(0)
		,m_unLaskWeekMatchAverageScore(0)
	{
	}
};

struct FcmInfo
{
	char sPTID[MAX_PTID];
	char sCharName[MAX_CHAR_NAME];
	int	 nOnlineMinutes;
	int	 nOfflineMinutes;
};
//
class CAudioServerMgr;
class CKeyWordFiltrateThread;

using namespace serverpipes;
using namespace SGPK;

class CProcessThread : public CServerThread
{
#define		SLEEPTIME			1		//����˯��ʱ��
#define		FETCHNETMSGNUM		10		//һ��ִ�е�Net��Ϣ����
#define		FETCHDBMSGNUM		5		//һ��ִ�е�DB����Ϣ����
#define		FETCHTIMERMSGNUM	2		//һ��ִ�е�Timer��Ϣ����
public:
	CProcessThread();
	virtual ~CProcessThread(void);

	CLoadDll* GetLoadDll()
	{
		SS_TRY_BEGIN_;
		return m_pDlls;
		SS_TRY_END_();
		return NULL;
	}

	//�̳߳�ʼ��
	bool InitThread();
	//�߳�����
	void FInitThread();
	//����Server״̬
	inline void SetServerStatus(EServerStatus eStatus)
	{
		SS_TRY_BEGIN_;
		m_eServerStatus	=	eStatus;
		SS_TRY_END_();
	}
	//��÷�����״̬
	SERVERSTATUS GetServerStatus();

	CResourceUsage* GetResourceUsage()
	{
		SS_TRY_BEGIN_;
		return m_pResourceUsage;
		SS_TRY_END_();
		return NULL;
	}
	
public:
	////////////////////////������õĹ��ú���////////////////////////////
	//ȡ��ServerID
	CSSConfig * GetConfig()
	{
		return m_pConfig;
	}	
	
public:
	////////////////////////���ģ��Ĺ��ú���////////////////////////////
	//ȡ���Զ����͹�����
	inline CAutoDispatchManager* GetAutoDispatchManager() const
	{
		SS_TRY_BEGIN_;
		return m_pAutoDispatchMgr;
		SS_TRY_END_();
		return NULL;
	}
	//ȡ�ÿͻ��˿�����
	inline  CSSClientManager* GetClientManager() const
	{
		SS_TRY_BEGIN_;
		return m_pClientManager;
		SS_TRY_END_();
		return NULL;
	}
	inline CAudioServerMgr* GetAudioServerMgr() const
	{
		SS_TRY_BEGIN_;
		return m_pAudioServerMgr;
		SS_TRY_END_();
		return NULL;
	}
	//ȡ��DBManager
	inline CDBTransationManager* GetDBTransationManager()
	{
		SS_TRY_BEGIN_;
		return &m_DBTransationManager;
		SS_TRY_END_();
		return NULL;
	}

	inline GameDbTransManager* GetStatisticDbTransManager()
	{
		SS_TRY_BEGIN_;
		return &m_StatisticDbTransManager;
		SS_TRY_END_();
		return NULL;
	}

/* move into CommunityServer
	inline CDBTransationManager* GetGameEventDbTransManager()
	{
		SS_TRY_BEGIN_;
		return &m_GameEventDbTransManager;
		SS_TRY_END_();
		return NULL;
	}
*/

	//ȡ��CS������
	inline CSSPipeManager* GetPipeManager() const
	{
		SS_TRY_BEGIN_;
		return m_pPipeManager;
		SS_TRY_END_();
		return NULL;
	}
	//ȡ�ô���
	inline CSSLobby* GetLobby() const
	{
		SS_TRY_BEGIN_;
		return m_pLobby;
		SS_TRY_END_();
		return NULL;
	}
	//ȡ��RoomManager
	inline CSSRoomManager* GetRoomManager() const
	{
		SS_TRY_BEGIN_;
		return m_pRoomManager;
		SS_TRY_END_();
		return NULL;
	}
	//��ȡ���ߴ�����
	inline CItemProcessor * GetItemProcessor() const
	{
		SS_TRY_BEGIN_;
		return m_pItemProcessor;
		SS_TRY_END_();
		return NULL;
	}
	inline CTaskSystem * GetTaskSystem() const
	{
		SS_TRY_BEGIN_;
		return m_pTaskSystem;
		SS_TRY_END_();
		return NULL;
	}

	inline CSuperActionMgr* GetSuperActionMgr() const
	{
		SS_TRY_BEGIN_;
		return m_pSuperActionMgr;
		SS_TRY_END_();
		return NULL;
	}
	CGmMonitor* GetGmMonitor() const
	{
		SS_TRY_BEGIN_;
		return m_pGmMonitor;
		SS_TRY_END_();
		return NULL;
	}
	CSSSpaceManager* GetSpaceManager() const
	{
		SS_TRY_BEGIN_;
		return m_pSpaceManager;
		SS_TRY_END_();
		return NULL;
	}
	CTitleUtil* GetTitleUtil() const
	{
		SS_TRY_BEGIN_;
		return m_pTitleUtil;
		SS_TRY_END_();
		return NULL;
	}
	IGPKSvrDynCode* GetGPKSvr() const
	{
		SS_TRY_BEGIN_;
		return m_pGPKSvr;
		SS_TRY_END_();
		return NULL;
	}

public:
	//�������а�
	void UpdateAllRoomCrowdedLadders(bool IsFirst);
	void UpdateLaddersForDay(bool IsFirst);

	void SetSingerLadder(int Times,SINGER_LADDER_TABLE& Ladder,bool IsNew);
	const SINGER_LADDER_TABLE* GetSingerLadder(int Times);

	void GetSongLog(CSSClient * pClient,int SongID);
	void AddSongLog(UINT QueryPlayerID,SONG_LADDER * pSongLog);

	//inline const string& GetAdminNotice() const
	//{
	//	return m_sAdminNotice;
	//}

	void AddSystemNotice(const GmNotice& notice);
	bool RemoveSysNotice(int nIndex);
	void RemoveAllSysNotice();
	void CheckSystemNotice();
	void CheckSystemNotice(CSSClient* pClient);
	void MarkInvalidBoNotice();
	virtual int GetClientCount();
protected:
	
	virtual BOOL OnRun();

	virtual BOOL OnStart();
	virtual void OnTerminate();

	//ִ��Net��Ϣ
	bool ExeNetMsg();
	//ִ��DB��Ϣ
	bool ExeDBMsg();
	//ȡ��DB(GameEvnet)��Ϣ
	// bool ExeGameEventDBMsg();
	//ֻ��ܵ���Ϣ
	bool ExePipeMsg();
	//ִ��ʱ����Ϣ
	bool ExeTimerMsg();
	bool ExeStatisticDBMsg();
	bool ExeChatMsg();

	//ִ�п���̨����
	virtual void ExecCommand(LPCTSTR Command);

	virtual BOOL OnTerminating();
	virtual void OnBeginTerminate();
	int InitDbTransManager(CDBTransationManager& dbTransManager, const AllDbConfig& allDbConfig);
	bool InitResUsageReportComponent();
	void ReleaseResUsageReportComponent();

#ifdef _HAVING_FCM_
	int UpdateFcm();
protected:
	static int FCMCallBack(const char* szUserID, const char* szRoleID,int iOnlineMinutes,int iOfflineMinutes);
	static void FCMCallBackSwitch(int needFCM); 
	static void FCMCallBackQuery(const char* szUserID, const char* szRoleID,int iState ,int iOnlineMinutes,int iOfflineMinutes);

public:
	CFCMSDKBase *GetFCM()
	{
		return m_pFCMSDKBase;
	}
    bool CanRequireAward()
    {
        return m_bCanRequireAward;
    }
    void SetRequireAwardFlag(bool bFlag)
    {
        SS_TRY_BEGIN_;
        m_bCanRequireAward	=	bFlag;
        SS_TRY_END_();
    }
#endif //_HAVING_FCM_
public:
	CKeyWordFiltrateThread* m_pKeyWordsThread;
protected:
	uint32						m_iGlobalUpdateCount;
#ifdef _HAVING_FCM_
	CRITICAL_SECTION			m_csFcmLock;
	CFCMSDKBase*				m_pFCMSDKBase;
#endif //_HAVING_FCM_
	CLoadDll*					m_pDlls;			//���е�Dll�ӿ�
	CSSConfig*					m_pConfig;			//Config�ӿ�
	CSSClientManager*			m_pClientManager;	//�ͻ��˿�����
	CMySQLDatabase*				m_pMySQLDatabase;
	CDBTransationManager		m_DBTransationManager;	//DB�������
	GameDbTransManager			m_StatisticDbTransManager;
	// CDBTransationManager		m_GameEventDbTransManager;	//DB�������(GameEvent)
	CSSPipeManager*				m_pPipeManager;		//�ܵ�������
	CSSTimerManager*			m_pTimerManager;	//ʱ�ӹ�����
	CSSLobby*					m_pLobby;			//����������
	CSSRoomManager*				m_pRoomManager;		//���������
	EServerStatus				m_eServerStatus;	//������״̬
	CItemProcessor *			m_pItemProcessor;	//���ߴ�����
	CTaskSystem *				m_pTaskSystem;		//����ϵͳ
	CSuperActionMgr*			m_pSuperActionMgr;	//���Ƕ���
	CGmMonitor*					m_pGmMonitor;		//GMȨ�޼��ģ��
	CSSSpaceManager*			m_pSpaceManager;	//���˿ռ����
	CTitleUtil*					m_pTitleUtil;
	IGPKSvrDynCode*				m_pGPKSvr;
	CResourceUsage*				m_pResourceUsage;
	CAutoDispatchManager*  m_pAutoDispatchMgr;

	CAudioServerMgr*			m_pAudioServerMgr;
	SINGER_LADDER_TABLE			m_SingerLadder[MAX_LADDER_TIMES];		//�������а�
	map<int,SONG_LADDER>		m_SongLogs;

	vector<GmNotice>			m_vSystemNotice;	//ϵͳ֪ͨ��Ϣ
	list<FcmInfo>				m_listFcmInfo;

    bool m_bCanRequireAward;	
public:
	//
	map<WORD,UINT>				m_mapLocalSvrStonePicInfo;
	map<WORD,UINT>				m_mapTotalSvrStonePicInfo;
	//
	RoomDiamondPicMgr			m_RoomDiamondPicMgr;
	RoomDiamondPicMgr&			GetRoomDiamondPicMgr()
	{
		return m_RoomDiamondPicMgr;
	}
};
