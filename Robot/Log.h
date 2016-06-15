#pragma once

#include <stdio.h>
#include <iostream>

const int MAX_LOG_LENGTH = 1024;

#define LOGFILE(code)		char strLog[256]={0};						\
							mLog->WriteLog(strLog,__FUNCTION__" [%s] Fail:%ld",GetRobotName().c_str(), code);	

#define MsgBox(a,b) {std::cout<<b<<std::endl;}

class CLog
{
public:
	CLog();
	~CLog();
	bool Init();
	void GetCurTime(char* timeBuf);
	void WriteLog(const char *fileInfo,const char* format...);
private:
	FILE *mfile;
};
