#pragma once

struct SaidWords
{
	UINT64	nDigitalID;
	uint32	charId;
	UINT	nClubFlagID;
	char	szCharName[24];
	
	int	 len;
	char szInMsg[300];
};

class CKeyWordFiltrateThread
	:public CEasyThread
{
	CircleQueue<SaidWords>	m_WordsTransQueue;
	CircleQueue<SaidWords>	m_WordsFinishTransQueue;
	
	NoCaseWordSet m_setBlackWords;
	HANDLE		m_KeyWordsWaitEvent;
	char		m_szTaiwanOneWords[50];

	CEasyCriticalSection	m_Lock;

	DECLARE_CLASS_INFO_STATIC(CKeyWordFiltrateThread)
public:
	CKeyWordFiltrateThread(void);
	~CKeyWordFiltrateThread(void);
	
	bool SetWordsSet(const NoCaseWordSet& setBlackWords);

	//初始化执行一次
	bool SetTaiWanOneWords(const char* pszTaiWanOneWords);
	bool InitKeyWordsFiltrate(int QueueSize);
	bool AddTransaction(UINT64 nDigitalID, uint32 charID, UINT nClubFlagID, const char* pszCharName, const char* pszInMsg,int len);
	SaidWords * PopFinishTransaction();
protected:
	virtual BOOL OnRun();

};
