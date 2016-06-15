#pragma once
#include <vector>
#include <string>

inline void TrimStr(std::string& str,char TrimChar)
{
	int Start=0,End=-1;
	for(int i=0;i<(int)str.size();i++)
	{
		if(str[i]!=TrimChar)
		{
			Start=i;
			break;
		}
	}

	for(int i=(int)str.size()-1;i>=0;i--)
	{
		if(str[i]!=TrimChar)
		{
			End=i;
			break;
		}
	}

	str=str.substr(Start,End-Start+1);

}

class CSettingFile
{
protected:
	std::vector<std::string> m_SettingStrings;
	int	m_FileChannel;
public:
	CSettingFile(int FileChannel=0)
	{
		m_FileChannel=FileChannel;
	}
	bool LoadFromFile(LPCTSTR FileName)
	{
		FILE * pFile;

		pFile=fopen(FileName,"rb");
		if(pFile==NULL)
			return false;
		fseek(pFile,0,SEEK_END);
		int FileSize=ftell(pFile);	
		char * Str=new char[FileSize+1];
		fseek(pFile,0,SEEK_SET);
		fread(Str,1,FileSize,pFile);
		fclose(pFile);		
		Str[FileSize]=0;
		bool ret=Load(Str);
		delete[] Str;
		return ret;		
	}

	bool Load(LPCTSTR SettingString,char LineDelimiter='\r',char CommentHead=';')
	{
		char * Source;
		char * Line;
		int Len=(int)strlen(SettingString)+1;
		char * pBuff=new char[Len];
		Source=pBuff;
		strcpy(Source,SettingString);
		m_SettingStrings.clear();
		while(Source)
		{
			char * Find=(char *)FindNextLine(Source,LineDelimiter);
			if(Find)
			{
				Find[0]=0;
				Find++;
				Line=Source;
				Source=Find;
			}
			else
			{
				Line=Source;
				Source=NULL;
			}
			Find=(char *)FindNextLine(Line,CommentHead);
			if(Find)
				Find[0]=0;
			std::string temp=Line;
			TrimStr(temp,' ');
			TrimStr(temp,LineDelimiter);
			TrimStr(temp,'\r');
			TrimStr(temp,'\n');			
			if(!temp.empty())
				m_SettingStrings.push_back(temp);
		}
		delete[] pBuff;
		return true;
	}

	const char * GetString( const char * pszSection, const char * pszItemName, const char * pszDefValue = NULL )
	{
		LPCTSTR p = GetSettingString( pszSection, pszItemName );
		if( p == NULL || strlen(p) == 0)return pszDefValue;
		return p;
	}
	int	GetInteger( const char * pszSection, const char * pszItemName, int DefValue = 0 )
	{
		LPCTSTR p = GetSettingString( pszSection, pszItemName );
		if( p == NULL || strlen(p) == 0 )return DefValue;
		return atoi(p);
	}
	double GetDouble(const char * pszSection, const char * pszItemName, double DefValue = 0 )
	{
		LPCTSTR p = GetSettingString( pszSection, pszItemName );
		if( p == NULL || strlen(p) == 0) return DefValue;
		return atof(p);
	}
protected:
	int	FindSectionLine( const char * pszSection )
	{
		if( pszSection == NULL )
			return 0;
		int linecount = (int)m_SettingStrings.size();
		int sectionlength = (int)strlen( pszSection );
		for( int i = 0;i < linecount;i ++ )
		{
			LPCTSTR p = m_SettingStrings[i].c_str();

			if( *p == '[' && *(p+sectionlength+1)==']' )
			{
				if( strnicmp( p+1, pszSection, sectionlength ) == 0 )
					return (i+1);
			}
		}
		return -1;
	}
	LPCTSTR GetSettingString(  const char * pszSection, const char * pszItemName )
	{
		int startindex = 0;
		if( pszSection != NULL )
		{
			startindex = FindSectionLine( pszSection );
			if( startindex == -1 )return NULL;
		}

		int itemnamelength = (int)strlen( pszItemName );
		if( itemnamelength == 0 )return NULL;

		int linecount = (int)m_SettingStrings.size();
		for( int i = startindex;i < linecount;i ++ )
		{
			LPCTSTR p = m_SettingStrings[i].c_str();
			const char* startP = p;
			if( *p == '[' )return NULL;
			p += itemnamelength;
			//	如果到达下一个section， 返回错误
			if( *p == '[' )return NULL;
			//	如果是ItemName=这样的句式，进入进一步搜索
			for( ; *p == ' '||*p == '\t';++p);
			if( *(p) == '=' )
			{
				for( ++p; *p == ' '||*p == '\t';++p);
				if( strnicmp( startP, pszItemName, itemnamelength ) == 0 )
				{
					if( *p != '\0')
					{
						if( *(p) == '\"' )
						{
							char * pret = (char *)(p+1);
							int length = (int)strlen( pret );
							if( *( pret + length-1 ) == '\"' )
								*( pret + length-1 ) = 0;
							return pret;
						}
						return (p);
					}
				}
			}
		}
		return NULL;

	}		
protected:
	LPCTSTR FindNextLine(LPCTSTR pStr,char LineDelimiter)
	{
		bool InStr=false;
		while(*pStr)
		{
			if(*pStr=='"')
				InStr=!InStr;
			if(*pStr==LineDelimiter&&(!InStr))
				return pStr;
			pStr++;

		}
		return NULL;
	}
};
 