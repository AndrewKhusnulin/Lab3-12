#include "chat_server.h"
#include "chat_server.h"
#include "chat_session.h"

chat_server::chat_server(boost::asio::io_service& io_service, const tcp::endpoint& endpoint)
	:_io_service(io_service),//ссылка на существующий ио_сервис
	_acceptor(io_service, endpoint), //новый приемщик
	_room(new chat_room(*this)) //создаем комнату для этого сервера
{
	std::cout << "Create server" << std::endl;

	wait_for_connection(); //запускаем ожидания подключения
}

//Ожидание нового клиента
void chat_server::wait_for_connection()
{
	boost::shared_ptr<tcp_connection> new_connection(new tcp_connection(_io_service)); 

																	 
	_acceptor.async_accept(new_connection->socket(),
		boost::bind(&chat_server::handle_accept, this,
			boost::asio::placeholders::error,
			new_connection)
		);
}


void chat_server::handle_accept(const boost::system::error_code& error, boost::shared_ptr<tcp_connection> new_connection) // Обработка доступа
{
	if (!error)
	{
		std::cout << "Connection accept" << std::endl;
		boost::shared_ptr<chat_session> session = chat_session::create(new_connection, _room);
		_room->join(session); 
		wait_for_connection();
	}
	else {
		std::cerr << "Connection denied" << std::endl;
	}
}