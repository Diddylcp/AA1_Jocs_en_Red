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
	Port port = 0;
	bool loop = true;
	TcpListener listener;
	Game myGame;
	bool inGame = false;
	PeerClient() {};

	void RecepcionClient(TcpSocket* sock, std::vector<std::string> message);
	void RecepcionMessages();
	void SendMessages();

	void PeerReceive(TcpSocket* socket);

	void GetGames(TcpSocket* socket);
	void ShowGamesInfo(TcpSocket* socket, std::vector<std::string> message);
	void CreateGame(TcpSocket* socket);
	void JoinGame(TcpSocket* socket, std::vector<std::string> message);
	void JoinCreateRecived(TcpSocket* socket);
	void StartGame();

};

#pragma region Sobrecargado de Operadores



#pragma endregion