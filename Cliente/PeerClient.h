#pragma once
#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <mutex>
#include <chrono>
#include "Selector.h"
#include "ClientData.h"

class PeerClient
{
	//ROOMINFO
public:
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
	void RoomInfo(TcpSocket* socket, std::vector<std::string> message);
	void JoinCreateRecived(TcpSocket* socket);

};

#pragma region Sobrecargado de Operadores



#pragma endregion