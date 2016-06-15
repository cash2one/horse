/********************************************************************
	created:	2014/03/14
	created:	14:3:2014   14:41
	filename: 	F:\test\horse\horse\GameClient.cpp
	file path:	F:\test\horse\horse
	file base:	GameClient
	file ext:	cpp
	author:		clauschen
	
	purpose:	
*********************************************************************/
#include <iostream>
#include "GameClient.h"

GameClient::GameClient(horse::socket_ptr sock,boost::asio::io_service& ios)
	: horse::client<GameClient>(sock,ios) 
{
}

GameClient::~GameClient()
{
// 	std::cout<<"~GameClient"<<std::endl;
}

bool GameClient::on_message(horse::msg_head_ptr head, horse::msg_body_ptr body)
{
// 	std::cout<<"msg cmd:"<<head->cmd<<std::endl;

// 	std::string msg = std::string(&(*body)[0],body->size());
// 	msg = std::string((char*)head.get(), sizeof(horse::horse_msg_head)) + msg;
	
	return false;
}

void GameClient::on_connect()
{

}
void GameClient::on_disconnect()
{

}
void GameClient::run()
{

}