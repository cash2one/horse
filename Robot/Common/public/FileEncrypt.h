/******************************************************************
* Copyright (c) 2006,�Ϻ�����
* All rights reserved.
* 
* �ļ����ƣ�FileEncrypt.h
* ժ    Ҫ���ļ����ܣ����ܽӿ�
* 
* ��ǰ�汾��1.0
* ��    �ߣ�sunfei
* �������ڣ�2007-12-19
********************************************************************/

#pragma once
#include <string>
using namespace std;

//�ļ��ӽ��ܷ���ֵ
enum eDataCrypt
{
	eDCSucess,
	eDCErrOpenRawFile,
	eDCFailInRawFile,
	eDCErrOpenResultFile,
	eDCFailOutResultFile
};
//�ļ����� -- ���ļ�
int ssEncryptFile(const char* sFilePath);
//�ļ����� -- ���ļ�
int ssDecryptFile(const char* sFilePath);
//�ļ����� -- ���ڴ�
int ssDecryptFile(const char* sFilePath, string& sResult);

//���ڴ��н���INI�ļ����ֶ�
UINT ssGetIntField(const char* sSection, const char* sFieldName, UINT nDefault, const char* sContent, int nContLength);
UINT ssGetStringField(const char* sSection, const char* sFieldName, const char* sDefault, char* sFieldBuf, int nBufLength, const char* sContent, int nContLength);

