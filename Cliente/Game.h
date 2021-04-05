#pragma once
#include <vector>
#include "TcpSocket.h"
#include "utils.h"
#include "GameElements.h"
class Game
{
	int points;
	std::vector<Carta> cartas;
	int turnPos;
	int currTurn;
	std::vector<TcpSocket*> clientes;
	void RequestCard();
	void NextTurn();
	void CheckTurn();
	bool ReceiveCard(std::vector<std::string> parameters);
	void CheckCard(std::vector<std::string> parameters);
};

