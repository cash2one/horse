/********************************************************************
	created:	2014/03/14
	created:	14:3:2014   14:42
	filename: 	F:\test\horse\horse\main.cpp
	file path:	F:\test\horse\horse
	file base:	main
	file ext:	cpp
	author:		clauschen
	
	purpose:	
*********************************************************************/
#include <iostream>
#include "GameServer.h"
#include "GameClient.h"
#include "GameDB.h"

int main(int argc, char* argv[])
{
	do 
	{
		if(!GameDB::instance().start())
		{
			std::cout<<"Game Database connect fail, stop"<<std::endl;
			break;
		}

		boost::asio::ip::tcp::endpoint endpoint(boost::asio::ip::tcp::v4(), 2014);
		GameServer svr(endpoint, 4);
		svr.start();

		GameDB::instance().stop();

	} while (0);

	::system("pause");
	return 0;
}