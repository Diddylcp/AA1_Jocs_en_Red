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
	friend bool isArab(Carta c)
	{
		return c.culture == Cultura::ARABE;
	}
	friend bool isBantu(Carta c)
	{
		return c.culture == Cultura::BANTU;
	}
	friend bool isChinese(Carta c)
	{
		return c.culture == Cultura::CHINA;
	}
	friend bool isEsquimal(Carta c)
	{
		return c.culture == Cultura::ESQUIMAL;
	}
	friend bool isIndian(Carta c)
	{
		return c.culture == Cultura::INDIA;
	}
	friend bool isMexican(Carta c)
	{
		return c.culture == Cultura::MEXICANA;
	}
	friend bool isTirol(Carta c)
	{
		return c.culture == Cultura::TIROLESA;
	}

	// Comprovar si hay familias
	void FamilyComplete();
	// Pasar turno


	// Comprovar carta pedida


	// 
};

