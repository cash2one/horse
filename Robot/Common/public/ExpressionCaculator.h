#pragma once

#define MAX_EXP_LENGTH			40960
#define MAX_IDENTIFIER_LENGTH	32
#define MAX_STRING_LENGTH		4096

#define BOLAN_TYPE_NUMBER			0
#define BOLAN_TYPE_OPERATOR			1
#define BOLAN_TYPE_FUNCTION			2
#define BOLAN_TYPE_STRING			3
#define BOLAN_TYPE_VARIABLE			4
#define BOLAN_TYPE_KEYWORD			5
#define BOLAN_TYPE_IDENTIFIER		6
#define OPERATOR_ADD				0
#define OPERATOR_SUB				1
#define OPERATOR_MUL				2
#define OPERATOR_DIV				3
#define OPERATOR_RP					4
#define OPERATOR_LP					5
#define OPERATOR_NEG				6
#define OPERATOR_COMMA				7
#define OPERATOR_LESS				8
#define OPERATOR_MORE				9
#define OPERATOR_LESS_EQU			10
#define OPERATOR_MORE_EQU			11
#define OPERATOR_EQU				12
#define OPERATOR_DIFF				13
#define OPERATOR_EVA				14
#define OPERATOR_AND				15
#define OPERATOR_OR					16
#define OPERATOR_NOT				17
#define VARIABLE_TYPE_NUMBER		0
#define VARIABLE_TYPE_STRING		1


#define MAX_STACK		16

typedef double DOUBLE;

enum SCRIPT_KEYWORDS
{		
	KW_END=0,
	KW_IF,
	KW_THEN,
	KW_ELSE,
	KW_ELSEIF,
	KW_ENDIF,
	KW_WHILE,
	KW_DO,
	KW_ENDWHILE,
	KW_BREAK,
	KW_CONTINUE,
	KW_GOTO,
	KW_IDHEADER,
	KW_LINEEND,
	KW_DECLARE,
	KW_NUMBER,
	KW_STRING,
	KW_COUNT,
};

typedef char Identifier[MAX_IDENTIFIER_LENGTH+1];


class CIdentifier
{
public:
	Identifier m_Identifier;
public:
	CIdentifier()
	{ZeroMemory(m_Identifier,MAX_IDENTIFIER_LENGTH+1);}
	CIdentifier(CIdentifier& id)
	{strncpy(m_Identifier,id.m_Identifier,MAX_IDENTIFIER_LENGTH);}
	CIdentifier(Identifier id)
	{strncpy(m_Identifier,id,MAX_IDENTIFIER_LENGTH);}
	bool operator==(const CIdentifier& id) const
	{if(strnicmp(m_Identifier,id.m_Identifier,MAX_IDENTIFIER_LENGTH)) return false; else return true;}

};

//typedef std::vector<CIdentifier> CIdentifierList;

class CBolan
{
public:
	int			type;
	double		value;
	CEasyString	StrValue;
	UINT		index;
	int			level;
public:
	CBolan()
	{
		type=0;
		value=0;
		index=0;
		level=0;
	}
	CBolan(const CBolan& Bolan)
	{
		type=Bolan.type;
		value=Bolan.value;
		StrValue=Bolan.StrValue;
		index=Bolan.index;
		level=Bolan.level;
	}
	CBolan& operator=(const CBolan& Bolan)
	{
		type=Bolan.type;
		value=Bolan.value;
		StrValue=Bolan.StrValue;
		index=Bolan.index;
		level=Bolan.level;
		return *this;
	}
	void Clear()
	{
		type=0;
		value=0;
		StrValue.Clear();
		index=0;
		level=0;
	}
};






//typedef std::vector<CBolan> CBolanList;

class CVariable
{
public:
	UINT		ID;
	Identifier	name;		
	DOUBLE		value;
	int			type;
	CEasyString	StrValue;
public:
	CVariable()
	{ID=0;ZeroMemory(name,sizeof(Identifier));value=0;StrValue.Clear();type=0;}
	CVariable(CVariable& var)
	{ID=var.ID;strncpy(name,var.name,MAX_IDENTIFIER_LENGTH);value=var.value;StrValue=var.StrValue;type=var.type;}
	CVariable(const char * VarName,int VarType,DOUBLE VarValue,char *Str)
	{ID=0;strncpy(name,VarName,MAX_IDENTIFIER_LENGTH);value=VarValue;if(Str) StrValue=Str;type=VarType;}
	bool operator==(const CVariable& var) const
	{if(strnicmp(name,var.name,MAX_IDENTIFIER_LENGTH)) return false; else return true;}
};

typedef  CNameStorage<CVariable,false,true> CVariableList;



typedef int FactionFN(INT_PTR FnParam,CVariableList* VarList,CBolan* result,CBolan* para1,CBolan* para2,CBolan* para3,CBolan* para4);

class CFaction
{
public:
	UINT		ID;
	Identifier	FnName;
	int			ParaCount;
	FactionFN	* Fn;
public:
	CFaction()
	{ID=0;ZeroMemory(FnName,sizeof(Identifier));ParaCount=0;Fn=NULL;}
	CFaction(CFaction& fn)
	{ID=fn.ID;strncpy(FnName,fn.FnName,MAX_IDENTIFIER_LENGTH);ParaCount=fn.ParaCount;Fn=fn.Fn;}
	CFaction(const char * name,int para,FactionFN FnPtr)
	{ID=0;strncpy(FnName,name,MAX_IDENTIFIER_LENGTH);ParaCount=para;Fn=FnPtr;}
	bool operator==(const CFaction& fn) const
	{if(strnicmp(FnName,fn.FnName,MAX_IDENTIFIER_LENGTH)) return false; else return true;}
};

typedef  CNameStorage<CFaction,false,true> CFactionList;



class CBolanStack
{
protected:
	CBolan * m_BolanBuff;
	int m_BolanBuffSize;
	int m_BolanCount;
	int m_GrowSize;
public:
	CBolanStack(int InitBuffSize=64,int GrowSize=16);
	~CBolanStack();
	void Grow();
	int GetSize()
	{return m_BolanCount;}
	bool Push(CBolan * Bolan);
	CBolan * Pop();	
	CBolan * GetTop();
	CBolan * GetAt(int index);
	int PushScript(const char * ExpStr,CVariableList* VariableList,CFactionList * FactionList);
	int FindKeyWord(const char * KeyWord);
	void Clear()
	{
		m_BolanCount=0;
	}
	int FindIdentifier(int start,const char * Name);
};

//class CControlKeyWord
//{
//public:
//	int KeyWord;
//	int pos;
//};
//
//typedef CList<CControlKeyWord,CControlKeyWord> CControlKeyWordList;

enum SCRIPT_INTERRUPT_TYPE
{
	IPT_NONE=0,	
	IPT_SLEEP=31,
	IPT_MAX=32,	
};

struct InterruptData
{
	int InterruptType;
	int InterruptPos;	
	DWORD UseWhichStack;
	DWORD InterruptStartTime;
};


class CExpressionCaculator
{
private:
	CBolanStack m_Stack[MAX_STACK];
	DWORD m_StackUseStatus;
	InterruptData m_InterruptDataBuff[IPT_MAX];
	int m_CurInterrupt;
	INT_PTR m_FnParam;	

	
	CVariableList	m_VarList;	
	CFactionList	m_FactionList;	
	
public:
	CExpressionCaculator(int MaxVariableCount,int MaxFactionCount);
	~CExpressionCaculator(void);

	int AddVariable(const char * VarName, int type, double value,const char * StrValue);
	bool EditVariable(const char * VarName, double value,const char * StrValue);
	bool EditVariable(UINT VarIndex, double value,const char * StrValue);
	bool DelVariable(const char * VarName);
	int AddFaction(const char * FactionName, int ParaCount,FactionFN * faction);

	CVariableList * GetVariableList()
	{
		return &m_VarList;
	}

	CFactionList * GetFactionList()
	{
		return &m_FactionList;
	}
	
	int CaculateExpression(const char * ExpStr,CBolan & result);
	int CaculateBolanExpression(CBolanStack& BolanExp,CBolan & result,int& StartPos);
	

	int ExecScript(CBolanStack& ScriptList,CBolan& ExpResult,int & pos,bool AutoDeclareVariable=true);
	int ExecScript(CBolanStack& ScriptList,CBolan& ExpResult,bool AutoDeclareVariable=true)
	{
		int pos=0;
		return ExecScript(ScriptList,ExpResult,pos,AutoDeclareVariable);
	}

	int ExecScript(LPCTSTR ScriptStr,CBolan& ExpResult,bool AutoDeclareVariable=true)
	{
		CBolanStack Stack;
		int pos=0;
		int RetCode=0;

		RetCode=Stack.PushScript(ScriptStr,GetVariableList(),GetFactionList());
		if(RetCode)
			return RetCode;

		return ExecScript(Stack,ExpResult,pos,AutoDeclareVariable);

	}
	
	int IsTypecompatible(CBolan * b1,CBolan * b2);
	char * GetErrorMsg(int ErrCode);
	bool DealInterrupt(int type,int pos,int StackUse);
	int ContinueInterrupt(int type,CBolanStack& ScriptList,const CBolan& InterruptReturn,CBolan& ExpResult,bool AutoDeclareVariable=true);
	void SetFnParam(INT_PTR param)
	{
		m_FnParam=param;
	}
	INT_PTR GetFnParam()
	{
		return m_FnParam;
	}
	void Reset();
	CEasyString BolanToString(CBolan bolan);
	int CheckInterruptTimeOut(DWORD TimeOut);

	int DealVariableDefine(int start,CBolanStack& ScriptList,bool EndWhileEnd);

protected:
	int FindKeyWord(CBolanStack& ScriptList,int KeyWord,int StartPos,int StopKeyWord);
	int FindKeyWordReverse(CBolanStack& ScriptList,int KeyWord,int StartPos,int StopKeyWord);
	int FindCoupleKeyWord(CBolanStack& ScriptList,int KeyWord,int CoupleKeyWord,int StartPos);
	int FindCoupleKeyWordReverse(CBolanStack& ScriptList,int KeyWord,int CoupleKeyWord,int StartPos);
	int FindCoupleIF(CBolanStack& ScriptList,int KeyWord,int CoupleKeyWord1,int CoupleKeyWord2,int StartPos,int StopKeyWord);

	
};
