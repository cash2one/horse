#pragma once
#include <string>
#include <vector>
#include "EasyThread.h"
#include "Robot.h"

using namespace std;

class RobotPool : public CEasyThread
{
public:
	typedef CEasyThread Base;
	typedef vector<Robot*> RobotList;

	RobotPool(short poolid = 1, int robotnum = 1);
	~RobotPool();

	BOOL Start(string ip,int port, int msgintval);

protected:
	virtual BOOL OnStart();
	virtual BOOL OnRun();
	virtual void OnTerminate();

private:
	short _poolid;
	int _errorcode;
	int _robotnum;
	string _ip;
	int _port;
	int _msgintval;
	RobotList _robotlist;
};