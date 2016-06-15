#pragma once

struct CheckPackInfo
{
	int	 len;
	char ThreadMsg[300];
};

class CWebThread : public CEasyThread
{
	CircleQueue<CheckPackInfo>	m_CheckPackQueue;
	CircleQueue<CheckPackInfo>	m_CheckFinishPackQueue;
		
	HANDLE		m_WebThreadEvent;	

	CEasyCriticalSection	m_Lock;
public:
	CWebThread(void);
	~CWebThread(void);


	//初始化执行一次	
	bool InitWebThread(int QueueSize);

	
  bool AddTransaction();
//  bool AddTransaction(uint64 uTransID,int Flag,char* serverinfo);
 // CheckCharNameInfo * PopFinishTransaction();
 

 protected:
 	virtual BOOL OnRun();
	
 //	void GetNewCHarName(CheckCharNameInfo* pNameInfo);

	//void CreateTcpConnect();
};