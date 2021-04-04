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
	void FamilyComplete() {
		short arabCounter = 0, bantuCounter = 0, chinaCounter = 0, inuitCounter = 0, indianCounter = 0, mexicanCounter = 0, tirolCounter = 0;
		for (int i = 0; i < cartas.size(); i++)
		{
			switch (cartas[i].culture)
			{
			case Cultura::ARABE:
				arabCounter++;
				break;
			case Cultura::BANTU:
				bantuCounter++;
				break;
			case Cultura::CHINA:
				chinaCounter++;
				break;
			case Cultura::ESQUIMAL:
				inuitCounter++;
				break;
			case Cultura::INDIA:
				indianCounter++;
				break;
			case Cultura::MEXICANA:
				mexicanCounter++;
				break;
			case Cultura::TIROLESA:
				tirolCounter++;
				break;
			default:
				break;
			}
		}
		if (arabCounter == 6)
		{
			for (int i = 0; i < arabCounter; i++)
			{
				std::vector<Carta>::iterator it = std::find_if(cartas.begin(), cartas.end(), isArab);
				cartas.erase(it);
			}
			points++;
		}
		if (bantuCounter == 6)
		{
			for (int i = 0; i < bantuCounter; i++)
			{
				std::vector<Carta>::iterator it = std::find_if(cartas.begin(), cartas.end(), isBantu);
				cartas.erase(it);
			}
			points++;
		}
		if (chinaCounter == 6)
		{
			for (int i = 0; i < chinaCounter; i++)
			{
				std::vector<Carta>::iterator it = std::find_if(cartas.begin(), cartas.end(), isChinese);
				cartas.erase(it);
			}
			points++;
		}
		if (inuitCounter == 6)
		{
			for (int i = 0; i < inuitCounter; i++)
			{
				std::vector<Carta>::iterator it = std::find_if(cartas.begin(), cartas.end(), isEsquimal);
				cartas.erase(it);
			}
			points++;
		}
		if (indianCounter == 6)
		{
			for (int i = 0; i < indianCounter; i++)
			{
				std::vector<Carta>::iterator it = std::find_if(cartas.begin(), cartas.end(), isIndian);
				cartas.erase(it);
			}
			points++;
		}
		if (mexicanCounter == 6)
		{
			for (int i = 0; i < mexicanCounter; i++)
			{
				std::vector<Carta>::iterator it = std::find_if(cartas.begin(), cartas.end(), isMexican);
				cartas.erase(it);
			}
			points++;
		}
		if (tirolCounter == 6)
		{
			for (int i = 0; i < tirolCounter; i++)
			{
				std::vector<Carta>::iterator it = std::find_if(cartas.begin(), cartas.end(), isTirol);
				cartas.erase(it);
			}
			points++;
		}
	}

	// Pasar turno


	// Comprovar carta pedida


	// 
};

