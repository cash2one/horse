#pragma once

#define DB_MAX_HASH		10
#define DEFAULT_TRANS_THREAD	4
#define DEFAULT_PROCESS_LIMIT	16

struct DbConnectThread
{
	uint32 m_DbID;
	char szConnectStr[500];
	CDBTransationWorkThread * m_pDbThread;

	DbConnectThread()
	{
		memset( this, 0, sizeof(*this) );
	}

	~DbConnectThread()
	{
		m_DbID = 0;

		if(m_pDbThread) 
		{
			delete m_pDbThread;
			m_pDbThread = NULL;
		}
		
	}
};

struct DbMap
{
	uint32 hashValue;
	uint32 m_DbID;

	DbMap()
	{
		memset( this, 0, sizeof(*this) );
	}
};

class CDBTransationManager
	:public CNameObject
{
protected:

	DbMap							m_DbMap[ DB_MAX_HASH + 1];
	std::vector<DbConnectThread>	m_WorkThreads;
	IDatabase *								m_pDatabase;
	
	
	DECLARE_CLASS_INFO_STATIC(CDBTransationManager)
public:
	CDBTransationManager(void);
	~CDBTransationManager(void);

	void Destory();

	bool Init(IDatabase * pDatabase,const DbMap* dbMap, uint32 threadNum);
	bool AddDBThread(uint32 dbID, const char * szConnectStr,int QueueSize);
	CDBTransationWorkThread * AllocWorkThread(uint64 DigitalPTID);
	
	bool AddTransaction(CDBTansaction * pDBTansaction, uint64 DigitalPTID =0);

	int Update(int ProcessLimit=DEFAULT_PROCESS_LIMIT);

	bool IsIdle();
};

