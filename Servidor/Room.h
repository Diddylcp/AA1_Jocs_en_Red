#pragma once
#include <string>
#include <list>
#include "ClientData.h"

class Room
{
	unsigned short id;
	bool hasPassword;
	std::string password;
	unsigned short maxUsers;
	float creationTime;


public:

	std::list<ClientData*> clients;
	Room();
	Room(int _roomSize, std::string _password);
	//Room(std::string _password);
	~Room();

	bool HasPassword();
	bool IsPasswordOk(std::string _password);
	void AddUserToRoom(ClientData* _clientData);
	void DisconnectUserFromRoom(ClientData* _clientData);
	void StartGame();
};

