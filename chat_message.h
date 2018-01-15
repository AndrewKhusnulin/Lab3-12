#pragma once
#include <string>
#include <list>
#include "Header.h"

class chat_message
{
private:
	
public:

	void reset() //�����
	{
		_list_string.clear();
		_message.clear();
		_login.clear();
	}

	int _type; // ��� ���������
	
	std::list<std::string>	_list_string; 

	std::string		_message; 
	std::string		_login; 
				
	

	template<class Archive> //����� ��� ����������� ������������
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
