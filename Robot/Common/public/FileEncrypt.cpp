/******************************************************************
* Copyright (c) 2006,上海好玩
* All rights reserved.
* 
* 文件名称：FileEncrypt.cpp
* 摘    要：文件解密，解密接口
* 
* 当前版本：1.0
* 作    者：sunfei
* 建立日期：2007-12-19
********************************************************************/
#include "StdAfx.h"
#include "arc4.h"
#include "fileencrypt.h"

int DecryptData(char* sData, long nLength)
{
	arc4_context key1 = {0};
	unsigned char skey[10] = {0};
	int len = sizeof(skey)/sizeof(skey[0]);
	arc4_setup(&key1, skey, len);
	unsigned char a=0;
	unsigned char b=1;
	for(int i=len; i>0; --i)
	{
		unsigned char c=b;
		b += a;
		skey[i-1] = b;
		a = c;
	}
	arc4_crypt(&key1, skey, len);
	arc4_setup(&key1, skey, len);
	arc4_crypt(&key1, (unsigned char*)sData, nLength);

	return eDCSucess;
}

int ssDecryptFile(const char* sFilePath, string& sResult)
{
	FILE* pf = fopen(sFilePath, "rb");
	if(pf==NULL)
	{
		return eDCErrOpenRawFile;
	}
	fseek(pf, 0, SEEK_END);
	long nlen = ftell(pf);
	if(nlen<0L)
	{
		fclose(pf);
		return eDCFailInRawFile;
	}
	sResult.resize(nlen);
	fseek(pf, 0, SEEK_SET);
	fread(&(sResult[0]), nlen, 1, pf);

	int nRet = DecryptData(&(sResult[0]), nlen);
	fclose(pf);

	return nRet;
}

int ssEncryptFile(const char* sFilePath)
{
	string sText;
	int nRet = ssDecryptFile(sFilePath, sText);
	if(nRet!=eDCSucess)
	{
		return nRet;
	}

	char sEncryptFile[MAX_PATH] = "";
	_snprintf(sEncryptFile, sizeof(sEncryptFile)-1, "%s.%s", sFilePath, "en");
	FILE* pf = fopen(sEncryptFile, "wb");

	if(pf==NULL)
		return eDCErrOpenResultFile;

	if(fwrite(&(sText[0]), sText.size(), 1, pf)!=1)
	{
		fclose(pf);
		return eDCFailOutResultFile;
	}

	fclose(pf);

	return eDCSucess;
}

int ssDecryptFile(const char* sFilePath)
{
	string sText;
	int nRet = ssDecryptFile(sFilePath, sText);
	if(nRet!=eDCSucess)
	{
		return nRet;
	}

	char sDecryptFile[MAX_PATH] = "";
	_snprintf(sDecryptFile, sizeof(sDecryptFile)-1, "%s.%s", sFilePath, "de");
	FILE* pf = fopen(sDecryptFile, "wb");

	if(pf==NULL)
		return eDCErrOpenResultFile;

	if(fwrite(&(sText[0]), sText.size(), 1, pf)!=1)
	{
		fclose(pf);
		return eDCFailOutResultFile;
	}

	fclose(pf);

	return eDCSucess;
}

inline const char* ssGetSectName(const char* sLine)
{
	const char* pStart = strchr(sLine, '[');
	if(!pStart)
		return NULL;

	for(const char* p=sLine; p<pStart; ++p)
	{
		if(*p!=' ' && *p!='\t')
			return NULL;
	}

	const char* pEnd = strchr(pStart, ']');
	if(!pEnd)
		return NULL;

	return pStart;
}

inline const char* ssNextLine(const char* sLineEnd)
{
	if(!sLineEnd || *sLineEnd=='\0')
		return NULL;

	while(*sLineEnd!='\0')
	{
		if(*sLineEnd!='\r' && *sLineEnd!='\n')
			break;
		++sLineEnd;
	}
	return sLineEnd;
}
inline bool ssIsEqualSect(const char* sSection, const char* sSectName)
{
	if(!sSection || *sSection=='\0')
		return false;

	const char* pStart = sSection+1;
	while(*pStart!='\0')
	{
		if(*pStart!=' ' && *pStart!='\t')
			break;
		++pStart;
	}
	const char* pEnd = strchr(pStart, ']');
	if(!pEnd)
		return false;
	
	do
	{
		--pEnd;
		if(*pEnd!=' ' && *pEnd!='\t')
			break;
	}while(pEnd>pStart);
	if(pEnd>pStart 
		&& strlen(sSectName)==pEnd-pStart+1
		&& memicmp(pStart, sSectName, pEnd-pStart+1)==0)
		return true;

	return false;
}

inline const char* ssGetFieldName(const char* sLine)
{
	const char* pSpliter = strchr(sLine, '=');
	if(!pSpliter)
		return NULL;
	const char* pStart = sLine;
	while(pStart<pSpliter)
	{
		if(*pStart!=' ' && *pStart!='\t')
			break;
		++pStart;
	}
	const char* pEnd = pSpliter;
	while(pEnd>pStart)
	{
		if(*pEnd!=' ' && *pEnd!='\t')
			break;
		--pEnd;
	}
	if(pEnd-pStart==0)
		return NULL;

	for(const char* p=pStart; p<pEnd; ++p)
	{
		if(!isalnum(*p))
			return NULL;
	}

	return pStart;
}

const char* ssFindField(const char* sSectName, const char* sFieldName, const char* sContent, int nContLength)
{
	if(!sContent || nContLength<=0)
		return NULL;
	const char* pLineHead = sContent;
	const char* pLineTail = sContent;
	const char* pSection = NULL;
	int nSectLen = (int)strlen(sSectName);
	int nFieldLen = (int)strlen(sFieldName);
	while(pLineHead!=NULL && pLineHead<sContent+nContLength)
	{
		pLineTail = strchr(pLineHead, '\r');
		if(pLineTail)
			*const_cast<char*>(pLineTail) = '\0';

		if(!pSection)
		{
			const char* sTmpSect = ssGetSectName(pLineHead);
			if(sTmpSect && ssIsEqualSect(sTmpSect, sSectName))
			{
				pSection = sTmpSect;
			}
		}
		else if(ssGetSectName(pLineHead)==NULL)
		{
			const char* sTmpField = ssGetFieldName(pLineHead);
			if(sTmpField && memicmp(sTmpField, sFieldName, nFieldLen)==0)
			{
				if(pLineTail)
					*const_cast<char*>(pLineTail) = '\r';
				return sTmpField;
			}
		}
		else
		{
			if(pLineTail)
				*const_cast<char*>(pLineTail) = '\r';
			return NULL;
		}

		if(pLineTail)
			*const_cast<char*>(pLineTail) = '\r';

		pLineHead = ssNextLine(pLineTail);
	}
	return NULL;
}

UINT ssGetIntField(const char* sSection, const char* sFieldName, UINT nDefault, const char* sContent, int nContLength)
{
	const char* sField = ssFindField(sSection, sFieldName, sContent, nContLength);
	if(sField)
	{
		const char* sValue = strchr(sField, '=')+1;
		char* pTail = strchr(sValue, '\r');
		if(pTail)*pTail = '\0';
		UINT nValue = (UINT)atoi(sValue);
		if(pTail)*pTail = '\r';
		return nValue;
	}
	else
	{
		return nDefault;
	}
	return nDefault;
}

UINT ssGetStringField(const char* sSection, const char* sFieldName, const char* sDefault, char* sFieldBuf, int nBufLength, const char* sContent, int nContLength)
{
	if(!sFieldBuf || nBufLength<=0)
		return 0;

	const char* sField = ssFindField(sSection, sFieldName, sContent, nContLength);
	if(sField)
	{
		char* sValue = strchr(sField, '=')+1;
		char* pTail = strchr(sValue, '\r');
		if(pTail)*pTail = '\0';
		bool bHasQuotation = false;
		if(*sValue=='"'
			&& strlen(sValue+1)
			&& (sValue)[strlen(sValue)-1]=='"')
		{
			bHasQuotation = true;
			sValue +=1;
			sValue[strlen(sValue)-1] = '\0';
		}

		strncpy(sFieldBuf, sValue, nBufLength-1);
		if(bHasQuotation)
		{
			sValue[strlen(sValue)] = '"';
		}
		if(pTail)*pTail = '\r';
		sFieldBuf[nBufLength-1] = '\0';
	}
	else
	{
		strncpy(sFieldBuf, sDefault, nBufLength-1);
		sFieldBuf[nBufLength-1] = '\0';
	}
	return (UINT)strlen(sFieldBuf);
}