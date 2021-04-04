#pragma once
#include <vector>
#include "TcpSocket.h"
#include "utils.h"
class Game
{
	std::vector<TcpSocket*> clientes;
	void RequestCard();
	void NextTurn();
};

