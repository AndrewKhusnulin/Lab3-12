#pragma once
#include "Header.h"
#include "tcp_connection.h"
#include "chat_message.h"
#include "chat_room.h"

using boost::asio::ip::tcp;

class chat_server
{
public:
	chat_server(boost::asio::io_service& io_service, const tcp::endpoint& endpoint); 

	void	wait_for_connection(); 

private:
	void	handle_accept(const boost::system::error_code& error, boost::shared_ptr<tcp_connection>);

	boost::asio::io_service &		_io_service;
	tcp::acceptor					_acceptor; 
	boost::shared_ptr<chat_room>	_room; 
};

