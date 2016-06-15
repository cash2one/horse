/********************************************************************
	created:	2014/03/14
	created:	14:3:2014   14:44
	filename: 	F:\test\horse\horse\server.hpp
	file path:	F:\test\horse\horse
	file base:	server
	file ext:	hpp
	author:		clauschen
	
	purpose:	
*********************************************************************/
#pragma once

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/make_shared.hpp>
#include <boost/progress.hpp>
#include "horse.hpp"
#include "client.hpp"
#include "client_mngr.hpp"
#include "time_wheel.hpp"
#include "timer.hpp"

namespace horse
{
	using namespace boost;
	using namespace boost::asio;
	using namespace boost::asio::ip;

	template<class T,int AutoKick = 0>
	class server
	{
		io_service							server_ios_;
		io_service							client_ios_;
		io_service							main_ios_;		//main io service
		shared_ptr<io_service::work>		work_;			
		thread_group						threads_;		//async network thread pool
		tcp::acceptor						acceptor_;
		bool								exit_;
		short								threadnum_;
		shared_ptr<time_wheel<T> >			tw_;
		//timer								timer_;
		signal_set							signals_;

	public:
		typedef shared_ptr<T>				client_ptr;

		explicit server(const tcp::endpoint& ep,short threadnum)
		:	acceptor_(client_ios_),exit_(false),threadnum_(threadnum),
			//timer_(main_ios_,0,10,bind(&server::run,this)),
			signals_(main_ios_)
		{
			signals_.add(SIGINT);
			signals_.add(SIGTERM);
			//signals_.add(SIGQUIT);
			signals_.async_wait(boost::bind(&server::handle_stop, this));

			// Open the acceptor with the option to reuse the address (i.e. SO_REUSEADDR).
			acceptor_.open(ep.protocol());
			acceptor_.set_option(boost::asio::ip::tcp::acceptor::reuse_address(true));
			acceptor_.bind(ep);
			acceptor_.listen();

			start_accept();

			if(AutoKick > 0) tw_.reset(new time_wheel<T>(main_ios_,AutoKick));
		}

		/// test of the memory pool performance
		void test0()
		{
			std::vector<client_ptr> v;
			socket_ptr sock(make_shared<tcp::socket>(client_ios_));

			{
				progress_timer t;
				for(int i = 0; i < 10000; ++i)
				{
					v.push_back(shared_ptr<T>(new T(sock,main_ios_)));
					//v.push_back(shared_ptr<T>(make_shared<T>(sock,main_ios_)));
					//v.push_back(shared_ptr<T>(T::make_client(sock,main_ios_)));
				}
			}
			{
				progress_timer t;
				v.clear();
			}
			return ;
		}

		void start()
		{
//   		work_.reset(new io_service::work(main_ios_));
			for (short i = 0; i < threadnum_; ++i)
			{
				threads_.create_thread(bind(&server::async_run, this));
			}
			threads_.create_thread(bind(&server::main_run,this));
			threads_.join_all();
		}

		void stop()
		{
			exit_ = true;
			client_ios_.stop();
			main_ios_.stop();
		}

		boost::asio::io_service& main_service()
		{
			return main_ios_;
		}

	protected:
		virtual void run()
		{
			client_mngr<T>::instance().run();
		}

		void main_run()
		{
			while(!exit_)
			{
				std::size_t ret = 0;
				int count = 10;
				while(count--) ret += main_ios_.poll_one();
					//ret = main_ios_.run();

				/// another way to run logic things is here
				run();
				this_thread::sleep(posix_time::millisec(10));
			}
		}

		void async_run()
		{
			while(!exit_)
			{
				client_ios_.run_one();
			}
		}
		
		void start_accept()
		{
// 			std::cout<<this_thread::get_id()<<std::endl;
			socket_ptr new_socket(make_shared<tcp::socket>(client_ios_));
			acceptor_.async_accept( *new_socket,
				bind(&server::post_accept, this, new_socket, placeholders::error));
		}

		void post_accept(socket_ptr sock,const system::error_code& error)
		{
			main_ios_.post(boost::bind(&server::handle_accept, this, sock,error));
		}

		void handle_accept(socket_ptr sock,const system::error_code& error)
		{
			if (!error)
			{
// 				thread::id id = this_thread::get_id();
// 				std::cout<<"on accept:"<<this_thread::get_id()<<std::endl;
				client_ptr new_(new T(sock,main_ios_));
				new_->on_connect();

				if(tw_)
				{
					tw_->on_join(new_);
				}
				new_->start();
				start_accept();

			}else
			{
				using namespace std;
				cerr<<"handle_accept fail:"<<error.message()<<endl;
			}
		}

		void handle_stop()
		{
			std::cout<<"handle stop"<<std::endl;
			stop();
		}
	};
}