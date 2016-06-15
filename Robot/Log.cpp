#include "Log.h"
#include "GameRoot.h"
#include <time.h>
#include <stdarg.h>

CLog::CLog()
{
	mfile = NULL;
	//Init();
}

CLog::~CLog()
{
 	if(mfile) fclose(mfile);
}

bool CLog::Init()
{  
#if (1) //CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
// 	if(!CCFileUtils::sharedFileUtils()->isFileExist("helpFile")) {
// 		_mkdir("helpFile");
// 	}
//     mfile = fopen("helpFile/errMsg.log","w");
#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    std:string path = CCFileUtils::sharedFileUtils()->getWritablePath() + "helpFile";
    if(!CCFileUtils::sharedFileUtils()->isFileExist(path)) {
        mkdir(path.c_str(),S_IRUSR | S_IREAD | S_IRWXU | S_IRWXO);
    }
    mfile = fopen(path.append("/errMsg.log").c_str(),"w");
#endif

    if(mfile)
		return true;
	return false;
}

void CLog::GetCurTime(char* timeBuf)
{
	if (timeBuf == NULL)
		return;

	struct tm *newtime;
	time_t long_time; 
	time( &long_time );                 
	newtime = localtime( &long_time );
	sprintf(timeBuf, "[%ld-%02ld-%02ld %02d:%02d:%02d]", newtime->tm_year, newtime->tm_mon, newtime->tm_mday, newtime->tm_hour, newtime->tm_min, newtime->tm_sec);
}

void CLog::WriteLog(const char *fileInfo,const char* format...)
{
	char  str[256];
	GetCurTime(str);

	char  str1[MAX_LOG_LENGTH+1];
	strcpy(str1,str);

	int abc = strlen(str1);
	strcpy(&str1[strlen(str1)],fileInfo);
	
	va_list argp; 
	va_start(argp,format); 

	abc = strlen(str1);

	_vsnprintf(&str1[strlen(str1)], MAX_LOG_LENGTH - strlen(str1), format, argp); 

	va_end(argp);
	//strcpy(&str1[strlen(str1)],"\n");

// 	if(!mfile) {
// 		MsgBox("文件错误","请先删除:helpFile/errMsg.txt\n文件再重新运行");
//         return ;
//     }
// 	fwrite(str1,sizeof(char),strlen(str1),mfile);
// 	fflush(mfile);

	std::cout<<str1<<std::endl;
}