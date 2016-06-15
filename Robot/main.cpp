#include "stdafx.h"
#include "RobotPool.h"
#include "SettingFile.h"
#include <conio.h>

#define  MIN_POOL_NUM 10
#define  MAX_POOL_NUM 20

#define  MIN_ROBOT_NUM 10
#define  MAX_ROBOT_NUM 100

RobotPool** pools = NULL;
int GameVersion = 0;

int _tmain(int argc, _TCHAR* argv[])
{
	string inifile = "Robot.ini";

	if(argc == 2) 
	{
// 		cout<<"Usage: Robot Base[Base Index] N1[Pool Num] N2[Robot Num]"<<endl;
// 		system("pause");
// 		return 0;
		inifile = argv[1];
	}

	string ip;
	int port = 0;
	int startbase = 0;//atoi(argv[1]);
	int poolnum = 0;//atoi(argv[2]);
	int robotnum = 0;//atoi(argv[3]);
	int msgintval = 0;

	CSettingFile SettingFile;
	if(SettingFile.LoadFromFile(inifile.c_str()))
	{
		ip = SettingFile.GetString("Robot","IP","");
		port = SettingFile.GetInteger("Robot","Port",0);
		startbase = SettingFile.GetInteger("Robot","IDbase",0);
		poolnum = SettingFile.GetInteger("Robot","PoolNum",0);
		robotnum = SettingFile.GetInteger("Robot","RobotNum",0);
		msgintval = SettingFile.GetInteger("Robot","MsgIntev",0);
		GameVersion = SettingFile.GetInteger("Robot","Version",0);
	}
	else
	{
		cout<<"Robot.ini file open fail! Quit now.."<<endl;
		system("pause");
		return 0;
	}

	cout<<"=============Pachinko Robot Start================"<<endl;
	cout<<"With ini file = "<<inifile<<endl;
	cout<<"Server IP = "<<ip<<endl;
	cout<<"Port = "<<port<<endl;
	cout<<"Robot ID from = "<<startbase<<endl;
	cout<<"Thread Number = "<<poolnum<<endl;
	cout<<"Robot Number = "<<poolnum<<" * "<<robotnum<<" = "<<poolnum*robotnum<<endl;
	cout<<"Robot msg intval = "<<msgintval<<endl;
	cout<<"GameServer Version is "<<GameVersion<<endl;
	cout<<"================================================="<<endl;

	pools = (RobotPool**) new char[sizeof(RobotPool*) * poolnum];

	for(int i = 0; i < poolnum; i++)
	{
		RobotPool* pool = new RobotPool(startbase ,robotnum);
		pool->Start(ip,port,msgintval);
		pools[i] = pool;
		startbase += robotnum;
	}

 	while(27 != _getch());

	for(int i = 0; i < poolnum; i++)
	{
		RobotPool* pool = pools[i];
		pool->SafeTerminate(INFINITE);
		delete pool;
	}
	delete[] pools;

	return 0;
}