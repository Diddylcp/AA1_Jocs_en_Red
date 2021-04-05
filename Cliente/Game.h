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
	int seed;
	std::vector<Carta> baraja;
	int numPlayers;
	void RequestCard();
	void NextTurn();
	void CheckTurn();
	void shuffleDeck();
	void reparteCartas();
};

