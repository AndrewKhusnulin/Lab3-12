#pragma once
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>
#include <vector>
#include "tcp_connection.h" // Must come before boost/serialization headers.
#include <boost/serialization/vector.hpp>
#include "chat_message.h"

std::mutex mutex;

class client
{
public:
		
	client(boost::asio::io_service& io_service,
			const std::string& host, const std::string& service)
			: _connection(io_service) 
	{
			boost::asio::ip::tcp::resolver resolver(io_service);
			boost::asio::ip::tcp::resolver::query query(host, service);
			boost::asio::ip::tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);

			boost::asio::async_connect(_connection.socket(), 
										endpoint_iterator,
										boost::bind(&client::handle_connect, 
													this,
													boost::asio::placeholders::error));
	}

	void handle_connect(const boost::system::error_code& e)//Функция ожидания соединения
	{
		if (!e)
		{
			do_read();
		}
		else
		{
			std::cerr << e.message() << std::endl;
		}
	}	
	void do_read()
	{
		_connection.async_read(_in_message,
			boost::bind(&client::handle_read,
				this,
				boost::asio::placeholders::error));
	}
	void handle_read(const boost::system::error_code& e)
	{
		if (!e)
		{
			if (_in_message._type == chat_message::PERSON_LEFT)
				std::cout << _in_message._login << " have left!\n";
			if (_in_message._type == chat_message::NEW_MSG)
				if (_in_message._login != _out_message._login)
					std::cout << _in_message._login << ": "<< _in_message._message << "\n";
			if (_in_message._type == chat_message::AUTENTIFICATION)
			{
				std::cout << _in_message._login << " here!\n";
			}
			do_read();
		}
		else
		{
			std::cout << e.message() << "\n";
		}
		
	}

	void do_write(std::string & msg, int type = chat_message::NEW_MSG)
	{
		if (type == chat_message::AUTENTIFICATION)
		{
			_in_message._login = msg;
			_out_message._login = msg;
		}
		else 
			_out_message._message = msg;
		_out_message._type = type;
		_connection.async_write(_out_message, 
								boost::bind(&client::handle_write,
								this,
								boost::asio::placeholders::error));
	}

	void handle_write(const boost::system::error_code& e)
	{
		if (e)
			std::cout << e.message() << "\n";
	}
	void do_login()
	{
		std::string name;
		std::cout << "Enter your name\n";
		std::getline(std::cin, name);
		do_write(name, chat_message::AUTENTIFICATION);
	}
	private:
		tcp_connection	_connection;
		chat_message	_in_message;
		chat_message	_out_message;
};


int main()
{	
	setlocale(LC_ALL, "Russian");
	try
	{	
		boost::asio::io_service io_service;
		client client(io_service, "127.0.0.1", "8001");  
		std::thread t([&io_service]() { 
			setlocale(LC_ALL, "Russian"); 
			io_service.run(); });
		client.do_login();
		std::string output;
		while (true)
		{
			std::getline(std::cin, output);
			client.do_write(output);
			output.clear();
		}
		t.join();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	_getch();

	return 0;
}

