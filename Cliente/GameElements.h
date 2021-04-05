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

std::string toString(Cultura c) {
	switch (c) {
	case Cultura::ARABE:
		return "Árabe";
		break;
	case Cultura::BANTU:
		return "Bantú";
		break;
	case Cultura::CHINA:
		return "China";
		break;
	case Cultura::ESQUIMAL:
		return "Esquimal";
		break;
	case Cultura::INDIA:
		return "India";
		break;
	case Cultura::MEXICANA:
		return "Mexicana";
		break;
	case Cultura::TIROLESA:
		return "Tirolesa";
	}
}

std::string toString(Familiar f) {
	switch (f)
	{
	case Familiar::ABUELO:
		return "Abuelo";
		break;
	case Familiar::ABUELA:
		return "Abuela";
		break;
	case Familiar::PADRE:
		return "Padre";
		break;
	case Familiar::MADRE:
		return "Madre";
		break;
	case Familiar::HIJO:
		return "Hijo";
		break;
	case Familiar::HIJA:
		return "Hija";
		break;
	default:
		break;
	}
}

struct Carta {

	Cultura culture;
	Familiar parent;

	Carta() {
		culture = Cultura::COUNT;
		parent = Familiar::COUNT;
	}

	Carta(Cultura _c, Familiar _p)
	{
		culture = _c;
		parent = _p;
	}

	Carta(std::string _c, std::string _p)
	{
		Cultura c = static_cast<Cultura>(std::stoi(_c));
		Familiar p = static_cast<Familiar>(std::stoi(_p));
	}

	std::string toString() 
	{

		std::string str;

		int identifier = static_cast<int>(culture);
		int identifier2 = static_cast<int>(parent);
		str = std::to_string(identifier) + "_" + std::to_string(identifier2);

		return str;
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
	pack << static_cast<int>(Message_Protocol::REQUEST_CARD);
	pack << p;
	pack << static_cast<int>(c);
	pack << static_cast<int>(f);
}


std::istream& operator>> (std::istream& in, Cultura& cult) {
	int val;
	if (in >> val) {
		val--;
		switch (val) {
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			cult = Cultura(val);
			break;
		default:
			cult = Cultura::COUNT;
			break;
		}
	}
}

std::istream& operator>> (std::istream& in, Familiar& fam) {
	int val;
	if (in >> val) {
		val--;
		switch (val) {
		case 0:
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
			fam = Familiar(val);
			break;
		default:
			fam = Familiar::COUNT;
			break;
		}
	}
}