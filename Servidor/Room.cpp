#include "Room.h"

Room::Room()
{
}

Room::Room(std::string _password)
{
	password = _password;
}

Room::~Room()
{
	clientes.clear();
	delete this;
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
	clientes.push_back(_clientData);
}

void Room::DisconnectUserFromRoom(ClientData* _clientData)
{
	clientes.remove(_clientData);
}

void Room::StartGame()
{
	// TODO: Enviar a todos los datos a los clientes antes de empezar la partida
	// (de forma escalada, el sigiente cliente recibe los datos de sus anteriores)
	//
	// eviar mensaje de "ecuchar connect de otros clientes"
	// al finalizar limpiar / eliminar sala
}
