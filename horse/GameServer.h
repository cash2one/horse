/********************************************************************
	created:	2014/03/14
	created:	14:3:2014   14:42
	filename: 	F:\test\horse\horse\GameServer.hpp
	file path:	F:\test\horse\horse
	file base:	GameServer
	file ext:	hpp
	author:		clauschen
	
	purpose:	
*********************************************************************/
#pragma once
#include "server.hpp"
#include "GameClient.h"

class GameServer : public horse::server<GameClient,8>
{
	typedef horse::server<GameClient,8> base;

public:
	GameServer(const boost::asio::ip::tcp::endpoint& ep,short threadnum);

protected:
	virtual void run();
};