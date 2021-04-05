#pragma once
#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <mutex>
#include <chrono>
#include "Selector.h"
#include "ClientData.h"
#include "Game.h"

class PeerClient
{
	//ROOMINFO
public:
	/*
	struct RoomsInfo {
		int idRoom;
		int maxClients;
		int countClients;
		bool hasPassword;

		RoomsInfo();
		RoomsInfo(std::string _idRoom, std::string _maxClients, std::string _countClients, std::string _hasPassword) {
			idRoom = std::stoi(_idRoom);
			maxClients = std::stoi(_maxClients);
			countClients = std::stoi(_countClients);
			hasPassword = (_hasPassword == "0" ? true : false);
		}

		std::string toString() {
			std::string s = std::to_string(idRoom) + " | (" + std::to_string(countClients) + " of " + std::to_string(maxClients) + ") " + (hasPassword ? "CLOSED" : "OPEN");
			return s;
		}
	};
	*/

	std::vector<TcpSocket*> clientes;
	Port port = 0;
	bool loop = true;
	TcpListener listener;

	PeerClient() {};

	void RecepcionClient(TcpSocket* sock);
	void RecepcionMessages();
	void SendMessages();

	void Recieve(TcpSocket* socket);

	void ShowGamesInfo(TcpSocket* socket, std::vector<std::string> message);
	void CreateGame(TcpSocket* socket);
	void JoinGame(TcpSocket* socket);
	void JoinCreateRecived(TcpSocket* socket);
	void StartGame();

};

#pragma region Sobrecargado de Operadores



#pragma endregion