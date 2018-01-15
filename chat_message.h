#pragma once
#include <string>
#include <list>
#include "Header.h"

class chat_message
{
private:
	
public:

	void reset() //сброс
	{
		_list_string.clear();
		_message.clear();
		_login.clear();
	}

	int _type; // тип сообщения
	
	std::list<std::string>	_list_string; 

	std::string		_message; 
	std::string		_login; 
				
	

	template<class Archive> //архив для последующей сериализации
	void serialize(Archive& ar, const unsigned int version) {
		ar & _type & _list_string & _message & _login;
	}

	enum {
		NEW_MSG = 0,
		PERSON_LEFT = 1,
		PERSON_CONNECTED = 2,
		AUTENTIFICATION = 3,
	};
};
