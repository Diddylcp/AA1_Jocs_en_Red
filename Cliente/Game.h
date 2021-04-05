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
public:
	Game();
	void RequestCard();
	void NextTurn();
	void CheckTurn();
	bool ReceiveCard(std::vector<std::string> parameters);
	void CheckCard(std::vector<std::string> parameters);
	void initDeck();
	void shuffleDeck();
	void dealCards();
	void SetSeed(int _seed);
	int GetSeed();
	void SetTurnPos(int _turnPos);
	int GetTurnPos();
};

