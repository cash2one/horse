#include "stdafx.h"
#include "Robot.h"
#include <time.h>

#define CCLog
//#define CCLog(b) {std::cout<<"["<<GetRobotName()<<"] "<<b<<endl;}
#define CCLogX(b) {std::cout<<"["<<GetRobotName()<<"] "<<b<<endl;}

long Robot::_ref = 0;

ActionDef Robot::_actionlist[] =
{
	{ enum_state_offline,&Robot::Login },
	{ enum_state_create,&Robot::Create },
	{ enum_state_idle,&Robot::Idle },
// 	{ enum_state_idle,NULL },
	{ enum_state_error,&Robot::Logout },
};

Robot::Robot(char* account,char* psw,int robotid)
: _robotid(robotid),_logincount(0),_eventid(0)
, _errorcode(eMSRetSuccess),connected(false)
{
	_account = GetRobotAccount();
	_psw = psw;
	_state = enum_state_offline;

	if( 1 == InterlockedIncrement(&_ref))
	{
		WSADATA wsa;
		WSAStartup(MAKEWORD(2,2),&wsa);
	}
}

Robot::~Robot()
{
	_state = enum_state_offline;

	if( 0 == InterlockedDecrement(&_ref))
	{
		WSACleanup();
	}
}

bool Robot::Init(string ip,int port)
{
// 	bool ret = _netmgr.Initialize(_ip.c_str(),_port,NULL);
// 
// 	if(!ret) MsgBox("","Robot init fail..");
	_state = enum_state_offline;
	_ip = ip;
	_port = port;

	return true;
}

int Robot::Act()
{
// 	if(_state == enum_state_waiting && CheckWaitTick())
// 	{
// 		if(GetRobotID() % 100 == 0)
// 		{
// 			cout<<"["<<GetRobotName()<<"] Wait timeout:"<<_eventid<<endl;
// 			//Logout();
// 		}	
// 	}

	int count = sizeof(_actionlist)/sizeof(ActionDef);

	for(int i = 0; i < count; i++)
	{
		if(_actionlist[i].state == _state && _actionlist[i].action != NULL)
		{
 			if(!CheckInterval()) break;

			Action action = _actionlist[i].action;
			//TEST echo ¹Ø±ÕÕâ¾ä
			//ENTER_STATE(enum_state_waiting);
			return (this->*action)();
		}
	}

	return -1;
}

int Robot::Loop()
{
	GameState state = _state;
	NetMsgLoop(0.0f);

	if(_waittimeout && state == enum_state_waiting && _state != state)
	{
		cout<<"["<<GetRobotName()<<"] Wait timeout restore:"<<_eventid<<" State:"<<state<<endl;
	}

	return 0;
}

int Robot::Login()
{
	if(_logincount > 0)
	{
// 		cout<<_account<<(" Login again ...")<<_logincount<<endl;
	}

// 	_logincount++;

	if(!connected)	connected = _netmgr.Initialize(_ip.c_str(),_port,NULL);
 	_netmgr.SendEnterGame(_account.c_str(),_psw.c_str());

// 	return _robotid % 4;
	return 0;
}

int Robot::Logout()
{
	CCLog("Logout...");

	_netmgr.SendLeaveGame();
	_netmgr.DisConnect();
	
	ENTER_STATE(enum_state_offline);

	return _errorcode;
}

int Robot::Create()
{
	CCLog("Create Charactor...");
	_netmgr.SendCreateChara(GetRobotName().c_str());
	return _errorcode;
}

int Robot::UseItem()
{
	if(_itemlist.empty()) 
	{
		ENTER_STATE(enum_state_idle);
		return -1;
	}
	int count = _itemlist.size();
	int i = _randomx(0,count -1);

	//cout<<("UseItem:")<<_itemlist[i].mPropName<<endl;
	CCLog(_itemlist[i].mPropName);
	_netmgr.SendUseProp(_itemlist[i].mPropID,1,_itemlist[i].mID);

	return 0;
}

int Robot::Idle()
{
	//enum_idle_ladder,enum_idle_exchagne,enum_idle_shop,enum_idle_play,enum_idle_cost
	_eventid = _randomx(1,enum_idle_num - 1);
	CCLog(_eventid);

	switch(_eventid)
	{
	case enum_idle_ladder:
		CCLog("Get Ladder...");
		_netmgr.SendGetLadder();
		break;
	case enum_idle_exchagne:
		if( 0 == _randomx(0,20))
		{
			CCLog("Exchange...");
			_netmgr.SendGetExchangePropLisg();
		}
		else ENTER_STATE(enum_state_idle);

		break;
	case enum_idle_shop:
		CCLog("Shop...");
		_netmgr.SendGetShoppingPropList();
		break;
	case enum_idle_play:
		CCLog("Play map...");
		_netmgr.SendGetChoseMapList();
		break;
	case enum_idle_cost:
		CCLog("Buy cost tick...");
		_netmgr.SendGetBuyRollCallList();
		break;
	case enum_idle_useitem:
		UseItem();
		break;
	default:
		CCLog("Idle...default\n");
		break;
	}
	return _errorcode;
}

string Robot::GetRobotName()
{
	char tmp[24] = {0};
	sprintf(tmp,"robot%05ld",_robotid);
	return string(tmp);
}

string Robot::GetRobotAccount()
{
	return GetRobotName();
}