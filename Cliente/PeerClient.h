#pragma once
#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <mutex>
#include <chrono>
#include "Game.h"

class PeerClient
{
	//ROOMINFO
public:
	struct RoomsInfo;

	std::vector<TcpSocket*> clientes;
	Port port = 0;
	bool loop = true;
	TcpListener listener;
	Game myGame;

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