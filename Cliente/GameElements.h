#include <string>
#include "utils.h"
#pragma once

enum class Cultura
{
	ARABE,
	BANTU,
	CHINA,
	ESQUIMAL,
	INDIA,
	MEXICANA,
	TIROLESA,
	COUNT
};

enum class Familiar
{
	ABUELO,
	ABUELA,
	PADRE,
	MADRE,
	HIJO,
	HIJA,
	COUNT
};

struct Carta {

	Cultura culture;
	Familiar parent;

	Carta(Cultura _c, Familiar _p)
	{
		culture = _c;
		parent = _p;
	}

	std::string toString() 
	{
		std::string cString;
		std::string pString;
		std::string fString;

		switch (culture)
		{
		case Cultura::ARABE:
			cString = "Árabe";
			break;
		case Cultura::BANTU:
			cString = "Bantú";
			break;
		case Cultura::CHINA:
			cString = "China";
			break;
		case Cultura::ESQUIMAL:
			cString = "Esquimal";
			break;
		case Cultura::INDIA:
			cString = "India";
			break;
		case Cultura::MEXICANA:
			cString = "Mexicana";
			break;
		case Cultura::TIROLESA:
			cString = "Tirolesa";
			break;
		default:
			break;
		}

		switch (parent)
		{
		case Familiar::ABUELO:
			pString = "Abuelo";
			break;
		case Familiar::ABUELA:
			pString = "Abuela";
			break;
		case Familiar::PADRE:
			pString = "Padre";
			break;
		case Familiar::MADRE:
			pString = "Madre";
			break;
		case Familiar::HIJO:
			pString = "Hijo";
			break;
		case Familiar::HIJA:
			pString = "Hija";
			break;
		default:
			break;
		}

		fString = pString + " " + cString;

		return fString;
	}

	bool operator== (Carta c) {
		if (c.culture == culture && c.parent == parent) return true;
		else return false;
	}
};


void SendRequestCard(Cultura c, Familiar f, int p)
{
	// Pide la carta que tiene la cultura c y el familiar f al jugador p
	sf::Packet pack;
	pack << static_cast<int>(Message_Protocol::PEDIR_CARTA);
	pack << p;
	pack << static_cast<int>(c);
	pack << static_cast<int>(f);
}
