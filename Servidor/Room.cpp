#include "Room.h"

Room::Room() {}

Room::Room(int _id, int _roomSize, std::string _password)
{
	id = _id;
	maxUsers = _roomSize;
	password = _password;
	
	if (password.empty() || password == " ")
	{
		hasPassword = false;
	}
}

Room::~Room()
{
	clients.clear();
	delete this;
}

unsigned short Room::GetMaxUsers()
{
	return maxUsers;
}

bool Room::HasPassword()
{
	return hasPassword;
}

bool Room::IsPasswordOk(std::string _password)
{
	return password == _password;
}

void Room::AddUserToRoom(ClientData* _clientData)
{
	if(clients.size()<maxUsers)
	{
		clients.push_back(_clientData);
		if (clients.size() == maxUsers) 
		{
			StartGame();
		}
	}
}

void Room::DisconnectUserFromRoom(ClientData* _clientData)
{
	clients.remove(_clientData);
}

void Room::StartGame()
{
	// TODO: Enviar a todos los datos a los clientes antes de empezar la partida
	// (de forma escalada, el sigiente cliente recibe los datos de sus anteriores)
	//
	// eviar mensaje de "ecuchar connect de otros clientes"
	// al finalizar limpiar / eliminar sala
	for (int i = 0; i < clients.size(); i++) 
	{
		std::string message = GetMessageProtocolFrom(Message_Protocol::SEND_PLAYERS_IP_PORT) + std::to_string(maxUsers) + "_";
		message += std::to_string(i) + "_";

		int j = 0;
		for(std::list<ClientData*>::iterator it = clients.begin(); it != clients.end(); ++it )
		{
			if (i != j) {
				message += (*it)->toString() + "_";
				j++;
			}
			else 
			{
				break;
			}
		}
		sf::Packet pack;
		pack << message;
		std::list<ClientData*>::iterator it2 = clients.begin();

		for (int j = 0; j < i; j++) {
			it2++;
		}

		(*it2)->socket->Send(pack);

	}
	clients.clear();
}

std::string Room::toString() 
{
	std::string roomInfo = std::to_string(id) + " | "
		+ std::to_string(clients.size()) + " of " + std::to_string(maxUsers);
	if (hasPassword) {
		roomInfo += " | Room with Password_1";
	}
	else
	{
		roomInfo += " | Room without Password_0";
	}
	return roomInfo + "_";
}
