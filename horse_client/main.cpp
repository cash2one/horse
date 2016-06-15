#include <cstdlib>
#include <deque>
#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <boost/thread/thread.hpp>
#include "chat_message.hpp"

using boost::asio::ip::tcp;

typedef std::deque<chat_message> chat_message_queue;

#pragma pack(push,1)
struct horse_msg_head
{
	uint16_t	length;
	uint16_t	cmd;
	uint16_t	code;
	int32_t		reserve;

	horse_msg_head(){}
};

struct horse_message
{
	horse_msg_head	head;
	char			body[1];
};

#pragma pack(pop)

static const char* msg[] = 
{
	"000",
	"1111",
	"22222",
	"333333",
	"4444444"
};

class chat_client
{
	bool can_read;
public:
	chat_client(boost::asio::io_service& io_service,
		tcp::resolver::iterator endpoint_iterator)
		: io_service_(io_service),
		socket_(io_service), can_read(false)
	{
		boost::asio::async_connect(socket_, endpoint_iterator,
			boost::bind(&chat_client::handle_connect, this,
			boost::asio::placeholders::error));
	}

	void read()
	{
// 		if(!can_read) { can_read = true; return; }

		boost::asio::async_read(socket_,
			boost::asio::buffer(&head_,sizeof(head_)),
			boost::bind(&chat_client::handle_read_header, this,
			boost::asio::placeholders::error));
	}

	void write(const std::string& msg)
	{
		io_service_.post(boost::bind(&chat_client::do_write, this, msg));
	}

	void close()
	{
		io_service_.post(boost::bind(&chat_client::do_close, this));
	}

	void send_one_message()
	{
		using namespace std; // For strlen and memcpy.
		horse_msg_head head;
		head.cmd = 100;
		head.code = 300;
		head.reserve = 400;
		head.length = sizeof(head);

		static int i = 3;
		string line( 4 * 1024, msg[i][0]);
		i = (i + 5) % 5;
		head.length += line.length();

		line = string((char*)&head, sizeof(head)) + line;
		write(line);
	}

private:

	void handle_connect(const boost::system::error_code& error)
	{
		static int count = 0;
		if (!error)
		{
			std::cout<<"connect success:"<<++count<<std::endl;
			read();
			send_one_message();
			/// try send multi message 
// 			send_one_message();
		}
	}

	void handle_read_header(const boost::system::error_code& error)
	{
		if(!error)
		{
			int body_len = head_.length - sizeof(horse_msg_head);
			if(body_len > 0)
			{
				read_msg_[body_len] = 0;
				boost::asio::async_read( socket_, boost::asio::buffer( read_msg_, body_len ),
										boost::bind( &chat_client::handle_read_body, this, boost::asio::placeholders::error));
			}else
			{
				read();
				send_one_message();
			}
		}else
		{
			do_close();
		}
	}

	void handle_read_body(const boost::system::error_code& error)
	{
		if(!error)
		{
			//std::cout<<"handle_read_body:"<<read_msg_<<std::endl;
			read();
			send_one_message();
		}else
		{
			do_close();
		}
	}

	void do_write(std::string msg)
	{
		boost::asio::async_write(socket_,
				boost::asio::buffer(msg.data(),	msg.length()),
				boost::bind(&chat_client::handle_write, this,
				boost::asio::placeholders::error));
	}

	void handle_write(const boost::system::error_code& error)
	{
		if (!error)
		{
			/// do not start read here,or will start more than 1 times. 
			/// It will cause problem if send multi message one times
			//read();
		}
		else
		{
			do_close();
		}
	}

	void do_close()
	{
		socket_.close();
	}

private:
	boost::asio::io_service& io_service_;
	tcp::socket socket_;
	horse_msg_head head_;
	char read_msg_[10240];
	chat_message_queue write_msgs_;
};

int main00(int argc, char* argv[])
{
	try
	{
		boost::asio::io_service io_service;

		tcp::resolver resolver(io_service);
		tcp::resolver::query query("127.0.0.1", "2014");
		tcp::resolver::iterator iterator = resolver.resolve(query);

		chat_client c(io_service, iterator);
		boost::thread t(boost::bind(&boost::asio::io_service::run, &io_service));

		do 
		{
// 			c.send_one_message();
// 			c.send_one_message();

		} while (0);

		t.join();
		std::cout<<"thread exit"<<std::endl;
		c.close();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}


class robot
{
public:
	boost::asio::io_service& io_service;
	robot(boost::asio::io_service& ios):io_service(ios)
	{

	}
	void run()
	{
		try
		{
			boost::asio::ip::tcp::resolver resolver(io_service);
			boost::asio::ip::tcp::resolver::query query("127.0.0.1", "2014");
// 			boost::asio::ip::tcp::resolver::query query("www.google.com", "80");
			boost::asio::ip::tcp::resolver::iterator iterator = resolver.resolve(query);

			chat_client c(io_service, iterator);
			io_service.run();

			std::cout<<"thread exit"<<std::endl;
			c.close();
		}
		catch (std::exception& e)
		{
			std::cerr << "Exception: " << e.what() << "\n";
		}
	}
};
// boost::asio::io_service robot::io_service;

int main(int argc, char* argv[])
{
	try
	{
		boost::asio::io_service ios[10];

		int count = 1;
		boost::thread_group tg;
		while(count--)
		{
			robot* r = new robot(ios[count/10]);
			tg.create_thread(boost::bind(&robot::run, r));
// 			boost::this_thread::sleep(boost::posix_time::seconds(1));
		}
		tg.join_all();
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}

	return 0;
}