#include "stdafx.h"
#include "msg_def.h"
#include "RobotPool.h"

RobotPool::RobotPool(short poolid, int robotnum)
: _robotnum(robotnum)
, _poolid(poolid)
, _errorcode(eMSRetSuccess)
{

}

RobotPool::~RobotPool()
{
	RobotList::iterator it = _robotlist.begin();
	while(it != _robotlist.end())
	{
		Robot* robot = *it++;
		CC_SAFE_DELETE(robot);
	}
}

BOOL RobotPool::Start(string ip,int port, int msgintval)
{
	_ip = ip;
	_port = port;
	_msgintval = msgintval;
	srand(time(NULL));

	return Base::Start();
}

BOOL RobotPool::OnStart()
{
	for(int i = 0; i < _robotnum; i++)
	{
		int robotid = _poolid + i; //(((int)_poolid)<<16) | (i + 1);
		Robot* robot = new Robot("","NmlMqb7G6Qgb",robotid);
		robot->SetInterval(_msgintval);
		if(robot->Init(_ip,_port))
		{
			_robotlist.push_back(robot);
		}
	}

	return TRUE;
}

BOOL RobotPool::OnRun()
{
	RobotList::iterator it = _robotlist.begin();
	while(it != _robotlist.end())
	{
		Robot* robot = *it++;
		if(3 == robot->Act()) break;
	}

#ifndef _DEBUG	
	Sleep(100);
#endif

	it = _robotlist.begin();
	while(it != _robotlist.end())
	{
		Robot* robot = *it++;
		robot->Loop();
	}

	return Base::OnRun();
}

void RobotPool::OnTerminate()
{
	Base::OnTerminate();
}
