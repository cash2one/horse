/********************************************************************
	created:	2014/03/14
	created:	14:3:2014   14:44
	filename: 	F:\test\horse\horse\message.hpp
	file path:	F:\test\horse\horse
	file base:	message
	file ext:	hpp
	author:		clauschen
	
	purpose:	
*********************************************************************/
#pragma once
#include <boost/cstdint.hpp>
namespace horse
{
	enum horse_msgid
	{
		horse_msg_system_start = 100,
		horse_msg_system_echo = horse_msg_system_start,
		horse_msg_system_end,
		horse_msg_app_start = horse_msg_system_end,
		horse_msg_app_end
	};

#pragma pack(push,1)
	struct horse_msg_head
	{
		uint16_t	length;
		uint16_t	cmd;
		uint16_t	code;
		int32_t		reserved;

		horse_msg_head() { /*std::cout<<"horse_msg_head created"<<std::endl;*/ }
// 		~horse_msg_head(){ std::cout<<"~horse_msg_head destroyed"<<std::endl; }
		horse_msg_head(uint16_t l,uint16_t c,uint16_t cd,int32_t r): length(l),cmd(c),code(cd),reserved(r) {}
	};

	struct horse_message
	{
		horse_msg_head	head;
		char			body[1];
	};
#pragma pack(pop)
}