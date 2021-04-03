#include <string>
#pragma once

struct Carta {

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
		case Carta::Cultura::ARABE:
			cString = "Árabe";
			break;
		case Carta::Cultura::BANTU:
			cString = "Bantú";
			break;
		case Carta::Cultura::CHINA:
			cString = "China";
			break;
		case Carta::Cultura::ESQUIMAL:
			cString = "Esquimal";
			break;
		case Carta::Cultura::INDIA:
			cString = "India";
			break;
		case Carta::Cultura::MEXICANA:
			cString = "Mexicana";
			break;
		case Carta::Cultura::TIROLESA:
			cString = "Tirolesa";
			break;
		default:
			break;
		}

		switch (parent)
		{
		case Carta::Familiar::ABUELO:
			pString = "Abuelo";
			break;
		case Carta::Familiar::ABUELA:
			pString = "Abuela";
			break;
		case Carta::Familiar::PADRE:
			pString = "Padre";
			break;
		case Carta::Familiar::MADRE:
			pString = "Madre";
			break;
		case Carta::Familiar::HIJO:
			pString = "Hijo";
			break;
		case Carta::Familiar::HIJA:
			pString = "Hija";
			break;
		default:
			break;
		}

		fString = pString + " " + cString;

		return fString;
	}
};