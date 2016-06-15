#include "StdAfx.h"


//消息头定义
MSGHEADER	g_msgHeader;

//消息包
string		g_strCliPacket;

//字符串变为大写
void StrUP(string& strSrc)
{
	SS_TRY_BEGIN_;
	strSrc = strupr((char*)strSrc.c_str());
	SS_TRY_END_();
}
uint32	LocalTimeStringToTime(const char* pszLocalTime)
{
	struct tm localTime = {0};
	sscanf( pszLocalTime, "%d-%d-%d %d:%d:%d",
		&localTime.tm_year,
		&localTime.tm_mon, 
		&localTime.tm_mday,
		&localTime.tm_hour,
		&localTime.tm_min,
		&localTime.tm_sec
		);
	localTime.tm_year -= 1900;
	localTime.tm_mon -= 1; 

	return mktime( &localTime);
};


bool FetchCharInfoEx(TPlayerData* pPlayerData,IDBConnection * pConnection,IDBRecordSet* pRecordSet)
{
	char pszSQL[1024] = "";
	int nRetCode;	

	int nSqlLen = _snprintf(pszSQL, SIZEOF_STR(pszSQL), 
		"select "
		"DigitalID,point,exchpoint,rank,viptime,pkgcapacity,clubid,title1,title2,difficulty,shopdiscount,btnstatus,"
		"unix_timestamp(createtime),unix_timestamp(lastlogintime)"
		" from charinfo"
		" where DigitalID=%u ",pPlayerData->CharInfo.iDigitalPTID);

	//提取角色信息
	nRetCode = pConnection->ExecuteSQL(pszSQL,nSqlLen);
	if(nRetCode!=DBERR_SUCCEED)
	{		
		DBLog(2, "Execute SQL:%s",pszSQL);
		DBLog(3, "FetchCharInfoEx1: execute db operation failed[%u]:[%s]",
			pConnection->GetLastDatabaseErrorCode(), 
			pConnection->GetLastDatabaseErrorString());
		return false;
	}	
	
	nRetCode=pConnection->GetResults(pRecordSet);
	if(nRetCode!=DBERR_SUCCEED)
	{
		DBLog(2, "Execute SQL:%s",pszSQL);
		DBLog(3, "FetchCharInfoEx2: execute db operation failed[%u]:[%s]",
			pConnection->GetLastDatabaseErrorCode(), 
			pConnection->GetLastDatabaseErrorString());
		return false;
	}
	if(pRecordSet->GetRecordCount()>0)
	{
		CHARACTER_INFO* pCharInfo = &(pPlayerData->CharInfo);		
		//charinfo表字段
		//DigitalID,point,exchpoint,rank,viptime,pkgcapacity,clubid,title1,title2,difficulty,shopdiscount,
		//btnstatus,unix_timestamp(createtime),unix_timestamp(lastlogintime)
		pCharInfo->point = pRecordSet->GetField("point");
		pCharInfo->exchpoint = pRecordSet->GetField("exchpoint");
		pCharInfo->rank = pRecordSet->GetField("rank");
		pCharInfo->viptime = pRecordSet->GetField("viptime");
		pCharInfo->pkgcapacity = pRecordSet->GetField("pkgcapacity");
		pCharInfo->clubid = pRecordSet->GetField("clubid");
		pCharInfo->title1 = pRecordSet->GetField("title1");
		pCharInfo->title2 = pRecordSet->GetField("title2");
		pCharInfo->difficulty = pRecordSet->GetField("difficulty");
		pCharInfo->shopdiscount = pRecordSet->GetField("shopdiscount");
		pCharInfo->btnstatus = pRecordSet->GetField("btnstatus");

		pPlayerData->uLoginTime = pRecordSet->GetField("unix_timestamp(lastlogintime)");
	}
	else
	{
		DBLog(2, "not found player info,Execute SQL:%s",pszSQL);
		return false;
	}
	////////////////////
	//道具信息
	CMySQLRecordSet RecodeSet2;
	nSqlLen = _snprintf(pszSQL, SIZEOF_STR(pszSQL), "select Count,PackageData "
		" from iteminfo"
		" where DigitalID=%u and Type=1",pPlayerData->CharInfo.iDigitalPTID);
	//提取角色信息
	nRetCode = pConnection->ExecuteSQL(pszSQL,nSqlLen);
	if(nRetCode!=DBERR_SUCCEED)
	{		
		DBLog(2, "Execute SQL:%s",pszSQL);
		DBLog(3, "FetchCharInfoEx3: execute db operation failed[%u]:[%s]",
			pConnection->GetLastDatabaseErrorCode(), 
			pConnection->GetLastDatabaseErrorString());
		return false;
	}	

	nRetCode=pConnection->GetResults(&RecodeSet2);
	if(nRetCode!=DBERR_SUCCEED)
	{
		DBLog(2, "Execute SQL:%s",pszSQL);
		DBLog(3, "FetchCharInfoEx4: execute db operation failed[%u]:[%s]",
			pConnection->GetLastDatabaseErrorCode(), 
			pConnection->GetLastDatabaseErrorString());
		return false;
	}
	if(RecodeSet2.GetRecordCount()>0)
	{
		int nCount = (int)RecodeSet2.GetField(0);
		PlayerItemInfo * pVarList=(PlayerItemInfo *)((LPCVOID)RecodeSet2.GetField(1));
		int VarCount=RecodeSet2.GetField(1).GetLength()/sizeof(PlayerItemInfo);
		//ASSERT(nCount == VarCount);
		for(int j=0;j<nCount;j++)
		{
			pPlayerData->ItemList.push_back(pVarList[j]);
		}
		//
		return true;
	}
}

bool PackCharInfo(const TPlayerData* pPlayerData,IDBConnection * pConnection)
{		
	char pszSQL[(MAX_ITEM_COUNT*sizeof(PlayerItemInfo))*2+1+1024] = "";
	int nRetCode;	
	//保存角色信息	
	//////////////////////////////
	char szExtCharaProperty[(MAX_EXT_CHARAPROPERTY_LEN+1)*2+10]={0};
	int Len=pConnection->TranslateString((char *)&(pPlayerData->CharaPropertyExt),sizeof(CHARACTEREXTPRO),szExtCharaProperty,sizeof(szExtCharaProperty),true);
	if( Len<= 0)
	{		
		DBLog(3, "PackCharInfo: szExtCharaProperty escape sql string failed");
		return false;
	}
	szExtCharaProperty[Len]=0;

	//charinfo表字段
	//DigitalID,score,point,exchpoint,rank,viptime,pkgcapacity,clubid,title1,title2,difficulty,shopdiscount,
	//btnstatus,unix_timestamp(createtime),unix_timestamp(lastlogintime)
	_snprintf(pszSQL, SIZEOF_STR(pszSQL),
		"update charinfo"
		" set point=%u,"
		"exchpoint=%u,"
		"rank=%u,"
		"viptime=%u,"
		"pkgcapacity=%u,"
		"clubid=%u,"		
		"title1=%u,"		
		"title2=%u,"
		"difficulty=%u,"
		"shopdiscount=%u,"	
		"lastlogintime=from_unixtime(%u)"
		" where DigitalID=%u",
		pPlayerData->CharInfo.point,
		pPlayerData->CharInfo.exchpoint,
		pPlayerData->CharInfo.rank,
		pPlayerData->CharInfo.viptime,
		pPlayerData->CharInfo.pkgcapacity,
		pPlayerData->CharInfo.clubid,
		pPlayerData->CharInfo.title1,
		pPlayerData->CharInfo.title2,
		pPlayerData->CharInfo.difficulty,
		pPlayerData->CharInfo.shopdiscount,
		pPlayerData->uLoginTime,
		//where
		pPlayerData->CharInfo.iDigitalPTID
		);

	nRetCode = pConnection->ExecuteSQL(pszSQL);
	if(nRetCode != DBERR_SUCCEED)
	{		
		DBLog(1, "Execute SQL:%s",pszSQL);
		DBLog(2, "PackCharInfo: execute db operation failed[%u]:[%s]",
			pConnection->GetLastDatabaseErrorCode(), 
			pConnection->GetLastDatabaseErrorString());		
		return false;
	}
	/////////////////////////////////	
	//执行操作
	int nRealItemCount = pPlayerData->ItemList.size();
	if(nRealItemCount > MAX_ITEM_COUNT)
		nRealItemCount = MAX_ITEM_COUNT;
	//创建道具信息
	char szItemList[(MAX_ITEM_COUNT*sizeof(PlayerItemInfo))*2+1] = {0};
	memset( szItemList, 0, sizeof( szItemList));
	int ItemListSize=nRealItemCount*sizeof(PlayerItemInfo);
	if( ItemListSize*2 +1 > sizeof( szItemList ) )
	{
		DBLog(3, "escape ItemListSize too long");
		return false;
	}
	Len=pConnection->TranslateString((char *)&(pPlayerData->ItemList[0]),ItemListSize,szItemList,sizeof(szItemList),true);
	if( ItemListSize&&Len<= 0)
	{		
		DBLog(3, "PackCharInfo:szItemList escape sql string failed");
		return false;
	}
	szItemList[Len]=0;
	_snprintf(pszSQL, SIZEOF_STR(pszSQL),
		"update iteminfo"
		" set Count=%u,"	
		" PackageData='%s'"			
		" where DigitalID=%u and Type=1",
		//
		nRealItemCount,		
		szItemList,
		//where
		pPlayerData->CharInfo.iDigitalPTID
		);

	nRetCode = pConnection->ExecuteSQL(pszSQL);
	if(nRetCode != DBERR_SUCCEED)
	{		
		DBLog(1, "Execute SQL:%s",pszSQL);
		DBLog(2, "PackCharInfo: execute db operation failed[%u]:[%s]",
			pConnection->GetLastDatabaseErrorCode(), 
			pConnection->GetLastDatabaseErrorString());		
		return false;
	}
	
	return true;
}


extern LPCTSTR GetServerIDString(DWORD ServerID)
{
	static char String[128];
	SD_PIPEID *pstID = (SD_PIPEID *)&ServerID;
	_snprintf(String, SIZEOF_STR(String), "%u-%u-%u-%u",
		pstID->byRegion, 
		pstID->byReserved, 
		pstID->byType, 
		pstID->byIndex);
	return String;
}

inline bool IsBlankChar(char c)
{
	if(c==' ' || c=='\t' || c=='\r' || c=='\n')
		return true;
	return false;
}

//1. 去掉开头和结尾的空字符(空格，\t, \r, \n)
//2. 使中间的空字符都成为空格
//3. 使中间连续出现的空字符只保留一个，如两个或者两个以上连续出现的空格，只保留一个
int TrimNameString(char* sName, int nMaxLen)
{
	char* pChked = sName;
	for(char* pChking = sName; pChking<sName+nMaxLen-1 && *pChking!=0; pChking++)
	{
		if(IsBlankChar(*pChking))
		{
			if(pChked!=sName && *(pChked-1)!=' ')
				*pChked ++ = ' ';
		}
		else
		{
			*pChked ++ = *pChking;
		}
	}
	if(pChked>sName)
	{
		if(*(pChked-1)==' ')
			pChked --;

		*pChked = '\0';
	}
	return int(pChked - sName);
}

//去掉字符串中的空白字符，返回结果字符串长度
int ClearBlankChar(char* sStr, int nStrLen)
{
	char* pChked = sStr;
	for(char* pChking = sStr; pChking<sStr+nStrLen && *pChking!=0; pChking++)
	{
		if(!IsBlankChar(*pChking))
			*pChked ++ = *pChking;
	}
	if(pChked>sStr)
	{
		if(*(pChked-1)==' ')
			pChked --;

		*pChked = '\0';
	}
	return int(pChked - sStr);
}

//去掉字符串头尾的空白字符，中间的空白字符保留，返回结果字符串长度
int TrimString(char* sStr, int nLength)
{
	ASSERT(sStr!=NULL && nLength>0);
	char* pChked = sStr;
	int   a = 0;
	int   b = nLength -1;
	int   f = 0;

	while(a<b)
	{
		f = 0;
		if(IsBlankChar(sStr[a]))
		{
			++a; ++f;
		}
		if(IsBlankChar(sStr[b]))
		{
			--b; ++f;
		}
		if(!f)break;
	}
	if(a>0 && a<b)
	{
		for(int i=0; i<=b-a; ++i)
			sStr[i] = sStr[a+i];
	}
	sStr[b-a+1] = 0;

	return b-a+1;
}

bool SystemTimeToTimet(const SYSTEMTIME& systime, time_t& tim)
{
	SS_TRY_BEGIN_;
	FILETIME localtm = {0};
	if(!SystemTimeToFileTime(&systime, &localtm))
		return false;

	FILETIME filetm = {0};
	LocalFileTimeToFileTime(&localtm, &filetm);
	ULONGLONG ltime = filetm.dwHighDateTime;
	ltime <<= 32;
	ltime |= filetm.dwLowDateTime;

	ltime -= 116444736000000000;
	tim = ltime/10000000;

	return true;
	SS_TRY_END_();
	return false;
}
//time format: yyyymmddhhmmss
bool ConvertTimet(char* sTimeStr, time_t& tim)
{
	SS_TRY_BEGIN;
	char* pCur = sTimeStr;
	if(pCur==NULL)
		return false;

	if(ClearBlankChar(pCur, strlen(pCur))<4)
		return false;

	SYSTEMTIME systime = {0};
	char ctemp = pCur[4];
	pCur[4] = 0;
	systime.wYear = (WORD)atoi(pCur);

	pCur = pCur+4;
	*pCur = ctemp;

	vector<WORD*> vFields;
	vFields.push_back(&systime.wMonth);
	vFields.push_back(&systime.wDay);
	vFields.push_back(&systime.wHour);
	vFields.push_back(&systime.wMinute);
	vFields.push_back(&systime.wSecond);
	int nStrLen = strlen(pCur);
	while(vFields.size() && nStrLen>=2)
	{
		ctemp = pCur[2];
		pCur[2] = 0;
		*(vFields[0]) = (WORD)atoi(pCur);
		vFields.erase(vFields.begin());

		pCur = pCur+2;
		*pCur = ctemp;
		nStrLen -= 2;
	}

	return SystemTimeToTimet(systime, tim);
	SS_TRY_END();
	return false;
}
//convert time_t to FILETIME
void TimetToFileTime( time_t t, LPFILETIME pft )
{
	SS_TRY_BEGIN_;
	LONGLONG ll = Int32x32To64(t, 10000000) + 116444736000000000;
	FILETIME ft = {0};
	ft.dwLowDateTime = (DWORD) ll;
	ft.dwHighDateTime = ll >>32;
	FileTimeToLocalFileTime(&ft, pft);
	SS_TRY_END_();
}
//convert time_t to SYSTEMTIME
void TimetToSystemTime(time_t t, SYSTEMTIME& systime)
{
	SS_TRY_BEGIN_;
	FILETIME ft = {0}; 
	TimetToFileTime(t, &ft);
	FileTimeToSystemTime(&ft, &systime);
	SS_TRY_END_();
}

void TimetToString(time_t t, string& sResultTime)
{
	SYSTEMTIME systime = {0};
	TimetToSystemTime(t, systime);
	char sTimeStr[64] = "";
	_snprintf(sTimeStr, SIZEOF_STR(sTimeStr), "%u-%02u-%02u %02u:%02u:%02u"
		, systime.wYear, systime.wMonth, systime.wDay, systime.wHour, systime.wMinute, systime.wSecond);
	sResultTime = sTimeStr;
}

bool AnsiStrnicmp(const char* pSrcString, const char* pDesString)
{
	const char* p = pSrcString;
	const char* q = pDesString;
	while( *p != '\0'&& *q != '\0' )
	{
		if( *p < 0 )
		{
			if( *p != *q )
			{
				return (*p < *q);
			}
			
			if( *(p+1) != '\0' && *(q+1) != '\0')
			{
				if( *(p+1) != *(q+1) )
				{
					return (*(p+1) < *(q+1) );
				}
				p += 2;
				q += 2;
				
			}
			else
			{
				++p;
				++q;
				
				break;
			}
		}
		else
		{
			if( *p != *q )
			{
				if( *p >= 'A' && *p <= 'Z' && *q >= 'a'&& *q <= 'z')
				{
					if( 32 + *p != *q)
					{
						return 32 + *p < *q;
					}
				}
				else if(*q >= 'A' && *q <= 'Z' && *p >= 'a'&& *p <= 'z')
				{
					if( 32 + *q != *p)
					{
						return *p < 32 + *q;
					}
				}
				else
				{
					return *p < *q;
				}
			}
			++p;
			++q;
			
		}
	}
	
	if( *p == '\0' && *q != '\0')
	{
		return true;
	}
	else
	{
		return false;
	}
};

bool KeyWordFiltrate( char* pszInMsg,int len, const NoCaseWordSet& setBlackWords)
{
	bool ret = false;
	char* pEnd = pszInMsg+len;
	char tempStr[20] ={0};
	for( char* p = pszInMsg; p < pEnd&& p < pszInMsg+80 ;) 
	{
		char* q = p;
		if( *p < 0  )
		{
			q = p + 2;
		}
		else
		{
			q = p + 1;
		}

		for( ; q < pEnd&& q <= p+18 ;)
		{
			memset( tempStr,0,sizeof(tempStr) ); 
			memcpy( tempStr,p,q-p);
			NoCaseWordSet::const_iterator iter = setBlackWords.find( string( tempStr));
			if( iter != setBlackWords.end() )
			{
				for( char* k = p; k< q;++k)
				{
					*k ='?';
				}
				p = q;
				ret = true;
			}

			if( *q < 0  )
			{
				q = q + 2;
			}
			else
			{
				q = q + 1;
			}
		}

		if( q >= pEnd)
		{
			memset( tempStr,0,sizeof(tempStr) );  
			memcpy( tempStr,p,pEnd-p);
			NoCaseWordSet::const_iterator iter = setBlackWords.find( string( tempStr));
			if( iter != setBlackWords.end() )
			{
				for( char* k = p; k< pEnd;++k)
				{
					*k ='?';
				}
				p = q;
				ret = true;
			}
		}

		if( *p < 0  )
		{
			p += 2;
		}
		else
		{
			++p;
		}
	}
	return ret;
}
