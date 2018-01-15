#include "chat_session.h"
#include "chat_server.h"

chat_session::chat_session(connection_ptr tcp_connection, chat_room_ptr room) : _tcp_connection(tcp_connection),
	_room(room)
{
	is_leaving = false;
	std::cout << "New chat_session ! " << std::endl;
}

chat_session::~chat_session()
{
	_tcp_connection->socket().close();
	std::cout << "Session destruct" << std::endl;
}

void chat_session::close()
{
	_tcp_connection->socket().close();
}

void chat_session::wait_for_data() 
{
	_tcp_connection->async_read(_message,
		boost::bind(&chat_session::handle_read, shared_from_this(),
			boost::asio::placeholders::error)
		);
}

void chat_session::handle_read(const boost::system::error_code &error) //Обработка чтения сообщения
{
	boost::shared_ptr<chat_room> room = _room.lock();
	if (room)
	{
		if (!error)
		{
			room->deliver(_message); 				  
			wait_for_data(); 
		}

		else
		{
			if (!is_leaving)
			{
				is_leaving = true;
				room->leave(shared_from_this()); 
			}
		}
	}
}

void chat_session::deliver(const chat_message& msg) //Доставка сообщения
{
	_tcp_connection->async_write(msg,
		boost::bind(&chat_session::handle_write, shared_from_this(),
			boost::asio::placeholders::error)
		);
	if (msg._type == chat_message::AUTENTIFICATION)
	{
		_user._login = msg._login;
	}
}

void chat_session::handle_write(const boost::system::error_code &error) //Обработка записи
{
	boost::shared_ptr<chat_room> room = _room.lock();
	if (room && error && (!is_leaving))
	{
		is_leaving = true;
		room->leave(shared_from_this());
	}
}


