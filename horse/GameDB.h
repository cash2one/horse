#pragma once
#include "database/TestDatabase.h"

class GameDB
{
	TestDatabaseWorkerPool LoginDatabase;

public:
	static GameDB& instance()
	{
		static GameDB mngr;
		return mngr;
	}

	bool start();
	void stop();
	void run();

	void test0();
};