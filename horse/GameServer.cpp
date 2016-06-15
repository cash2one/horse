
#include <boost/shared_ptr.hpp>
#include "timer.hpp"
#include "GameServer.h"
#include "GameDB.h"

GameServer::GameServer(const boost::asio::ip::tcp::endpoint& ep,short threadnum)
	:base(ep,threadnum)
{
}

void GameServer::run()
{
	/// test
	GameDB::instance().test0();

	base::run();

	GameDB::instance().run();
}
