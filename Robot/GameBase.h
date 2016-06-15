#pragma once
#include <time.h>

class GameBase
{
public:
	GameBase();

	bool CheckInterval();
	bool CheckWaitTick();
	void SetWaitTick();
	void SetInterval(unsigned int);
	void SetMsgTick();
	unsigned int GetMsgTick();

protected:
	unsigned int _curtick;
	unsigned int _msgtick;

	unsigned int _waittick;
	unsigned int _interval;
	bool _waittimeout;
};