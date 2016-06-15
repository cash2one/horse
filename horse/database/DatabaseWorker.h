/*
 * Copyright (C) 2008-2013 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _WORKERTHREAD_H
#define _WORKERTHREAD_H

#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/lockfree/queue.hpp>

#include "MySQLConnection.h"
// class MySQLConnection;

class DatabaseWorker
{
    public:
        DatabaseWorker(boost::lockfree::queue<SQLOperation*>* new_queue, MySQLConnection* con);

        int svc();
		int wait()
		{
			exit_ = true;
			thread_->join(); 
			return 0;
		}

    private:
        boost::lockfree::queue<SQLOperation*>* m_queue;
        MySQLConnection* m_conn;

		bool exit_;
		boost::shared_ptr<boost::thread> thread_;
};

#endif
