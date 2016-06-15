/******************************************************************************
* Copyright (c) 2003,上海盛大网络研发中心
* All rights reserved.
* 
* 文件名称：ProcessThread.h
* 摘    要：SS服务器的通讯线程
* 
* 当前版本：1.0
* 作    者：Xu Chengyong
* 建立日期：2005年12月01日
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
#define		SLEEPTIME			1		//空闲睡眠时间
#define		FETCHNETMSGNUM		10		//一次执行的Net消息个数
#define		FETCHDBMSGNUM		5		//一次执行的DB的消息个数
#define		FETCHTIMERMSGNUM	2		//一次执行的Timer消息个数
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

	//线程初始化
	bool InitThread();
	//线程清理
	void FInitThread();
	//设置Server状态
	inline void SetServerStatus(EServerStatus eStatus)
	{
		SS_TRY_BEGIN_;
		m_eServerStatus	=	eStatus;
		SS_TRY_END_();
	}
	//获得服务器状态
	SERVERSTATUS GetServerStatus();

	CResourceUsage* GetResourceUsage()
	{
		SS_TRY_BEGIN_;
		return m_pResourceUsage;
		SS_TRY_END_();
		return NULL;
	}
	
public:
	////////////////////////获得配置的公用函数////////////////////////////
	//取得ServerID
	CSSConfig * GetConfig()
	{
		return m_pConfig;
	}	
	
public:
	////////////////////////获得模块的公用函数////////////////////////////
	//取得自动赠送管理器
	inline CAutoDispatchManager* GetAutoDispatchManager() const
	{
		SS_TRY_BEGIN_;
		return m_pAutoDispatchMgr;
		SS_TRY_END_();
		return NULL;
	}
	//取得客户端控制器
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
	//取得DBManager
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

	//取得CS控制器
	inline CSSPipeManager* GetPipeManager() const
	{
		SS_TRY_BEGIN_;
		return m_pPipeManager;
		SS_TRY_END_();
		return NULL;
	}
	//取得大厅
	inline CSSLobby* GetLobby() const
	{
		SS_TRY_BEGIN_;
		return m_pLobby;
		SS_TRY_END_();
		return NULL;
	}
	//取得RoomManager
	inline CSSRoomManager* GetRoomManager() const
	{
		SS_TRY_BEGIN_;
		return m_pRoomManager;
		SS_TRY_END_();
		return NULL;
	}
	//获取道具处理器
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
	//更新排行榜
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

	//执行Net消息
	bool ExeNetMsg();
	//执行DB消息
	bool ExeDBMsg();
	//取得DB(GameEvnet)消息
	// bool ExeGameEventDBMsg();
	//只想管道消息
	bool ExePipeMsg();
	//执行时钟消息
	bool ExeTimerMsg();
	bool ExeStatisticDBMsg();
	bool ExeChatMsg();

	//执行控制台命令
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
	CLoadDll*					m_pDlls;			//所有的Dll接口
	CSSConfig*					m_pConfig;			//Config接口
	CSSClientManager*			m_pClientManager;	//客户端控制器
	CMySQLDatabase*				m_pMySQLDatabase;
	CDBTransationManager		m_DBTransationManager;	//DB事务管理
	GameDbTransManager			m_StatisticDbTransManager;
	// CDBTransationManager		m_GameEventDbTransManager;	//DB事务管理(GameEvent)
	CSSPipeManager*				m_pPipeManager;		//管道控制器
	CSSTimerManager*			m_pTimerManager;	//时钟管理器
	CSSLobby*					m_pLobby;			//大厅管理器
	CSSRoomManager*				m_pRoomManager;		//房间管理器
	EServerStatus				m_eServerStatus;	//服务器状态
	CItemProcessor *			m_pItemProcessor;	//道具处理器
	CTaskSystem *				m_pTaskSystem;		//任务系统
	CSuperActionMgr*			m_pSuperActionMgr;	//巨星动作
	CGmMonitor*					m_pGmMonitor;		//GM权限检查模块
	CSSSpaceManager*			m_pSpaceManager;	//个人空间管理
	CTitleUtil*					m_pTitleUtil;
	IGPKSvrDynCode*				m_pGPKSvr;
	CResourceUsage*				m_pResourceUsage;
	CAutoDispatchManager*  m_pAutoDispatchMgr;

	CAudioServerMgr*			m_pAudioServerMgr;
	SINGER_LADDER_TABLE			m_SingerLadder[MAX_LADDER_TIMES];		//歌手排行榜
	map<int,SONG_LADDER>		m_SongLogs;

	vector<GmNotice>			m_vSystemNotice;	//系统通知消息
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
