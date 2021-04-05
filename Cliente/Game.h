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
	void NotifyTurn();
	void NotifyCardRequest(std::vector<std::string> str);
	void NotifyHasCard(bool has);
	void NotifyFamilyCompleted(std::vector<std::string> str);
	bool isArab(Carta c)
	{
		return c.culture == Cultura::ARABE;
	}
	bool isBantu(Carta c)
	{
		return c.culture == Cultura::BANTU;
	}
	bool isChinese(Carta c)
	{
		return c.culture == Cultura::CHINA;
	}
	bool isEsquimal(Carta c)
	{
		return c.culture == Cultura::ESQUIMAL;
	}
	bool isIndian(Carta c)
	{
		return c.culture == Cultura::INDIA;
	}
	bool isMexican(Carta c)
	{
		return c.culture == Cultura::MEXICANA;
	}
	bool isTirol(Carta c)
	{
		return c.culture == Cultura::TIROLESA;
	}

	void FamilyComplete();
};

