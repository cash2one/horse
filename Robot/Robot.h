#pragma once
#include <string>
#include "GameRoot.h"

using namespace std;

class Robot;
typedef int CmdID;
typedef int (Robot::*Action)();

typedef struct  
{
	GameState	state;
	Action		action;
}ActionDef;

class Robot : public GameRoot
{
public:
	typedef enum 
	{
		enum_idle_none,
		enum_idle_ladder,
		enum_idle_exchagne,
		enum_idle_shop,
		enum_idle_play,
		enum_idle_cost,
		enum_idle_useitem, 
		enum_idle_num 
	}IdleEventID;

	typedef GameRoot Base;
	Robot(char* account,char* psw,int robotid);
	~Robot();

	bool Init(string ip,int port);
	int Act();
	int Loop();

private:
	
	int Login();
	int Logout();
	int Idle();
	int Create();
	int UseItem();

	string GetRobotName();
	string GetRobotAccount();

	virtual int GetEventID()
	{
		return _eventid;
	}

	virtual int GetRobotID()
	{
		return _robotid;
	}

private:
	int _robotid;
	int _errorcode;
	int _logincount;
	int _eventid;

	string _ip;
	int _port;
	bool connected;
	static long _ref;
	static ActionDef _actionlist[];
};