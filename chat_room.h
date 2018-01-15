#pragma once

#include "Header.h"
#include "chat_message.h"

class chat_session;
class chat_server;
typedef boost::shared_ptr<chat_session> chat_session_ptr;


class chat_room
{
public:
	chat_room(chat_server& server); 

	void	join(chat_session_ptr participant); 
	void	leave(chat_session_ptr participant); 
	void	deliver(const chat_message & msg); 

private:
	std::set<boost::shared_ptr<chat_session>>	_participants; 
	chat_server&								_server;
};

typedef boost::shared_ptr<chat_room>	chat_room_ptr;
typedef boost::weak_ptr<chat_room>		chat_room_wptr;
