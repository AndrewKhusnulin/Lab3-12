#pragma once

#include "Header.h"
#include "chat_server.h"
#include "chat_room.h"
#include "tcp_connection.h"

using boost::asio::ip::tcp;

class chat_server;


class chat_session : public boost::enable_shared_from_this<chat_session>
{
public:
	~chat_session();

	static boost::shared_ptr<chat_session> create(connection_ptr tcp_connection, chat_room_ptr room) //создаем ту самую сессию
	{
		boost::shared_ptr<chat_session> session(new chat_session(tcp_connection, room));
		session->wait_for_data();
		return session;
	}

	std::string get_name() const
	{
		return _user._login;
	}

	void chat_session::close();
	void deliver(const chat_message& msg); 

private:
	chat_session(connection_ptr tcp_connection, chat_room_ptr room);
	void wait_for_data();

	void handle_write(const boost::system::error_code& error);
	void handle_read(const boost::system::error_code& error);

	chat_message						_user;

	boost::shared_ptr<tcp_connection>	_tcp_connection;
	boost::weak_ptr<chat_room>			_room; 
	chat_message						_message;

	bool								is_leaving;
};

typedef boost::shared_ptr<chat_session> chat_session_ptr;
