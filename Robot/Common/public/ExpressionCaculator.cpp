#include "StdAfx.h"
#include "math.h"
#include "stdlib.h"
#include ".\expressioncaculator.h"
#include "Mmsystem.h"

LPCTSTR KEYWORD_STRINGS[KW_COUNT]=
{"END","IF","THEN","ELSE","ELSEIF","ENDIF","WHILE","DO","ENDWHILE","BREAK","CONTINUE","GOTO",":",";",
	"DECLARE","NUMBER","STRING"};

static BOOL CanMakeIdentifier(char c)
{
	if(c!='+'&&c!='-'&&c!='*'&&c!='/'&&
		c!=','&&c!='('&&c!=')'&&c!=' '&&
		c!='='&&c!='>'&&c!='<'&&c!='~'&&
		c!=0&&c!=':'&&c!=';'&&c!='\r'&&c!='\n'&&c!='\t')
		return true;
	else 
		return false;
}

int IfFN(INT_PTR FnParam,CVariableList* VarList,CBolan* result,CBolan* para1,CBolan* para2,CBolan* para3,CBolan* para4)
{
	SS_TRY_BEGIN_;
	if(para1->value!=0) *result=*para2;
	else *result=*para3;
	SS_TRY_END_();
	return 0;
}

int SinFN(INT_PTR FnParam,CVariableList* VarList,CBolan* result,CBolan* para1,CBolan* para2,CBolan* para3,CBolan* para4)
{
	SS_TRY_BEGIN_;
	result->type=BOLAN_TYPE_NUMBER;
	result->level=0;
	result->value=sin(para1->value);
	SS_TRY_END_();
	return 0;	
}

int CosFN(INT_PTR FnParam,CVariableList* VarList,CBolan* result,CBolan* para1,CBolan* para2,CBolan* para3,CBolan* para4)
{
	SS_TRY_BEGIN_;
	result->type=BOLAN_TYPE_NUMBER;
	result->level=0;
	result->value=cos(para1->value);
	SS_TRY_END_();
	return 0;
}

int RoundFN(INT_PTR FnParam,CVariableList* VarList,CBolan* result,CBolan* para1,CBolan* para2,CBolan* para3,CBolan* para4)
{
	SS_TRY_BEGIN_;
	result->type=BOLAN_TYPE_NUMBER;
	result->level=0;
	if(para1->value>0)
		result->value=floor(para1->value*pow(10,(int)para2->value)+0.5)/pow(10,(int)para2->value);
	else 
		result->value=ceil(para1->value*pow(10,(int)para2->value)-0.5)/pow(10,(int)para2->value);
	SS_TRY_END_();
	return 0;
}

int StrCmpFN(INT_PTR FnParam,CVariableList* VarList,CBolan* result,CBolan* para1,CBolan* para2,CBolan* para3,CBolan* para4)
{
	SS_TRY_BEGIN_;
	result->type=BOLAN_TYPE_NUMBER;
	result->level=0;
	result->value=(DOUBLE)strcmp(para1->StrValue,para2->StrValue);
	SS_TRY_END_();
	return 0;
}

int NumToStrFN(INT_PTR FnParam,CVariableList* VarList,CBolan* result,CBolan* para1,CBolan* para2,CBolan* para3,CBolan* para4)
{
	SS_TRY_BEGIN_;
	result->type=BOLAN_TYPE_STRING;
	result->level=0;
	result->StrValue.Format("%0.2f",para1->value);
	SS_TRY_END_();
	return 0;
}

int RandomFN(INT_PTR FnParam,CVariableList* VarList,CBolan* result,CBolan* para1,CBolan* para2,CBolan* para3,CBolan* para4)
{
	SS_TRY_BEGIN_;
	result->type=BOLAN_TYPE_NUMBER;
	result->level=0;
	result->value=rand()*para1->value/RAND_MAX;
	SS_TRY_END_();
	return 0;
}

CBolanStack::CBolanStack(int InitBuffSize,int GrowSize)
{	
	SS_TRY_BEGIN;
	m_GrowSize=GrowSize;

	m_BolanBuff=new CBolan[InitBuffSize];
	m_BolanBuffSize=InitBuffSize;
	m_BolanCount=0;
	SS_TRY_END();
}

CBolanStack::~CBolanStack()
{
	SS_TRY_BEGIN;
	if(m_BolanBuff)
	{
		delete[] m_BolanBuff;
		m_BolanBuff=0;
		m_BolanBuffSize=0;
		m_BolanCount=0;
	}
	SS_TRY_END();
}

void CBolanStack::Grow()
{
	SS_TRY_BEGIN;
	CBolan * NewBuff=new CBolan[m_BolanBuffSize+m_GrowSize];
	for(int i=0;i<m_BolanBuffSize;i++)
		NewBuff[i]=m_BolanBuff[i];
	//memcpy(NewBuff,m_BolanBuff,sizeof(CBolan)*m_BolanBuffSize);
	delete[] m_BolanBuff;
	m_BolanBuff=NewBuff;
	m_BolanBuffSize+=m_GrowSize;
	SS_TRY_END();
}

bool CBolanStack::Push(CBolan * Bolan)
{
	SS_TRY_BEGIN_;
	if(m_BolanCount>=m_BolanBuffSize)
		Grow();
	m_BolanBuff[m_BolanCount]=*Bolan;
	m_BolanCount++;
	return true;
	SS_TRY_END_();
	return false;
}

CBolan *  CBolanStack::Pop()
{
	SS_TRY_BEGIN_;
	if(m_BolanCount>0)
	{
		m_BolanCount--;
		return m_BolanBuff+(m_BolanCount);
	}
	SS_TRY_END_();
	return NULL;
}

CBolan * CBolanStack::GetAt(int index)
{
	SS_TRY_BEGIN_;
	if(index<m_BolanCount)
	{
		return m_BolanBuff+index;
	}
	SS_TRY_END_();
	return NULL;
}

CBolan * CBolanStack::GetTop()
{
	SS_TRY_BEGIN_;
	if(m_BolanCount>0)
	{
		return m_BolanBuff+(m_BolanCount-1);
	}
	SS_TRY_END_();
	return NULL;
}

int  CBolanStack::PushScript(const char * ExpStr,CVariableList* VariableList,CFactionList * FactionList)
{
	SS_TRY_BEGIN;
	char temp[MAX_STRING_LENGTH+1];
	Identifier TempID;
	char ExpBuff[MAX_EXP_LENGTH];
	CBolanStack TempStack;
	CBolan Bolan;
	int i;
	bool IsInStr;

	i=0;
	IsInStr=false;
	while(*ExpStr)
	{
		if(*ExpStr=='"') IsInStr=!IsInStr;
		
		if(IsInStr)
			ExpBuff[i++]=(*ExpStr++);
		else
			ExpBuff[i++]=toupper(*ExpStr++);		
		
	}
	ExpBuff[i]=0;
	//CharUpper(ExpBuff);
	ExpStr=ExpBuff;
	while(*ExpStr)
	{
		Bolan.Clear();
		if(*ExpStr==' '||*ExpStr=='\r'||*ExpStr=='\n'||*ExpStr=='\t'||*ExpStr=='	' )
		{
			//空格忽略
			ExpStr++;
		}
		else if(*ExpStr=='/'&&*(ExpStr+1)=='/')
		{
			while((*ExpStr!='\r')&&(*ExpStr!='\n')&&(*ExpStr))
			{
				ExpStr++;
			}
		}
		else if(*ExpStr=='"')//字符串
		{
			i=0;
			ExpStr++;
			while(*ExpStr!='"')
			{
				if(i>MAX_STRING_LENGTH)
					return 1001;
				if(*ExpStr==0)
					return 1002;
				temp[i++]=*ExpStr++;
			}
			temp[i]=0;
			ExpStr++;

			Bolan.type=BOLAN_TYPE_STRING;			
			Bolan.StrValue=temp;
			Bolan.level=0;
			Push(&Bolan);
		}
		else if((*ExpStr>='0'&&*ExpStr<='9')||*ExpStr=='.')	//数字
		{
			i=0;
			while((*ExpStr>='0'&&*ExpStr<='9')||*ExpStr=='.')
			{
				temp[i++]=*ExpStr++;
			}
			temp[i]=0;
			Bolan.type=BOLAN_TYPE_NUMBER;
			Bolan.value=atof(temp);
			Bolan.level=0;
			Push(&Bolan);
		}
		else if(*ExpStr==':')		//跳转标识
		{
			i=0;
			ExpStr++;
			while(CanMakeIdentifier(*ExpStr))
			{
				if(i>MAX_IDENTIFIER_LENGTH)
					return 1003;
				TempID[i++]=*ExpStr++;
			}
			TempID[i]=0;			
			Bolan.type=BOLAN_TYPE_IDENTIFIER;
			Bolan.level=0;				
			Bolan.StrValue=TempID;
			Push(&Bolan);			
		}
		else if(*ExpStr==';')//行结束符
		{
			//弹出所有操作符
			while(TempStack.GetTop()!=NULL)
			{				
				Push(TempStack.Pop());
			}
			Bolan.type=BOLAN_TYPE_KEYWORD;
			Bolan.level=0;
			Bolan.index=KW_LINEEND;
			Push(&Bolan);
			//除去多余的行结束符
			while(*ExpStr==' '||*ExpStr==';'||*ExpStr=='\r'||*ExpStr=='\n'||*ExpStr=='\t')
				ExpStr++;
		}
		else if(*ExpStr>='A'&&*ExpStr<='Z')//标识符
		{
			i=0;
			while(CanMakeIdentifier(*ExpStr))
			{
				if(i>MAX_IDENTIFIER_LENGTH)
					return 1003;
				temp[i++]=*ExpStr++;
			}
			temp[i]=0;	
			if(stricmp(temp,"AND")==0)
			{
				while(TempStack.GetTop()!=NULL&&TempStack.GetTop()->level>15)
				{				
					Push(TempStack.Pop());
				}
				Bolan.type=BOLAN_TYPE_OPERATOR;
				Bolan.level=15;
				Bolan.index=OPERATOR_AND;
				TempStack.Push(&Bolan);			
			}
			else if(stricmp(temp,"OR")==0)
			{
				while(TempStack.GetTop()!=NULL&&TempStack.GetTop()->level>15)
				{				
					Push(TempStack.Pop());
				}
				Bolan.type=BOLAN_TYPE_OPERATOR;
				Bolan.level=15;
				Bolan.index=OPERATOR_OR;
				TempStack.Push(&Bolan);			
			}
			else if(stricmp(temp,"NOT")==0)
			{
				while(TempStack.GetTop()!=NULL&&TempStack.GetTop()->level>16)
				{				
					Push(TempStack.Pop());
				}
				Bolan.type=BOLAN_TYPE_OPERATOR;
				Bolan.level=16;
				Bolan.index=OPERATOR_NOT;
				TempStack.Push(&Bolan);			
			}
			else
			{
				int KeyWord=FindKeyWord(temp);
				if(KeyWord>=0)//关键字
				{
					//弹出所有操作符
					while(TempStack.GetTop()!=NULL)
					{				
						Push(TempStack.Pop());
					}

					Bolan.type=BOLAN_TYPE_KEYWORD;
					Bolan.level=0;
					Bolan.index=KeyWord;
					Push(&Bolan);
					
				}
				else
				{
					//函数
					CFaction * pFaction=FactionList->GetObject(temp);
					if(pFaction)
					{		
						while(TempStack.GetTop()!=NULL&&TempStack.GetTop()->level>50)
						{				
							Push(TempStack.Pop());
						}
						Bolan.type=BOLAN_TYPE_FUNCTION;
						Bolan.level=50;
						Bolan.index=pFaction->ID;
						TempStack.Push(&Bolan);					
					}
					else
					{		
						//其他的标识符作为变量					
						Bolan.type=BOLAN_TYPE_VARIABLE;
						Bolan.level=0;							
						Bolan.StrValue=temp;
						CVariable * pVar=VariableList->GetObject(Bolan.StrValue);
						if(pVar)
						{
							Bolan.index=pVar->ID;							
						}
						else
						{
							Bolan.index=0;
							ErrorLog(1, "Variable %s is not defined",(LPCTSTR)Bolan.StrValue);
						}
						Push(&Bolan);					
					}
				}
			}
		}
		else if(*ExpStr=='(')
		{			
			Bolan.type=BOLAN_TYPE_OPERATOR;
			Bolan.level=0;
			Bolan.index=OPERATOR_LP;
			TempStack.Push(&Bolan);
			ExpStr++;			
		}
		else if(*ExpStr==')')
		{
			while(TempStack.GetTop()!=NULL&&(TempStack.GetTop()->type!=BOLAN_TYPE_OPERATOR||TempStack.GetTop()->index!=OPERATOR_LP))
			{				
				Push(TempStack.Pop());
			}			
			TempStack.Pop();
			ExpStr++;
		}
		else if(*ExpStr=='=')
		{	
			if(*(ExpStr+1)=='=')
			{
				while(TempStack.GetTop()!=NULL&&TempStack.GetTop()->level>20)
				{				
					Push(TempStack.Pop());
				}		
				Bolan.type=BOLAN_TYPE_OPERATOR;
				Bolan.level=20;
				Bolan.index=OPERATOR_EQU;	
				TempStack.Push(&Bolan);
				ExpStr+=2;
			}
			else
			{
				while(TempStack.GetTop()!=NULL&&TempStack.GetTop()->level>10)
				{				
					Push(TempStack.Pop());
				}		
				Bolan.type=BOLAN_TYPE_OPERATOR;
				Bolan.level=10;
				Bolan.index=OPERATOR_EVA;	
				TempStack.Push(&Bolan);
				ExpStr++;
			}
		}
		else if(*ExpStr=='>')
		{			
			if(*(ExpStr+1)=='=')
			{
				while(TempStack.GetTop()!=NULL&&TempStack.GetTop()->level>20)
				{				
					Push(TempStack.Pop());
				}	
				Bolan.type=BOLAN_TYPE_OPERATOR;
				Bolan.level=20;
				Bolan.index=OPERATOR_MORE_EQU;	
				TempStack.Push(&Bolan);
				ExpStr+=2;	
			}
			else
			{
				while(TempStack.GetTop()!=NULL&&TempStack.GetTop()->level>20)
				{				
					Push(TempStack.Pop());
				}	
				Bolan.type=BOLAN_TYPE_OPERATOR;
				Bolan.level=20;
				Bolan.index=OPERATOR_MORE;	
				TempStack.Push(&Bolan);
				ExpStr++;				
			}
		}
		else if(*ExpStr=='<')
		{			
			if(*(ExpStr+1)=='=')
			{
				while(TempStack.GetTop()!=NULL&&TempStack.GetTop()->level>20)
				{				
					Push(TempStack.Pop());
				}	
				Bolan.type=BOLAN_TYPE_OPERATOR;
				Bolan.level=20;
				Bolan.index=OPERATOR_LESS_EQU;		
				TempStack.Push(&Bolan);
				ExpStr+=2;	
			}
			else if(*(ExpStr+1)=='>')
			{
				while(TempStack.GetTop()!=NULL&&TempStack.GetTop()->level>20)
				{				
					Push(TempStack.Pop());
				}	
				Bolan.type=BOLAN_TYPE_OPERATOR;
				Bolan.level=20;
				Bolan.index=OPERATOR_DIFF;		
				TempStack.Push(&Bolan);
				ExpStr+=2;	
			}
			else
			{
				while(TempStack.GetTop()!=NULL&&TempStack.GetTop()->level>20)
				{				
					Push(TempStack.Pop());
				}	
				Bolan.type=BOLAN_TYPE_OPERATOR;
				Bolan.level=20;
				Bolan.index=OPERATOR_LESS;			
				TempStack.Push(&Bolan);
				ExpStr++;				
			}			
			
		}
		else if(*ExpStr=='+'||*ExpStr=='-')
		{
			if(*ExpStr=='-'&&(ExpStr==ExpBuff||*(ExpStr-1)=='('))
			{
				Bolan.type=BOLAN_TYPE_OPERATOR;
				Bolan.index=OPERATOR_NEG;
				Bolan.level=60;
				TempStack.Push(&Bolan);
				ExpStr++;
			}
			else
			{
				while(TempStack.GetTop()!=NULL&&TempStack.GetTop()->level>30)
				{				
					Push(TempStack.Pop());
				}		
				Bolan.type=BOLAN_TYPE_OPERATOR;
				Bolan.level=30;
				if(*ExpStr=='+') Bolan.index=OPERATOR_ADD;
				else Bolan.index=OPERATOR_SUB;	
				TempStack.Push(&Bolan);
				ExpStr++;				
			}
		}
		else if(*ExpStr=='*'||*ExpStr=='/')
		{
			while(TempStack.GetTop()!=NULL&&TempStack.GetTop()->level>40)
			{				
				Push(TempStack.Pop());
			}				
			Bolan.type=BOLAN_TYPE_OPERATOR;
			Bolan.level=40;
			if(*ExpStr=='*') Bolan.index=OPERATOR_MUL;
			else Bolan.index=OPERATOR_DIV;	
			TempStack.Push(&Bolan);
			ExpStr++;
		}
		else if(*ExpStr==',')
		{
			while(TempStack.GetTop()!=NULL&&TempStack.GetTop()->level>0)
			{				
				Push(TempStack.Pop());
			}	
			ExpStr++;
		}		
		else
		{
			return 1005;				//非法字符
		}		
	}
	while(TempStack.GetTop()!=NULL)
	{				
		Push(TempStack.Pop());
	}
	SS_TRY_END();
	return 0;
}

int CBolanStack::FindKeyWord(const char * KeyWord)
{
	SS_TRY_BEGIN_;
	for(int i=0;i<KW_COUNT;i++)
	{
		if(stricmp(KeyWord,KEYWORD_STRINGS[i])==0)
			return i;
	}
	SS_TRY_END_();
	return -1;
}
int CBolanStack::FindIdentifier(int start,const char * Name)
{
	SS_TRY_BEGIN_;
	for(int i=start;i<GetSize();i++)
	{
		if(GetAt(i)->type==BOLAN_TYPE_IDENTIFIER)
		{
			if(i>0)
				if(GetAt(i-1)->type==BOLAN_TYPE_KEYWORD&&GetAt(i-1)->index==KW_GOTO)
					continue;
			if(GetAt(i)->StrValue.CompareNoCase(Name)==0)
				return i;
		}
	}
	SS_TRY_END_();
	return -1;
}

CExpressionCaculator::CExpressionCaculator(int MaxVariableCount,int MaxFactionCount)
{	
	SS_TRY_BEGIN;
	m_VarList.Create(MaxVariableCount);
	m_FactionList.Create(MaxFactionCount);

	ZeroMemory(m_InterruptDataBuff,sizeof(m_InterruptDataBuff));
	m_CurInterrupt=IPT_NONE;	
	m_StackUseStatus=0;

	AddFaction("IFF",3,IfFN);
	AddFaction("SIN",1,SinFN);
	AddFaction("COS",1,CosFN);
	AddFaction("ROUND",2,RoundFN);
	AddFaction("STRCMP",2,StrCmpFN);
	AddFaction("NumToStr",2,NumToStrFN);	
	AddFaction("Random",1,RandomFN);	
	
	AddVariable("PI",VARIABLE_TYPE_NUMBER,3.141592653589793238462643383280,NULL);
	
	SS_TRY_END();
}

CExpressionCaculator::~CExpressionCaculator(void)
{
	SS_TRY_BEGIN;
	m_VarList.Clear();
	m_FactionList.Clear();
	SS_TRY_END();
}

int CExpressionCaculator::AddVariable(const char * VarName,int type, double value,const char * StrValue)
{	
	SS_TRY_BEGIN_;
	CVariable * pVar=m_VarList.GetObject(VarName);
	if(pVar==NULL)
	{
		UINT ID=m_VarList.NewObject(&pVar,VarName);
		if(ID)
		{
			pVar->ID=ID;
			strncpy_0(pVar->name,VarName,MAX_IDENTIFIER_LENGTH);
			strupr(pVar->name);
			pVar->type=type;
			pVar->value=value;
			if(StrValue)
				pVar->StrValue=StrValue;
			return 0;
		}
		
		return 2;
	}	
	SS_TRY_END_();
	return 1;
}

bool CExpressionCaculator::EditVariable(const char * VarName, double value,const char * StrValue)
{
	SS_TRY_BEGIN_;
	CVariable * pVar=m_VarList.GetObject(VarName);
	if(pVar)
	{
		
		switch(pVar->type)
		{
		case VARIABLE_TYPE_NUMBER:
			pVar->value=value;
			break;
		case VARIABLE_TYPE_STRING:
			pVar->StrValue=StrValue;
			break;
		}
		return true;
	}
	
	SS_TRY_END_();
	return false;
}

bool CExpressionCaculator::EditVariable(UINT VarIndex, double value,const char * StrValue)
{	
	SS_TRY_BEGIN_;
	CVariable * pVar=m_VarList.GetObject(VarIndex);
	if(pVar)
	{

		switch(pVar->type)
		{
		case VARIABLE_TYPE_NUMBER:
			pVar->value=value;
			break;
		case VARIABLE_TYPE_STRING:
			pVar->StrValue=StrValue;
			break;
		}
		return true;
	}
	SS_TRY_END_();
	return false;
}

bool CExpressionCaculator::DelVariable(const char * VarName)
{
	SS_TRY_BEGIN_;
	return m_VarList.DeleteObject(VarName);	
	SS_TRY_END_();
	return false;
}

int CExpressionCaculator::AddFaction(const char * FactionName, int ParaCount,FactionFN * faction)
{
	SS_TRY_BEGIN_;
	CFaction * pFaction=m_FactionList.GetObject(FactionName);
	if(pFaction==NULL)
	{
		UINT ID=m_FactionList.NewObject(&pFaction,FactionName);
		if(ID)
		{
			pFaction->ID=ID;
			strncpy(pFaction->FnName,FactionName,MAX_IDENTIFIER_LENGTH);
			strupr(pFaction->FnName);
			pFaction->ParaCount=ParaCount;
			pFaction->Fn=faction;
			return 0;
		}

		return 2;
	}	
	SS_TRY_END_();
	return 1;	
}



int CExpressionCaculator::CaculateBolanExpression(CBolanStack& BolanExp,CBolan & result,int& StartPos)
{
	SS_TRY_BEGIN;
	CBolan *pBolan;		
	CVariable * pVar;
	CFaction fn;
	CBolan * t1,* t2;
	int err;
	CBolan temp;
	CBolan *t3,*t4;
	int ValType;
	CBolanStack * stack=NULL;
	int StackUsed;

	if(m_CurInterrupt>0)
	{
		StartPos=m_InterruptDataBuff[m_CurInterrupt].InterruptPos;
		StackUsed=m_InterruptDataBuff[m_CurInterrupt].UseWhichStack;
		m_InterruptDataBuff[m_CurInterrupt].InterruptType=IPT_NONE;
		stack=&(m_Stack[StackUsed]);
		m_CurInterrupt=IPT_NONE;
	}
	else
	{
		for(int i=0;i<MAX_STACK;i++)
		{
			if((m_StackUseStatus&(1L<<i))==0)
			{
				StackUsed=i;
				m_StackUseStatus|=1L<<StackUsed;
				stack=&(m_Stack[StackUsed]);
				break;
			}
		}		
	}
	if(stack==NULL)
		return 3013;

	pBolan=BolanExp.GetAt(StartPos);
	while(pBolan&&
		(pBolan->type==BOLAN_TYPE_NUMBER||pBolan->type==BOLAN_TYPE_OPERATOR||
		pBolan->type==BOLAN_TYPE_FUNCTION||pBolan->type==BOLAN_TYPE_STRING||
		pBolan->type==BOLAN_TYPE_VARIABLE))
	{		
		switch(pBolan->type)
		{
		case BOLAN_TYPE_NUMBER:
			stack->Push(pBolan);
			break;
		case BOLAN_TYPE_VARIABLE:						
			pVar=m_VarList.GetObject(pBolan->index);
			if(pVar==NULL)
				return 4012;
			temp=*pBolan;
			temp.value=pVar->value;			
			temp.StrValue=pVar->StrValue;
			stack->Push(&temp);
			break;
		case BOLAN_TYPE_STRING:
			stack->Push(pBolan);
			break;		
		case BOLAN_TYPE_OPERATOR:
			switch((int)(pBolan->index))
			{
			case OPERATOR_EVA:
				t2=stack->Pop();
				t1=stack->GetTop();				
				ValType=IsTypecompatible(t1,t2);
				if(t1->type!=BOLAN_TYPE_VARIABLE)
					return 3001;
				if(ValType<0)
					return 3004;
				pVar=m_VarList.GetObject(t1->index);
				if(pVar==NULL)
					return 4012;
				switch(ValType)
				{
				case VARIABLE_TYPE_NUMBER:
					t1->value=t2->value;				
					pVar->value=t1->value;
					break;
				case VARIABLE_TYPE_STRING:
					t1->StrValue,t2->StrValue;
					pVar->StrValue,t2->StrValue;				
					break;
				}				
				break;
			case OPERATOR_ADD:
				t2=stack->Pop();
				t1=stack->GetTop();		
				ValType=IsTypecompatible(t1,t2);
				if(ValType<0)
					return 3004;
				switch(ValType)
				{
				case VARIABLE_TYPE_NUMBER:
					t1->value=t1->value+t2->value;		
					t1->type=BOLAN_TYPE_NUMBER;
					break;
				case VARIABLE_TYPE_STRING:
					t1->StrValue+=t2->StrValue;
					t1->type=BOLAN_TYPE_STRING;					
					break;
				}				
				break;
			case OPERATOR_SUB:
				t2=stack->Pop();
				t1=stack->GetTop();				
				ValType=IsTypecompatible(t1,t2);
				if(ValType<0)
					return 3004;
				if(ValType==VARIABLE_TYPE_STRING)
					return 3002;
				t1->value=t1->value-t2->value;	
				t1->type=BOLAN_TYPE_NUMBER;
				break;
			case OPERATOR_MUL:
				t2=stack->Pop();
				t1=stack->GetTop();				
				ValType=IsTypecompatible(t1,t2);
				if(ValType<0)
					return 3004;
				if(ValType==VARIABLE_TYPE_STRING)
					return 3003;
				t1->value=t1->value*t2->value;	
				t1->type=BOLAN_TYPE_NUMBER;
				break;
			case OPERATOR_DIV:
				t2=stack->Pop();
				t1=stack->GetTop();				
				ValType=IsTypecompatible(t1,t2);
				if(ValType<0)
					return 3004;
				if(ValType==VARIABLE_TYPE_STRING)
					return 3005;
				t1->value=t1->value/t2->value;	
				t1->type=BOLAN_TYPE_NUMBER;
				break;
			case OPERATOR_EQU:
				t2=stack->Pop();
				t1=stack->GetTop();				
				ValType=IsTypecompatible(t1,t2);
				if(ValType<0)
					return 3004;
				switch(ValType)
				{
				case VARIABLE_TYPE_NUMBER:
					if(t1->value==t2->value) 
						t1->value=1;
					else 
						t1->value=0;	
					t1->type=BOLAN_TYPE_NUMBER;
					break;
				case VARIABLE_TYPE_STRING:
					if(t1->StrValue.CompareNoCase(t2->StrValue)==0) 
						t1->value=1;
					else 
						t1->value=0;	
					t1->type=BOLAN_TYPE_NUMBER;		
					break;
				}					
				break;			
			case OPERATOR_DIFF:
				t2=stack->Pop();
				t1=stack->GetTop();				
				ValType=IsTypecompatible(t1,t2);
				if(ValType<0)
					return 3004;
				switch(ValType)
				{
				case VARIABLE_TYPE_NUMBER:
					if(t1->value!=t2->value) 
						t1->value=1;
					else 
						t1->value=0;	
					t1->type=BOLAN_TYPE_NUMBER;
					break;
				case VARIABLE_TYPE_STRING:
					if(t1->StrValue.CompareNoCase(t2->StrValue)!=0) 
						t1->value=1;
					else 
						t1->value=0;	
					t1->type=BOLAN_TYPE_NUMBER;		
					break;
				}		
				break;
			case OPERATOR_LESS:
				t2=stack->Pop();
				t1=stack->GetTop();				
				ValType=IsTypecompatible(t1,t2);
				if(ValType<0)
					return 3004;
				switch(ValType)
				{
				case VARIABLE_TYPE_NUMBER:
					if(t1->value<t2->value) 
						t1->value=1;
					else 
						t1->value=0;	
					t1->type=BOLAN_TYPE_NUMBER;
					break;
				case VARIABLE_TYPE_STRING:
					if(t1->StrValue.CompareNoCase(t2->StrValue)<0) 
						t1->value=1;
					else 
						t1->value=0;	
					t1->type=BOLAN_TYPE_NUMBER;		
					break;
				}		
				break;
			case OPERATOR_MORE:
				t2=stack->Pop();
				t1=stack->GetTop();				
				ValType=IsTypecompatible(t1,t2);
				if(ValType<0)
					return 3004;
				switch(ValType)
				{
				case VARIABLE_TYPE_NUMBER:
					if(t1->value>t2->value) 
						t1->value=1;
					else 
						t1->value=0;	
					t1->type=BOLAN_TYPE_NUMBER;
					break;
				case VARIABLE_TYPE_STRING:
					if(t1->StrValue.CompareNoCase(t2->StrValue)>0) 
						t1->value=1;
					else 
						t1->value=0;	
					t1->type=BOLAN_TYPE_NUMBER;		
					break;
				}		
				break;
			case OPERATOR_LESS_EQU:
				t2=stack->Pop();
				t1=stack->GetTop();				
				ValType=IsTypecompatible(t1,t2);
				if(ValType<0)
					return 3004;
				switch(ValType)
				{
				case VARIABLE_TYPE_NUMBER:
					if(t1->value<=t2->value) 
						t1->value=1;
					else 
						t1->value=0;	
					t1->type=BOLAN_TYPE_NUMBER;
					break;
				case VARIABLE_TYPE_STRING:
					if(t1->StrValue.CompareNoCase(t2->StrValue)<=0) 
						t1->value=1;
					else 
						t1->value=0;	
					t1->type=BOLAN_TYPE_NUMBER;		
					break;
				}		
				break;
			case OPERATOR_MORE_EQU:
				t2=stack->Pop();
				t1=stack->GetTop();				
				ValType=IsTypecompatible(t1,t2);
				if(ValType<0)
					return 3004;
				switch(ValType)
				{
				case VARIABLE_TYPE_NUMBER:
					if(t1->value>=t2->value) 
						t1->value=1;
					else 
						t1->value=0;	
					t1->type=BOLAN_TYPE_NUMBER;
					break;
				case VARIABLE_TYPE_STRING:
					if(t1->StrValue.CompareNoCase(t2->StrValue)>=0) 
						t1->value=1;
					else 
						t1->value=0;	
					t1->type=BOLAN_TYPE_NUMBER;		
					break;
				}		
				break;
			case OPERATOR_NEG:
				t1=stack->GetTop();
				ValType=IsTypecompatible(t1,t1);
				if(ValType<0)
					return 3004;
				if(ValType==VARIABLE_TYPE_STRING)
					return 3006;
				t1->value=-t1->value;
				t1->type=BOLAN_TYPE_NUMBER;
				break;
			case OPERATOR_AND:
				t2=stack->Pop();
				t1=stack->GetTop();				
				ValType=IsTypecompatible(t1,t2);
				if(ValType<0)
					return 3004;
				if(ValType==VARIABLE_TYPE_STRING)
					return 3010;
				t1->value=((int)t1->value)&&((int)t2->value);
				t1->type=BOLAN_TYPE_NUMBER;
				break;
			case OPERATOR_OR:
				t2=stack->Pop();
				t1=stack->GetTop();				
				ValType=IsTypecompatible(t1,t2);
				if(ValType<0)
					return 3004;
				if(ValType==VARIABLE_TYPE_STRING)
					return 3011;
				t1->value=((int)t1->value)||((int)t2->value);
				t1->type=BOLAN_TYPE_NUMBER;
				break;
			case OPERATOR_NOT:				
				t1=stack->GetTop();				
				ValType=IsTypecompatible(t1,t1);
				if(ValType<0)
					return 3004;
				if(ValType==VARIABLE_TYPE_STRING)
					return 3012;
				t1->value=!((int)t1->value);
				t1->type=BOLAN_TYPE_NUMBER;
				break;
			}
			break;
		case BOLAN_TYPE_FUNCTION:
			{
				CBolan r;
				CFaction * pFaction=m_FactionList.GetObject(pBolan->index);
				
				
				switch(pFaction->ParaCount)
				{
				case 0:
					err=pFaction->Fn(m_FnParam,&m_VarList,&r,NULL,NULL,NULL,NULL);
					break;
				case 1:
					t1=stack->Pop();
					if(t1==NULL)
						return 3007;
					if(t1->type!=BOLAN_TYPE_NUMBER&&t1->type!=BOLAN_TYPE_STRING&&t1->type!=BOLAN_TYPE_VARIABLE)
						return 3008;
					err=pFaction->Fn(m_FnParam,&m_VarList,&r,t1,NULL,NULL,NULL);
					break;
				case 2:					
					t2=stack->Pop();
					t1=stack->Pop();
					if(t1==NULL||t2==NULL)
						return 3007;
					if((t1->type!=BOLAN_TYPE_NUMBER&&t1->type!=BOLAN_TYPE_STRING&&t1->type!=BOLAN_TYPE_VARIABLE)||
						(t2->type!=BOLAN_TYPE_NUMBER&&t2->type!=BOLAN_TYPE_STRING&&t2->type!=BOLAN_TYPE_VARIABLE))
						return 3008;
					err=pFaction->Fn(m_FnParam,&m_VarList,&r,t1,t2,NULL,NULL);
					break;
				case 3:
					t3=stack->Pop();
					t2=stack->Pop();
					t1=stack->Pop();
					if(t1==NULL||t2==NULL||t3==NULL)
						return 3007;
					if((t1->type!=BOLAN_TYPE_NUMBER&&t1->type!=BOLAN_TYPE_STRING&&t1->type!=BOLAN_TYPE_VARIABLE)||
						(t2->type!=BOLAN_TYPE_NUMBER&&t2->type!=BOLAN_TYPE_STRING&&t2->type!=BOLAN_TYPE_VARIABLE)||
						(t3->type!=BOLAN_TYPE_NUMBER&&t3->type!=BOLAN_TYPE_STRING&&t3->type!=BOLAN_TYPE_VARIABLE))
						return 3008;
					err=pFaction->Fn(m_FnParam,&m_VarList,&r,t1,t2,t3,NULL);
					break;
				case 4:
					t4=stack->Pop();
					t3=stack->Pop();
					t2=stack->Pop();
					t1=stack->Pop();
					if(t1==NULL||t2==NULL||t3==NULL||t4==NULL)
						return 3007;
					if((t1->type!=BOLAN_TYPE_NUMBER&&t1->type!=BOLAN_TYPE_STRING&&t1->type!=BOLAN_TYPE_VARIABLE)||
						(t2->type!=BOLAN_TYPE_NUMBER&&t2->type!=BOLAN_TYPE_STRING&&t2->type!=BOLAN_TYPE_VARIABLE)||
						(t3->type!=BOLAN_TYPE_NUMBER&&t3->type!=BOLAN_TYPE_STRING&&t3->type!=BOLAN_TYPE_VARIABLE)||
						(t4->type!=BOLAN_TYPE_NUMBER&&t4->type!=BOLAN_TYPE_STRING&&t4->type!=BOLAN_TYPE_VARIABLE))
						return 3008;
					err=pFaction->Fn(m_FnParam,&m_VarList,&r,t1,t2,t3,t4);
					break;
				}
				if(err<0) return 3009;					//调用函数出错
				stack->Push(&r);
				if(err>0)
				{
					if(!DealInterrupt(err,StartPos+1,StackUsed))
						return 3014;
					return -1;
				}
			}
			break;		
		}
		StartPos++;
		pBolan=BolanExp.GetAt(StartPos);
	}
	pBolan=stack->Pop();
	if(pBolan)
		result=*pBolan;
	if(stack->GetSize()!=0)
		OutputDebugString("表达式计算对堆栈未能清空！");

	//清空堆栈使用标记
	m_StackUseStatus&=~(1L<<StackUsed);
	SS_TRY_END();
	return 0;
}

int CExpressionCaculator::CaculateExpression(const char * ExpStr,CBolan & result)
{
	CBolanStack Bolans;
	int pos;
	int ReturnCode;

	ReturnCode=Bolans.PushScript(ExpStr,&m_VarList,&m_FactionList);
	if(ReturnCode!=0)
		return ReturnCode;	
	pos=0;
	return CaculateBolanExpression(Bolans,result,pos);
}
int CExpressionCaculator::DealVariableDefine(int start,CBolanStack& ScriptList,bool EndWhileEnd)
{
	SS_TRY_BEGIN_;
	int pos=start;	
	CVariable * pVar;

	while(pos<ScriptList.GetSize())
	{
		switch(ScriptList.GetAt(pos)->type)
		{
			case BOLAN_TYPE_VARIABLE:
				pVar=m_VarList.GetObject(ScriptList.GetAt(pos)->StrValue);
				if(pVar==NULL)
					return 4012;
				ScriptList.GetAt(pos)->index=pVar->ID;
				ScriptList.GetAt(pos)->level=pVar->type;
				pos++;
				break;
			case BOLAN_TYPE_KEYWORD:
				switch((int)(ScriptList.GetAt(pos)->index))
				{
					case KW_NUMBER:
						pos++;
						if(ScriptList.GetAt(pos)->type!=BOLAN_TYPE_VARIABLE)
							return 4010;
						pVar=m_VarList.GetObject(ScriptList.GetAt(pos)->StrValue);
						if(pVar==NULL)
							AddVariable(ScriptList.GetAt(pos)->StrValue,VARIABLE_TYPE_NUMBER,0,NULL);
						pos++;
						break;
					case KW_STRING:			
						pos++;
						if(ScriptList.GetAt(pos)->type!=BOLAN_TYPE_VARIABLE)
							return 4011;
						pVar=m_VarList.GetObject(ScriptList.GetAt(pos)->StrValue);
						if(pVar==NULL)
							AddVariable(ScriptList.GetAt(pos)->StrValue,VARIABLE_TYPE_STRING,0,NULL);						
						pos++;
						break;
					case KW_END:
						if(EndWhileEnd)
							return 0;
					default:
						pos++;
				}
				break;
			default:
				pos++;
		}

	}
	SS_TRY_END_();
	return 0;
}
int CExpressionCaculator::ExecScript(CBolanStack& ScriptList,CBolan& ExpResult,int & pos,bool AutoDeclareVariable)
{
	SS_TRY_BEGIN_;
	int ReturnCode;
	int temp;
	bool IsInIF=false;
	
	if(AutoDeclareVariable)
	{
		ReturnCode=DealVariableDefine(pos,ScriptList,true);
		if(ReturnCode)
			return ReturnCode;
	}
	//处理中断恢复
	if(m_CurInterrupt>0)
	{
		ReturnCode=CaculateBolanExpression(ScriptList,ExpResult,pos);
		if(ReturnCode!=0)
			return ReturnCode;
	}

	while(pos<ScriptList.GetSize())
	{
		switch(ScriptList.GetAt(pos)->type)
		{
		case BOLAN_TYPE_NUMBER:
		case BOLAN_TYPE_OPERATOR:
		case BOLAN_TYPE_FUNCTION:
		case BOLAN_TYPE_STRING:
		case BOLAN_TYPE_VARIABLE:
			ReturnCode=CaculateBolanExpression(ScriptList,ExpResult,pos);
			if(ReturnCode!=0)
				return ReturnCode;
			break;
		case BOLAN_TYPE_KEYWORD:
			switch((int)(ScriptList.GetAt(pos)->index))
			{			
			case KW_END:
				pos++;
				return 0;
			case KW_IF:			
				pos++;
				ReturnCode=CaculateBolanExpression(ScriptList,ExpResult,pos);
				if(ReturnCode!=0)
					return ReturnCode;
				if(ExpResult.value==0)
				{
					temp=FindCoupleIF(ScriptList,KW_THEN,KW_IF,KW_ENDIF,pos,KW_ENDIF);
					if(temp<0)
						return 2003;

					temp=FindCoupleIF(ScriptList,KW_ELSEIF,KW_IF,KW_ENDIF,pos,KW_ENDIF);
					if(temp<0)
						temp=FindCoupleIF(ScriptList,KW_ELSE,KW_IF,KW_ENDIF,pos,KW_ENDIF);
					if(temp<0)
						temp=FindCoupleIF(ScriptList,KW_ENDIF,KW_IF,KW_ENDIF,pos,-1);
					if(temp<0)
						return 2001;
					pos=temp;
					IsInIF=false;
				}
				else
					IsInIF=true;
				break;
			case KW_THEN:
				pos++;
				break;
			case KW_ELSE:
				pos++;
				if(IsInIF)
				{
					temp=FindCoupleIF(ScriptList,KW_ENDIF,KW_IF,KW_ENDIF,pos+1,-1);
					if(temp<0)
						return 2002;
					pos=temp+1;
					IsInIF=false;
				}				
				break;
			case KW_ELSEIF:
				pos++;
				if(IsInIF)
				{					
					temp=FindCoupleIF(ScriptList,KW_ENDIF,KW_IF,KW_ENDIF,pos,-1);
					if(temp<0)
						return 2003;
					pos=temp+1;
					IsInIF=false;
				}
				else
				{					
					ReturnCode=CaculateBolanExpression(ScriptList,ExpResult,pos);
					if(ReturnCode!=0)
						return ReturnCode;
					if(ExpResult.value==0)
					{
						temp=FindCoupleIF(ScriptList,KW_THEN,KW_IF,KW_ENDIF,pos,KW_ENDIF);
						if(temp<0)
							return 2003;

						temp=FindCoupleIF(ScriptList,KW_ELSEIF,KW_IF,KW_ENDIF,pos,KW_ENDIF);
						if(temp<0)
							temp=FindCoupleIF(ScriptList,KW_ELSE,KW_IF,KW_ENDIF,pos,KW_ENDIF);
						if(temp<0)
							temp=FindCoupleIF(ScriptList,KW_ENDIF,KW_IF,KW_ENDIF,pos,-1);
						if(temp<0)
							return 2004;
						pos=temp;
						IsInIF=false;
					}
					else
						IsInIF=true;
				}
				break;
			case KW_ENDIF:
				IsInIF=false;
				pos++;
				break;
			case KW_WHILE:				
				pos++;				
				ReturnCode=CaculateBolanExpression(ScriptList,ExpResult,pos);
				if(ReturnCode!=0)
					return ReturnCode;
				if(ExpResult.value==0)
				{
					temp=FindCoupleKeyWord(ScriptList,KW_ENDWHILE,KW_WHILE,pos);
					if(temp<0)
						return 2010;
					pos=temp+1;					
				}				
				break;
			case KW_DO:
				pos++;
				break;
			case KW_ENDWHILE:
				if(pos==0)
					return 2014;
				temp=FindCoupleKeyWordReverse(ScriptList,KW_WHILE,KW_ENDWHILE,pos-1);
				if(temp<0)
					return 2011;
				pos=temp;
				break;
			case KW_BREAK:
				temp=FindKeyWord(ScriptList,KW_ENDWHILE,pos,-1);
				if(temp<0)
					return 2012;
				pos=temp+1;				
				break;
			case KW_CONTINUE:
				temp=FindKeyWordReverse(ScriptList,KW_WHILE,pos,-1);
				if(temp<0)
					return 2013;
				pos=temp;
				break;
			case KW_GOTO:
				if(ScriptList.GetAt(pos+1)==NULL)
					return 2014;
				if(ScriptList.GetAt(pos+1)->type!=BOLAN_TYPE_IDENTIFIER)
					return 2014;
				temp=ScriptList.FindIdentifier(0,ScriptList.GetAt(pos+1)->StrValue);
				if(temp<0)
					return 2015;
				pos=temp+1;				
				break;
			case KW_IDHEADER:
				pos++;
				break;
			case KW_LINEEND:
				pos++;				
				break;
			case KW_DECLARE:
				pos++;				
				break;
			case KW_NUMBER:
			case KW_STRING:			
				pos+=2;
				break;
			default:
				pos++;
			}
			break;
		case BOLAN_TYPE_IDENTIFIER:
			pos++;
			//if(pos>0)
			//	if(ScriptList.GetAt(pos-1)->type==BOLAN_TYPE_KEYWORD&&ScriptList.GetAt(pos-1)->index==(POSITION)KW_GOTO)

			//	{
			//		temp=FindIdentifier(ScriptList,ScriptList.GetAt(pos)->StrValue);
			//		if(temp>=0)
			//		{
			//			pos=temp+1;
			//		}
			//	}
			break;
		default:
			pos++;
		}
	}
	SS_TRY_END_();
	return 0;
}

int CExpressionCaculator::FindKeyWord(CBolanStack& ScriptList,int KeyWord,int StartPos,int StopKeyWord)
{
	SS_TRY_BEGIN_;
	for(int i=StartPos;i<ScriptList.GetSize();i++)
	{
		if(StopKeyWord>0&&ScriptList.GetAt(i)->type==BOLAN_TYPE_KEYWORD&&ScriptList.GetAt(i)->index==StopKeyWord)
			return -1;
		if(ScriptList.GetAt(i)->type==BOLAN_TYPE_KEYWORD&&ScriptList.GetAt(i)->index==KeyWord)
			return i;
	}
	SS_TRY_END_();
	return -1;
}

int CExpressionCaculator::FindKeyWordReverse(CBolanStack& ScriptList,int KeyWord,int StartPos,int StopKeyWord)
{
	SS_TRY_BEGIN_;
	if(StartPos>=ScriptList.GetSize())
		return -1;
	for(;StartPos>=0;StartPos--)
	{
		if(StopKeyWord>0&&ScriptList.GetAt(StartPos)->type==BOLAN_TYPE_KEYWORD&&ScriptList.GetAt(StartPos)->index==StopKeyWord)
			return -1;
		if(ScriptList.GetAt(StartPos)->type==BOLAN_TYPE_KEYWORD&&ScriptList.GetAt(StartPos)->index==KeyWord)
			return StartPos;
	}
	SS_TRY_END_();
	return -1;
}

//int CExpressionCaculator::FindIdentifier(CBolanStack& ScriptList,char * IDName)
//{
//	for(int i=0;i<ScriptList.GetSize();i++)
//	{
//		if(ScriptList.GetAt(i)->type==BOLAN_TYPE_IDENTIFIER)
//		{
//			if(i==0||ScriptList.GetAt(i-1)->type!=BOLAN_TYPE_KEYWORD||ScriptList.GetAt(i-1)->index!=(POSITION)KW_GOTO)
//				if(strnicmp(ScriptList.GetAt(i)->StrValue,IDName,MAX_IDENTIFIER_LENGTH)==0)
//					return i;
//		}
//				
//	}
//	return -1;
//}

int CExpressionCaculator::FindCoupleKeyWord(CBolanStack& ScriptList,int KeyWord,int CoupleKeyWord,int StartPos)
{
	SS_TRY_BEGIN_;
	int CoupleCount=0;
	for(int i=StartPos;i<ScriptList.GetSize();i++)
	{
		if(ScriptList.GetAt(i)->type==BOLAN_TYPE_KEYWORD&&ScriptList.GetAt(i)->index==CoupleKeyWord)
			CoupleCount++;
		if(ScriptList.GetAt(i)->type==BOLAN_TYPE_KEYWORD&&ScriptList.GetAt(i)->index==KeyWord)
		{
			if(CoupleCount)
				CoupleCount--;
			else
				return i;
		}
	}
	SS_TRY_END_();
	return -1;
}

int CExpressionCaculator::FindCoupleKeyWordReverse(CBolanStack& ScriptList,int KeyWord,int CoupleKeyWord,int StartPos)
{
	SS_TRY_BEGIN_;
	int CoupleCount=0;
	if(StartPos>=ScriptList.GetSize())
		return -1;
	for(;StartPos>=0;StartPos--)
	{
		if(ScriptList.GetAt(StartPos)->type==BOLAN_TYPE_KEYWORD&&ScriptList.GetAt(StartPos)->index==CoupleKeyWord)
			CoupleCount++;
			
		if(ScriptList.GetAt(StartPos)->type==BOLAN_TYPE_KEYWORD&&ScriptList.GetAt(StartPos)->index==KeyWord)
		{
			if(CoupleCount)
				CoupleCount--;
			else
				return StartPos;
		}
	}
	SS_TRY_END_();
	return -1;
}

int CExpressionCaculator::FindCoupleIF(CBolanStack& ScriptList,int KeyWord,int CoupleKeyWord1,int CoupleKeyWord2,int StartPos,int StopKeyWord)
{
	SS_TRY_BEGIN_;
	int CoupleCount=0;
	for(int i=StartPos;i<ScriptList.GetSize();i++)
	{
		if(StopKeyWord>0&&ScriptList.GetAt(i)->type==BOLAN_TYPE_KEYWORD&&ScriptList.GetAt(i)->index==StopKeyWord)
			return -1;
		if(ScriptList.GetAt(i)->type==BOLAN_TYPE_KEYWORD&&ScriptList.GetAt(i)->index==CoupleKeyWord1)
			CoupleCount++;
		if(ScriptList.GetAt(i)->type==BOLAN_TYPE_KEYWORD&&ScriptList.GetAt(i)->index==KeyWord)
		{
			if(CoupleCount==0)				
				return i;
		}
		if(ScriptList.GetAt(i)->type==BOLAN_TYPE_KEYWORD&&ScriptList.GetAt(i)->index==CoupleKeyWord2)
			CoupleCount--;
	}
	SS_TRY_END_();
	return -1;
}

int CExpressionCaculator::IsTypecompatible(CBolan * b1,CBolan * b2)
{
	SS_TRY_BEGIN_;
	int type1,type2;
	switch(b1->type)
	{
	case BOLAN_TYPE_NUMBER:
		type1=VARIABLE_TYPE_NUMBER;
		break;
	case BOLAN_TYPE_STRING:
		type1=VARIABLE_TYPE_STRING;
		break;
	case BOLAN_TYPE_VARIABLE:
		type1=b1->level;
		break;
	default:
		type1=-1;
	}
	switch(b2->type)
	{
	case BOLAN_TYPE_NUMBER:
		type2=VARIABLE_TYPE_NUMBER;
		break;
	case BOLAN_TYPE_STRING:
		type2=VARIABLE_TYPE_STRING;
		break;
	case BOLAN_TYPE_VARIABLE:
		type2=b2->level;
		break;
	default:
		type2=-1;
	}
	if(type1==type2)
		return type1;
	SS_TRY_END_();
	return -1;
}

char * CExpressionCaculator::GetErrorMsg(int ErrCode)
{
	SS_TRY_BEGIN_;
	switch(ErrCode)
	{
	case 1001:
		return "字符串常量过长";
		break;
	case 1002:
		return "缺少字符串结尾符";
		break;
	case 1003:
		return "标识符过长";
		break;
	case 1005:
		return "非法字符";
		break;
	case 2001:
		return "有IF而缺少ENDIF";
		break;
	case 2002:
		return "有ELSE而缺少ENDIF";
		break;
	case 2003:
		return "有IF或ELSEIF而缺少THEN";
		break;
	case 2004:
		return "有ELSEIF而缺少ENDIF";
		break;
	case 2010:
	case 2011:
		return "有ENDWHILE而缺少WHILE";
		break;
	case 2012:
		return "BREAK必须出现在WHILE循环中";
		break;
	case 2013:
		return "CONTINUE必须出现在WHILE循环中";
		break;
	case 2014:
		return "GOTO语句后面缺少标记";
		break;
	case 2015:
		return "GOTO语句后面的标记未定义";
		break;
	case 3001:
		return "赋值对象不是变量";
		break;
	case 3004:
		return "操作符前后的变量或常量类型不一致";
		break;
	case 3002:
		return "字符串不能进行减法操作";
		break;
	case 3003:
		return "字符串不能进行乘法操作";
		break;
	case 3005:
		return "字符串不能进行除法操作";
		break;
	case 3006:
		return "字符串不能进行取负数操作";
		break;
	case 3007:
		return "函数参数不足";
		break;
	case 3008:
		return "非法的函数参数";
		break;
	case 3009:
		return "函数执行错误";
		break;
	case 3010:
		return "AND不能对字符串使用";
	case 3011:
		return "OR不能对字符串使用";
	case 3012:
		return "NOT不能对字符串使用";
	case 3013:
		return "堆栈溢出";
	case 3014:
		return "中断溢出";
	case 4010:
	case 4011:
		return "定义变量错误";		
	case 4012:
		return "变量未定义";		
	case 4013:
		return "未定义标示符";
	case 5001:
		return "中断不存在";
	}
	SS_TRY_END_();
	return "未知错误";
}

bool CExpressionCaculator::DealInterrupt(int type,int pos,int StackUse)
{	
	SS_TRY_BEGIN_;
	if(m_InterruptDataBuff[type].InterruptType!=IPT_NONE)
	{
		int StackUsed=m_InterruptDataBuff[type].UseWhichStack;
		m_Stack[StackUsed].Clear();
		m_StackUseStatus&=~(1L<<StackUsed);
	}
	m_InterruptDataBuff[type].InterruptType=type;
	m_InterruptDataBuff[type].InterruptPos=pos;
	m_InterruptDataBuff[type].UseWhichStack=StackUse;	
	m_InterruptDataBuff[type].InterruptStartTime=timeGetTime();
	return true;
	SS_TRY_END_();
	return false;
}

int CExpressionCaculator::ContinueInterrupt(int type,CBolanStack& ScriptList,const CBolan& InterruptReturn,CBolan& ExpResult,bool AutoDeclareVariable)
{
	SS_TRY_BEGIN_;
	int pos;

	if(type>IPT_NONE&&type<IPT_MAX)
	{
		if(m_InterruptDataBuff[type].InterruptType==type)
		{
			int StackUsed=m_InterruptDataBuff[type].UseWhichStack;
			CBolan * pInterruptResult=m_Stack[StackUsed].GetTop();
			if(pInterruptResult)
				*pInterruptResult=InterruptReturn;
			m_CurInterrupt=type;
			return ExecScript(ScriptList,ExpResult,pos,AutoDeclareVariable);
		}
	
	}
	SS_TRY_END_();
	return 5001;
}

void CExpressionCaculator::Reset()
{
	SS_TRY_BEGIN_;
	ZeroMemory(m_InterruptDataBuff,sizeof(m_InterruptDataBuff));
	m_CurInterrupt=IPT_NONE;	
	m_StackUseStatus=0;
	SS_TRY_END_();
}

CEasyString CExpressionCaculator::BolanToString(CBolan bolan)
{
	CEasyString temp;
	SS_TRY_BEGIN;
	switch(bolan.type)
	{
	case BOLAN_TYPE_NUMBER:
		temp.Format("%0.02f",bolan.value);
		break;
	case BOLAN_TYPE_STRING:
		temp=bolan.StrValue;
		break;
	case BOLAN_TYPE_VARIABLE:
		{
			CVariable* pVar=m_VarList.GetObject(bolan.index);
			if(pVar)
			{
			
				if(pVar->type==VARIABLE_TYPE_NUMBER)
					temp.Format("变量=%0.02f",bolan.value);
				else
					temp.Format("变量=%s",bolan.StrValue);
			}
			else
				temp="未定义变量";
		}
		break;
	default:
		temp="不可解释的符号";
	}
	SS_TRY_END();
	return temp;
}

int CExpressionCaculator::CheckInterruptTimeOut(DWORD TimeOut)
{
	SS_TRY_BEGIN_;
	DWORD time=timeGetTime();
	for(int i=0;i<IPT_MAX;i++)
	{
		if(m_InterruptDataBuff[i].InterruptType!=IPT_NONE&&m_InterruptDataBuff[i].InterruptType!=IPT_SLEEP)
		{
			if(time-m_InterruptDataBuff[i].InterruptStartTime>TimeOut)
				return i;
		}
	}
	SS_TRY_END_();
	return IPT_NONE;
}

