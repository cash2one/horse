/********************************************************************
	created:	2014/03/14
	created:	14:3:2014   14:44
	filename: 	F:\test\horse\horse\client.cpp
	file path:	F:\test\horse\horse
	file base:	client
	file ext:	cpp
	author:		clauschen
	
	purpose:	
*********************************************************************/
#include <iostream>
#include <algorithm>
#include <iterator>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/make_shared.hpp>
#include "server.hpp"
#include "client.hpp"

namespace horse
{
	static int count = 0;
	client::client(socket_ptr s,io_service& ios)
		: socket_(s), ios_(ios)/*, wheel_(NULL)*/
	{
		std::cout<<"client:"<<++count<<std::endl;
	}

	client::~client()
	{
		std::cout<<"~client:"<<--count<<std::endl;
	}

	void client::start()
	{
		on_start();

		head_ = make_shared< horse_msg_head >();
		body_ = make_shared< std::vector<char> >(0);

		asio::async_read( *socket_, buffer( head_.get(), sizeof(horse_msg_head)),
						  bind( &client::handle_read_head, shared_from_this(),placeholders::error));
	}

	void client::stop()
	{
		system::error_code ec;
		socket_->shutdown(tcp::socket::shutdown_both, ec);
	}

	void client::handle_read_head(const system::error_code& error)
	{
		if(!error)
		{
// 			std::cout<<"handle_read_head()..."<<head_->length<<std::endl;
			int body_len = head_->length - sizeof(horse_msg_head);
			if(body_len > 0)
			{
				body_->resize(body_len);
				asio::async_read( *socket_, buffer( &(*body_)[0], body_len ),
								  bind( &client::handle_read_body, shared_from_this(),placeholders::error));
			}else
			{
				post_packet();
				start();
			}
		}else
		{
			std::cerr<<"handle_read_head error"<<std::endl;
		}
	}

	void client::handle_read_body(const system::error_code& error)
	{
		if(!error)
		{
// 			std::cout<<"handle_read_body success"<<std::endl;
			post_packet();
			start();

		}else
		{
			std::cerr<<"handle_read_head error"<<std::endl;
		}
	}

	void client::handle_write_msg(const system::error_code& error)
	{
		if(!error)
		{
			// std::cout<<"handle_write_msg success"<<std::endl;
		}else
		{
			std::cerr<<"handle_write_msg error"<<std::endl;
		}
	}

	void client::post_packet()
	{
		ios_.post( boost::bind(&client::on_message, shared_from_this(), head_, body_));
	}
	
	void client::send_message(uint16_t cmd,uint16_t code,int32_t reserved,char* data,int len)
	{
		horse_msg_head head (sizeof(horse_msg_head), cmd, code, reserved);
		send_message(head,data,len);
	}
	
	void client::send_message(horse_msg_head& head,char* data,int len)
	{
		head.length = sizeof(head) + len;

		/// async send mode
// 		msg_.clear();
// 		msg_.append((char*)&head,sizeof(head));
// 		if(data != NULL) msg_.append(data,len);
// 
// 		asio::async_write_some( *socket_,
// 			buffer(msg_.data(), msg_.length()),
// 			bind(&client::handle_write_msg, shared_from_this(), placeholders::error));
// 		system::error_code e;
// 		asio::write( *socket_, buffer(msg_.data(), msg_.length()), e);
// 		if(!e)
// 		{
// 			std::cout<<"send succcess!"<<std::endl;
// 		}

		/// sync send mode
// 		shared_ptr< std::string > pstr = make_shared<std::string>();
		std::string msg;
		msg.append((char*)&head,sizeof(head));
		if(data != NULL) msg.append(data,len);

// 		asio::async_write( *socket_,
// 			buffer(msg.data(), msg.length()),
// 			bind(&client::handle_write_msg, shared_from_this(), placeholders::error));
		system::error_code e;
		asio::write( *socket_, buffer(msg.data(), msg.length()), e);
		if(e) { std::cout<< "Send message fail:"<<e.message()<<std::endl; }
	}

	bool client::on_message(msg_head_ptr head, msg_body_ptr body)
	{
		/// todo
// 		if(wheel_ != NULL) wheel_->on_active(weak_entry_);
		if(head->cmd == horse::horse_msg_system_echo)
		{
			send_message(*head, body->data(),body->size());
			return true;
		}

		return false;
	}
}