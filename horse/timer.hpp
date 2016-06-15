#pragma once
#include <boost/asio.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

namespace horse
{
	class timer
	{
		int count_, maxcount_, elapse_;
		boost::function<void()> f_;
		boost::asio::deadline_timer t_;

	public:
		template<class F>
		timer(boost::asio::io_service& ios, int count, int elapse, F f)
			: f_(f),maxcount_(count),count_(0), elapse_(elapse), 
			  t_(ios, boost::posix_time::millisec(elapse))
		{
			if(count >= 0)	t_.async_wait(bind(&timer::on_timer,this,boost::asio::placeholders::error));
		}

	private:
		void on_timer(const boost::system::error_code& ec)
		{
			if(ec) std::cerr<<"timer::on_timer fail:"<<ec.message()<<std::endl;

			if(maxcount_ > 0)
			{
				if(count_ >= maxcount_) return;
				++count_;
			}

			f_();

// 			std::cout<<t_.expires_at()<<std::endl;
			t_.expires_at(t_.expires_at() + boost::posix_time::millisec(elapse_));
			t_.async_wait(boost::bind(&timer::on_timer,this, boost::asio::placeholders::error));
		}
	};
}