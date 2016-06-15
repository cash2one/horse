/********************************************************************
	created:	2014/03/16
	created:	16:3:2014   10:11
	filename: 	E:\code\horse2\horse\time_wheel.hpp
	file path:	E:\code\horse2\horse
	file base:	time_wheel
	file ext:	hpp
	author:		clauschen
	
	purpose:	
*********************************************************************/
#pragma once
#include <boost/circular_buffer.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/unordered_set.hpp>
#include <boost/asio.hpp>


namespace horse
{
	using namespace boost;
	using namespace boost::asio;

	template<class T>
	struct entry
	{
		typedef weak_ptr<entry<T> > weak_entry_ptr;
		weak_ptr<T> weak_;

		entry(const shared_ptr<T>& ptr)
			: weak_(ptr)
		{
			std::cout<<this_thread::get_id()<<std::endl;
// 			std::cout<<"entry: push one.."<<std::endl;
		}

		~entry()
		{
// 			std::cout<<this_thread::get_id()<<std::endl;
			shared_ptr<T> ptr = weak_.lock();
			if(ptr)
			{
// 				std::cout<<"entry: kick it.."<<std::endl;
				ptr->exit_wheel();
			}
		}
	};
	/// todo
	template<class T>
	struct enable_time_wheel
	{

	};

	template<class T>
	class time_wheel
	{
		typedef shared_ptr< entry<T> >				entry_ptr;
		typedef unordered_set< entry_ptr >			bucket; 
		typedef typename entry<T>::weak_entry_ptr	weak_entry_ptr;

		circular_buffer<bucket> buckets_;
		io_service&				ios_;
		deadline_timer			timer_;
		int						count_;

	public:
		time_wheel(io_service& ios,int count = 0)
		: ios_(ios),timer_(ios_,posix_time::seconds(1)),count_(count)
		{
			if(count_ > 0)
			{
				timer_.async_wait(bind(&time_wheel::on_timer,this));
				buckets_.resize(count);
			}
		}

		void on_timer()
		{
			thread::id id = this_thread::get_id();
//  		std::cout<<"time_wheel: on timer"<<std::endl;
			buckets_.push_back(bucket());

			timer_.expires_from_now(posix_time::seconds(1));
			timer_.async_wait(bind(&time_wheel::on_timer,this));
		}

		void on_join(shared_ptr<T>& new_)
		{
			entry_ptr e(new entry<T>(new_));
			buckets_.back().insert(e);

 			weak_entry_ptr w(e);
 			new_->join_wheel(w,this);
		}

		void on_active(weak_entry_ptr& weak,int session)
		{
			std::cout<<"on active: "<<session<<std::endl;
			entry_ptr e = weak.lock();
			if(e)
			{
				int size = (buckets_.size());
				buckets_.back().insert(e);
			}
		}
	};
}