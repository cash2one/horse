/********************************************************************
	created:	2014/03/14
	created:	14:3:2014   14:41
	filename: 	F:\test\horse\horse\GameClient.h
	file path:	F:\test\horse\horse
	file base:	GameClient
	file ext:	h
	author:		clauschen
	
	purpose:	
*********************************************************************/
#pragma once 
#include "client.hpp"

class GameClient : public horse::client<GameClient>
{
	typedef horse::client<GameClient> base;
	typedef horse::client_mngr<GameClient> base_mngr;
	friend class base;
	friend class base_mngr;

public:
	GameClient(horse::socket_ptr s,boost::asio::io_service& ios);
	virtual ~GameClient();
	void on_connect();
	void on_disconnect();

protected:
	bool on_message(horse::msg_head_ptr head, horse::msg_body_ptr body);
	void run();
};