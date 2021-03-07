#pragma once
#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <mutex>
#include <chrono>
#include "Selector.h"

class PeerClient
{
public:
	std::vector<TcpSocket*> clientes;
	Port port = 0;
	bool loop = true;
	TcpListener listener;

	PeerClient() {};

	void RecepcionClient(TcpSocket* sock);
	void RecepcionMessages();
	void SendMessages();
};

#pragma region Sobrecargado de Operadores



#pragma endregion