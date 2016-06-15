#pragma once
#include <set>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include "client.hpp"

namespace horse
{
	using namespace boost;

	template<typename T>
	class client_mngr
	{
// 		typedef client<T> base1;//todo compile error
		typedef typename T::base base;
		typedef shared_ptr<base> client_ptr;
		std::set<client_ptr> clients_;

	public:
		static client_mngr<T>& instance()
		{
			static client_mngr<T> mngr;
			return mngr;
		}

		void join(client_ptr client)
		{
			clients_.insert(client);
		}

		void leave(client_ptr client)
		{
			std::cout<<"leave:"<<this_thread::get_id()<<std::endl;
			clients_.erase(client);
		}

		void run()
		{
			for_each(clients_.begin(), clients_.end(), bind(&base::run,_1));
		}
	};
}