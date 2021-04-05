#include "Room.h"

Room::Room()
{

}

Room::Room(int _id, int _roomSize, std::string _password)
{
	id = _id;
	maxUsers = _roomSize;
	password = _password;
	
	if ( password.empty() || password == " ")
	{
		hasPassword = false;
	}
}

Room::~Room()
{
	clients.clear();
	delete this;
}

unsigned short Room::GetId() {
	return id;
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
	clients.push_back(_clientData);
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
	std::string message;
	for (int i = 0; i < clients.size(); i++) {
		std::list<ClientData*>::iterator it = clients.begin();
		message = GetMessageProtocolFrom(Message_Protocol::SEND_PLAYERS_IP_PORT) + std::to_string(i) + "_";
		for (int j = 0; j < i; j++) {
			message += (*it)->ipAddress.ip.toString() + "_" + std::to_string((*it)->port) + "_";
			it++;
		}
		sf::Packet pack;
		pack << message;
		(*it)->socket->Send(pack);
	}
}

std::string Room::toString() 
{
	std::string roomInfo = std::to_string(id) + " | "
		+ std::to_string(clients.size()) + " of " + std::to_string(maxUsers);
	return roomInfo;
}
