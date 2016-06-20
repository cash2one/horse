/********************************************************************
	created:	2014/03/14
	created:	14:3:2014   14:44
	filename: 	F:\test\horse\horse\client.hpp
	file path:	F:\test\horse\horse
	file base:	client
	file ext:	hpp
	author:		clauschen
	
	purpose:	
*********************************************************************/
#pragma once
#include <boost/asio.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/pool/object_pool.hpp>
#include <boost/pool/pool.hpp>
#include <boost/utility.hpp>
#include <boost/typeof/typeof.hpp>
#include "message.hpp"
#include "horse.hpp"
#include "time_wheel.hpp"
#include "server.hpp"
#include "memory_pool.hpp"

namespace horse
{
	using namespace boost;
	using namespace boost::asio;
	using namespace boost::asio::ip;

	typedef shared_ptr< horse_msg_head >	msg_head_ptr;
	typedef shared_ptr< std::vector<char> > msg_body_ptr;

	static int count = 0;

	template<class T, bool WithMemPool = true>
	class client : public enable_shared_from_this<T>, public memory_pool<T, WithMemPool>
	{
		typedef typename entry< T >::weak_entry_ptr weak_entry_ptr;
		typedef shared_ptr<T>	client_ptr;

		socket_ptr						socket_;
		msg_head_ptr					head_;
		msg_body_ptr					body_;
		io_service&						ios_;
		weak_entry_ptr					weak_entry_;
		time_wheel<T>*					wheel_ptr_;
		int								session_;
		bool							disconnected_;

		char							test[10240];

	public:
// 		static boost::object_pool<T>	client_pool_;
// 		static boost::pool<>	client_pool_;
// 		void* operator new(std::size_t size)
// 		{
// 			void* mem = client_pool_.malloc();
// 			return static_cast<T*>(mem);
// 		}
// 
// 		void operator delete(void* object)
// 		{
// 			client_pool_.free(static_cast<T*>(object));
// 		}

// 		static client_ptr make_client(const socket_ptr& sock,const io_service& ios)
// 		{
// 			return client_ptr(client_pool_.construct(sock,ios));
// 		}

		client(socket_ptr s,io_service& ios)
			: socket_(s), ios_(ios), wheel_ptr_(NULL),disconnected_(false)
		{
			++count;
// 			std::cout<<"client:"<<++count<<std::endl;
			session_ = count;
// 			std::cout<<typeid(enable_if_11<WithMemPool>::type).name() <<std::endl;

		}

		virtual ~client()
		{
			--count;
			if(count == 0) 
				std::cout<<"~client:"<<session_<<" count: "<<count<<std::endl;
		}

		void start()
		{
			head_ = make_shared< horse_msg_head >();
			body_ = make_shared< std::vector<char> >(0);

			asio::async_read( *socket_, buffer( head_.get(), sizeof(horse_msg_head)),
				bind( &client::handle_read_head, shared_from_this(),placeholders::error));
		}

		void stop()
		{
			disconnected_ = true;
			system::error_code ec;
			socket_->shutdown(tcp::socket::shutdown_both, ec);
		}

		void on_connect()
		{
			client_mngr<T>::instance().join(shared_from_this());

			return static_cast<T*>(this)->on_connect();
		}

		void on_disconnect()
		{
			disconnected_ = true;
			client_mngr<T>::instance().leave(shared_from_this());

			return static_cast<T*>(this)->on_disconnect();
		}

		void run()
		{
			return static_cast<T*>(this)->run();
		}

		/// timing wheel function
		void join_wheel(const weak_entry_ptr& ptr,time_wheel<T>* wheel)
		{
			weak_entry_ = ptr;
 			wheel_ptr_ = wheel;
			//std::cout<<autokick<<std::endl;
		}

		void exit_wheel()
		{
			stop();
		}

	protected:
		void handle_read_head(const system::error_code& error)
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
			}
            else
			{
// 				std::cerr<<"handle_read_head error"<<std::endl;
				post_fail();
			}
		}

		void handle_read_body(const system::error_code& error)
		{
			if(!error)
			{
				// std::cout<<"handle_read_body success"<<std::endl;
				post_packet();
				start();

			}
            else
			{
// 				std::cerr<<"handle_read_head error"<<std::endl;
				post_fail();
			}
		}

		void handle_write_msg(const system::error_code& error)
		{
			if(!error)
			{
				// std::cout<<"handle_write_msg success"<<std::endl;
			}
            else
			{
// 				std::cerr<<"handle_write_msg error"<<std::endl;
				post_fail();
			}
		}

		void post_packet()
		{
			ios_.post( boost::bind(&client::on_message, shared_from_this(), head_, body_));
		}

		void post_fail()
		{
			ios_.post( boost::bind(&client::on_disconnect, shared_from_this()));
		}

		void send_message(uint16_t cmd,uint16_t code,int32_t reserved,char* data = NULL,int len = 0)
		{
			if(disconnected_)
				return;

			horse_msg_head head (sizeof(horse_msg_head), cmd, code, reserved);
			send_message(head,data,len);
		}

		void send_message(horse_msg_head& head,char* data = NULL,int len = 0)
		{
			if(disconnected_)
				return;

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
			//////////////////////////////////////////////////////////////////////////////////////////
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

		bool on_message(msg_head_ptr head, msg_body_ptr body)
		{
			std::cout<<"on message:"<<this_thread::get_id()<<std::endl;
			if(wheel_ptr_ != NULL) wheel_ptr_->on_active(weak_entry_,session_);

			if(head->cmd == horse::horse_msg_system_echo)
			{
				send_message(*head, body->data(),body->size());
				return true;
			}

			return static_cast<T*>(this)->on_message(head,body);
		}

	};
	
// 	template<class T> 
// 	object_pool<T> client<T>::client_pool_(sizeof(T));
// 	pool<> client<T>::client_pool_(sizeof(T),10000);
}