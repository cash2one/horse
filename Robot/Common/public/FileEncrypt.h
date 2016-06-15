/******************************************************************
* Copyright (c) 2006,上海好玩
* All rights reserved.
* 
* 文件名称：FileEncrypt.h
* 摘    要：文件解密，解密接口
* 
* 当前版本：1.0
* 作    者：sunfei
* 建立日期：2007-12-19
********************************************************************/

#pragma once
#include <string>
using namespace std;

//文件加解密返回值
enum eDataCrypt
{
	eDCSucess,
	eDCErrOpenRawFile,
	eDCFailInRawFile,
	eDCErrOpenResultFile,
	eDCFailOutResultFile
};
//文件解密 -- 到文件
int ssEncryptFile(const char* sFilePath);
//文件解密 -- 到文件
int ssDecryptFile(const char* sFilePath);
//文件解密 -- 到内存
int ssDecryptFile(const char* sFilePath, string& sResult);

//从内存中解析INI文件的字段
UINT ssGetIntField(const char* sSection, const char* sFieldName, UINT nDefault, const char* sContent, int nContLength);
UINT ssGetStringField(const char* sSection, const char* sFieldName, const char* sDefault, char* sFieldBuf, int nBufLength, const char* sContent, int nContLength);

