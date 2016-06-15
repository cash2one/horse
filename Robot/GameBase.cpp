#include "stdafx.h"
#include "GameBase.h"

GameBase::GameBase()
: _curtick(0),_interval(0),_waittick(0),_waittimeout(false)
, _msgtick(0)
{

}

bool GameBase::CheckInterval()
{
	unsigned int nowtick = GetTickCount();
	if(nowtick - _curtick > _interval)
	{
		_curtick = nowtick;
		return true;
	}
	return false;
}

bool GameBase::CheckWaitTick()
{
	unsigned int nowtick = GetTickCount();
	if(nowtick - _waittick > 20000)
	{
		_waittick = nowtick;
		_waittimeout = true;
		return true;
	}
	return false;
}

void GameBase::SetInterval(unsigned int interval)
{
	_interval = interval;
}

void GameBase::SetWaitTick()
{
	_waittick = GetTickCount();
	_msgtick = _waittick;
	_waittimeout = false;
}

void GameBase::SetMsgTick()
{
	_msgtick = GetTickCount() - _msgtick;
}

unsigned int GameBase::GetMsgTick()
{
	return _msgtick;
}