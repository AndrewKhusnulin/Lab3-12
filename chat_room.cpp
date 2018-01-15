#include "chat_room.h"
#include "chat_session.h"
#include "chat_server.h"

chat_room::chat_room(chat_server& server)
	:_server(server)
{
	std::cout << "New room" << std::endl;
}

void chat_room::join(chat_session_ptr participant)//Функция присоединения к серверу
{
	_participants.insert(participant);

	chat_message e;
	e._type = chat_message::PERSON_CONNECTED;
	e._login = participant->get_name();
	deliver(e);
}

void chat_room::leave(chat_session_ptr participant)//Удаляет _participants 
{
	chat_message e;
	e._type = chat_message::PERSON_LEFT;
	e._login = participant->get_name();
	deliver(e);
	_participants.erase(participant);
}

void chat_room::deliver(const chat_message& msg)//Отправляет сообщения на все _participants
{
	if (msg._type == chat_message::AUTENTIFICATION)//Проверка на одинаковые имена,в случае совпадения программа завершается.
	{
		for (const auto & x : _participants)
			if (msg._login == x->get_name())
			{
				auto p = find_if(_participants.begin(), _participants.end(),
					[&msg](const chat_session_ptr & arg)
				{ return arg->get_name() == msg._login; });
				(*p)->close();
				_participants.erase(*p);
				return;
			}
	}
	std::for_each(_participants.begin(), _participants.end(),
		boost::bind(&chat_session::deliver, _1, boost::ref(msg)));
}
