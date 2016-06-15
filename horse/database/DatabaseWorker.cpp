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
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/date_time.hpp>

#include "DatabaseWorker.h"

DatabaseWorker::DatabaseWorker(boost::lockfree::queue<SQLOperation*>* new_queue, MySQLConnection* con) :
m_queue(new_queue),
m_conn(con), exit_(false)
{
	thread_.reset(new boost::thread( boost::bind(&DatabaseWorker::svc,this)));
}

int DatabaseWorker::svc()
{
    if (!m_queue)
        return -1;

    SQLOperation *request = NULL;
    while (!exit_)
    {
//         request = (SQLOperation*)(m_queue->dequeue());
		bool result = m_queue->pop(request);
        if (!result){
			//boost::this_thread::sleep(boost::posix_time::milliseconds(50));
			::Sleep(50);
            continue;
		}

        request->SetConnection(m_conn);
        request->call();

        delete request;
    }

    return 0;
}
