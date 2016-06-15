/******************************************************************************
* Copyright (c) 2012,上海好玩研发中心
* All rights reserved.
* 
* 文件名称：Public.h
* 摘    要：服务器公用库
* 
* 当前版本：1.0
* 作    者：Lu ZhiHua
* 建立日期：2006年12月27日
**********************************************************************************/
#pragma once


#if COMPILER == COMPILER_MICROSOFT
typedef __int64            int64;
typedef long               int32;
typedef short              int16;
typedef char               int8;
typedef unsigned __int64   uint64;
typedef unsigned long      uint32;
typedef unsigned short     uint16;
typedef unsigned char      uint8;
#else
//typedef long long int64;
typedef __int64_t   int64;
typedef __int32_t   int32;
typedef __int16_t   int16;
typedef __int8_t    int8;
typedef __uint64_t  uint64;
typedef __uint32_t  uint32;
typedef __uint16_t  uint16;
typedef __uint8_t   uint8;
typedef uint16      WORD;
typedef uint32      DWORD;
#endif

typedef unsigned __int64 ULONG64;

#include "Windows.h"
#include "string"
#include "map"
#include "vector"
#include "string"
#include "queue"
using namespace std;

#include "Winsock2.h"

#ifdef _DEBUG
//#define new              new( __FILE__, __LINE__ )
#endif

#include "LogUtil.h"
#include "..\..\SDGPFrame\Server\Include\SDBase.h"
//#include "..\..\SDGPFrame\Server\Include\SDError.h"
#include "..\..\SDGPFrame\Server\Include\SDNet.h"
#include "..\..\SDGPFrame\Server\Include\SDDB.h"
#include "..\..\SDGPFrame\Server\Include\SDEvent.h"
#include "..\..\SDGPFrame\Server\Include\SDRecordSet.h"
#include "..\..\SDGPFrame\Server\Include\SDPipe.h"
#include "..\..\SDGPFrame\Server\Include\SDTimer.h"
//#include "..\..\SDGPFrame\Server\Include\SDPT.h"

//#include "..\..\SDGPFrame\Server\Protocol\CenterProto.h"
#include "..\..\SDGPFrame\Server\Protocol\LGProto.h"
#include "..\..\SDGPFrame\Server\Protocol\LG_ProtHeader.h"
//#include "..\..\SDGPFrame\Server\Protocol\CT_ProtHeader.h"
#include "..\..\SDGPFrame\Server\Protocol\ShopProtocol.h"
#include "..\..\SDGPFrame\Server\Protocol\SP_ProtHeader.h"
#include "..\..\SDGPFrame\Server\Protocol\SDErrCode.h"

#include "..\..\SDGPFrame\Server\Include\SDHelper\Thread.h"
#include "..\..\SDGPFrame\Server\Include\SDHelper\Config.h"

#include "..\..\SDGPFrame\Server\Include\SDHelper\Misc.h"

//#ifdef _DEBUG
//#define SS_TRY_BEGIN
//#define TRY_END_INFO(a,b)
//#endif

using namespace SGDP;


#define MAX_CONTROL_PANEL_MSG_LEN	20480						//控制台可显示的最多消息

#include "NameObject.h"

#include "xPacket.h"
#include "BaseTimer.h"
#include "DBErrHandle.h"
#include "DBEventEx.h"
#include "LoadDll.h"
#include "SlkReader.h"

#include "StaticObject.h"
#include "EasyTimer.h"
#include "EasyCriticalSection.h"
#include "AutoLock.h"
#include "EasyThread.h"
#include "ThreadSafeList.h"
#include "ThreadSafeIDStorage.h"
#include "SettingFile.h"
#include "StringSplitter.h"
#include "EasyString.h"
#include "IDStorage.h"
#include "Base64.h"

#include "ControlPanel.h"


#include "ServerConsoleDlg.h"
#include "ServerThread.h"

#include "DBInterfaces.h"

#include <sql.h>
#include <sqlext.h>
#include <odbcss.h>

#include <WinSock2.h>
#include <MSWSock.h>
#include "MySQL\include\mysql.h"

#include "MySQLValue.h"
#include "MySQLRecordSet.h"
#include "MySQLConnection.h"
#include "MySQLDatabase.h"

#include "DBTansaction.h"
#include "DBTransationWorkThread.h"
#include "DBTransationManager.h"

#include "IPAddress.h"

#include "NameStorage.h"
#include "ExpressionCaculator.h"


#include "Tools.h"
#include "keywordfiltratethread.h"
#ifndef SIZEOF_STR
#define SIZEOF_STR(str) sizeof(str)-1
#endif //SIZEOF_STR

#pragma comment(lib,"winmm.lib")
#ifdef _DEBUG
#pragma comment(lib,"SDHelperLD.lib")
//#pragma comment(lib,"SDErrorD.lib")
#else
#pragma comment(lib,"SDHelperL.lib")
//#pragma comment(lib,"SDError.lib")
#endif
#pragma comment(lib,"Version.lib")
