#include "GameDB.h"

bool GameDB::start()
{
	bool ret = false;

	MySQL::Library_Init();

	do 
	{
		std::string dbstring = "127.0.0.1;3306;root;111111;auth";
		int32 worker_threads = 1;
		int32 synch_threads = 1;

		// NOTE: While authserver is singlethreaded you should keep synch_threads == 1. Increasing it is just silly since only 1 will be used ever.
		ret = LoginDatabase.Open(dbstring.c_str(), uint8(worker_threads), uint8(synch_threads));
		if(!ret) break;

	} while (0);

	return ret;
}

void GameDB::stop()
{
	LoginDatabase.Close();
	MySQL::Library_End();
}

void GameDB::run()
{
	static uint32 numLoops = (30) * (60 * 1000000 / 100000);
	static uint32 loopCounter = 0;

	// Wait for termination signal
	if ((++loopCounter) == numLoops)
	{
		loopCounter = 0;
		LoginDatabase.KeepAlive();
	}
}

void GameDB::test0()
{
	static bool done = false;
	if(done) return;
	done = true;
	///////////////////////////////////////////////////////////////////////////////////////
	/// Sync operation
	////////////////////////////////////////////////////////////////////////////////////////
	PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_SEL_REALMLIST);
	PreparedQueryResult result = LoginDatabase.Query(stmt);

	// Circle through results and add them to the realm map
	if (result)
	{
		do
		{
			Field* fields = result->Fetch();
			uint32 realmId              = fields[0].GetUInt32(); std::cout<<realmId<<" ";
			std::string name            = fields[1].GetString(); std::cout<<name<<" ";
			std::string externalAddress = fields[2].GetString(); std::cout<<externalAddress<<" ";
			std::string localAddress    = fields[3].GetString(); std::cout<<localAddress<<" ";
			std::string localSubmask    = fields[4].GetString(); std::cout<<localSubmask<<" ";
			uint16 port                 = fields[5].GetUInt16(); std::cout<<port<<" ";
			uint8 icon                  = fields[6].GetUInt8(); std::cout<<icon<<" ";
			RealmFlags flag             = RealmFlags(fields[7].GetUInt8()); std::cout<<flag<<" ";
			uint8 timezone              = fields[8].GetUInt8(); 
			uint8 allowedSecurityLevel  = fields[9].GetUInt8(); 
			float pop                   = fields[10].GetFloat(); 
			uint32 build                = fields[11].GetUInt32(); 

			std::cout<<std::endl;
		}
		while (result->NextRow());
	}

	QueryResult result0 = LoginDatabase.Query("SELECT id, name FROM rbac_roles");
	if (!result0)	return;

	do
	{
		Field* field = result0->Fetch();
		uint32 id = field[0].GetUInt32();			std::cout<<id<<" ";
		std::string name = field[1].GetString();	std::cout<<name<<" "<<std::endl;
	}while (result0->NextRow());

	///////////////////////////////////////////////////////////////////////////////////////
	/// Async operation
	////////////////////////////////////////////////////////////////////////////////////////
	LoginDatabase.Execute(LoginDatabase.GetPreparedStatement(LOGIN_DEL_EXPIRED_IP_BANS));
}