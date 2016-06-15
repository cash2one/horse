#pragma once
#include <string>
#include "NetMgr.h"
#include "Log.h"
#include "CCharaDate.h"
#include "GameBase.h"
#include <vector>
#include <map>

using namespace std;

typedef enum
{ 
	enum_state_offline,
	enum_state_create,
	enum_state_idle,
// 	enum_state_shopping,
// 	enum_state_exchanging,
// 	enum_state_playing,
	enum_state_error,
	enum_state_waiting,
	enum_state_num 
} GameState;

#define ENTER_STATE(s)			EnterState(s);

// #define ENTER_STATE(s)			cout<<"EnterState:Line="<<__LINE__<<" State="<<s<<endl;		\
// 								EnterState(s);

#define RECV_PRIVATE			private:
#define RECV_MSG(cmd,fun)		void fun(const char* buf,int nLen);

class GameRoot:public GameBase
{
public :
	GameRoot();
	~GameRoot(){}
	
	void NetMsgLoop(float dt);
	bool MessageCallback(const char* buf,int nLen);

#include "MsgRecvDeclare.inl"

#undef	RECV_PRIVATE
#undef	RECV_MSG

protected:
	void EnterState(GameState state);
	int _randomx(int min,int max);
	virtual string GetRobotName()
	{
		return "";
	}
	
	virtual int GetEventID()
	{
		return 0;
	}

	virtual int GetRobotID()
	{
		return 0;
	}

protected:
	string _account;
	string _psw;

	GameState _state;

	NetMngr  _netmgr;
	NetMngr  *mNetMgr;

	CLog	 _log;
	CLog     *mLog;

	CCharaDate _charinfo;
	CCharaDate *mCharaInfo;

	vector<PropInfo> _itemlist;
	vector<ShoppingPropInfo> _shoplist;
	vector<ShoppingPropInfo> _exchangelist;
	map<int,AcquireBuffInfo> _bufflist;

	int _stageid;
	Scene_date _scenedata;
	vector<Ball_date> _balllist;
	vector<BlockObj> _blocklist;
	vector<ChoseMapListST> _stagelist;
};