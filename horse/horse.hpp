/********************************************************************
	created:	2014/03/14
	created:	14:3:2014   14:43
	filename: 	F:\test\horse\horse\horse.hpp
	file path:	F:\test\horse\horse
	file base:	horse
	file ext:	hpp
	author:		clauschen
	
	purpose:	
*********************************************************************/
#pragma once

namespace horse
{
	using namespace boost;
	using namespace boost::asio;
	using namespace boost::asio::ip;

	typedef shared_ptr< tcp::socket >		socket_ptr;
}