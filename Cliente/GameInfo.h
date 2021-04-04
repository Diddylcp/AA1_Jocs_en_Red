#pragma once
#include <vector>
#include "GameElements.h"
class GameInfo
{
	int points;
	std::vector<Carta> cartas;
	int turnPos;
	int currTurn;

	GameInfo(int turn) {
		points = 0;
	}
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

	// Comprovar si hay familias
	void FamilyComplete();
	// Pasar turno


	// Comprovar carta pedida


	// 
};

