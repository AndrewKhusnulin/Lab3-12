#include "Header.h"

#include "chat_server.h"

#define max_count 4


int main(int argc, char* argv[])
{
	boost::thread_group g; 
	g.create_thread([]() {
		try
		{
			boost::asio::io_service io_service;//  объект класса, который предоставляет программе связь с нативными объектами ввода/вывода
			tcp::endpoint endpoint(tcp::v4(), 8001);
			chat_server server(io_service, endpoint);
			io_service.run();
		}
		catch (std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
	});
	g.join_all();
	_getch();
	return 0;
}