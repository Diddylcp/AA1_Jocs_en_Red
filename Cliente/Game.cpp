#include "Game.h"

Game::Game()
{
	points = 0;
	currTurn = -1;
}

void Game::RequestCard()
{
	
	int player = -1;
	Carta request;
	while (player > clientes.size() && player < 0 && player != turnPos) {
		std::cout << "Escribe el numero del jugador que quieras robar la carta\n";
		for (int i = 0; i < clientes.size(); i++)
			std::cout << i << " - Jugador " << i << std::endl;
		std::cin >> player;
	}
	while (request.culture == Cultura::COUNT) {
		std::cout << "Que cultura quieres pedirle?\n";
		std::cout 
			<< "1 - ARABE\n"
			<< "2 - BANTU\n"
			<< "3 - CHINA\n"
			<< "4 - ESQUIMAL\n"
			<< "5 - INDIA\n"
			<< "6 - MEXICANA\n"
			<< "7 - TIROLESA\n";
		std::cin >> request.culture;
	}
	while (request.parent == Familiar::COUNT) {
		std::cout << "Que cultura quieres pedirle?\n";
		std::cout
			<< "1 - ABUELO\n"
			<< "2 - ABUELA\n"
			<< "3 - PADRE\n"
			<< "4 - MADRE\n"
			<< "5 - HIJO\n"
			<< "6 - HIJA\n";
		std::cin >> request.parent;
	}
	std::string message = GetMessageProtocolFrom(Message_Protocol::REQUEST_CARD) + std::to_string(turnPos) + "_" + std::to_string(player) + "_" + request.toString();
	sf::Packet pack;
	pack << message;
	for (int i = 0; i < clientes.size(); i++) {
		clientes[i]->Send(pack);
	}

}

void Game::NextTurn()
{
	sf::Packet pack;
	pack << GetMessageProtocolFrom(Message_Protocol::NEXT_TURN);
	for (int i = 0; i < clientes.size(); i++) {
		Status status = clientes[i]->Send(pack);
		if (status == Status::Disconnected || status == Status::Error)
		{
			break;
		}
	}
	currTurn++;
	if (currTurn > clientes.size())
		currTurn = 0;
}

void Game::CheckTurn()
{
	if (currTurn == turnPos) {
		// Need to make a timer
		std::cout << "Es tu turno! Ahora toca elegir una carta de un jugador\n";
		RequestCard();
	}
}

void Game::ReceiveCard(std::vector<std::string> parameters)
{
	if (parameters[1] == "yes") {
		std::cout << "Ha acertado la carta! Vuelve a elegir otra\n";
		if (currTurn == turnPos)
		{
			Carta card(parameters[2], parameters[3]);
			cartas.push_back(card);
			FamilyComplete();
			RequestCard();
		}
	}
	else
	{
		NotifyHasCard(false);
		NextTurn();
	}
}

void Game::CheckCard(std::vector<std::string> parameters)
{
	if (std::stoi(parameters[2]) == turnPos) {
		sf::Packet pack;
		std::string message;
		bool haveCard = false;
		for (int i = 0; i < cartas.size(); i++) {
			if (toString(cartas[i].culture) == parameters[3] && toString(cartas[i].parent) == parameters[4]) {
				message = GetMessageProtocolFrom(Message_Protocol::RESPONSE_REQUEST_CARD) + "yes" + "_" + cartas[i].toString();
				haveCard = true; // TODO: Quitar carta de la mano
				auto it = std::find(cartas.begin(), cartas.end(), cartas[i]);
				cartas.erase(it);
				break;
			}
		}
		if (!haveCard) {
			message = GetMessageProtocolFrom(Message_Protocol::RESPONSE_REQUEST_CARD) + "no";
		}
		pack << message;
		for (int i = 0; i < clientes.size(); i++) {
			clientes[i]->Send(pack);
		}
	}
}

void Game::initDeck()
{
	for (int i = 0; i < (int)Cultura::COUNT; i++)
	{
		for (int j = 0; j < (int)Familiar::COUNT; j++)
		{
			Carta aux((Cultura)i, (Familiar)j);
			baraja.push_back(aux);
		}
	}
	if(baraja.size() != ((int)Cultura::COUNT*(int)Familiar::COUNT)) 
		std::cout << "Unexpected number of cards" << std::endl; 

	shuffleDeck();
	dealCards();
}

void Game::shuffleDeck()
{
	std::srand(seed);
	std::random_shuffle(baraja.begin(), baraja.end(), myRandom);
}

void Game::dealCards()
{
	for (int i = 0; i < baraja.size(); i++)
	{
		if (i % numPlayers == turnPos)
		{
			cartas.push_back(baraja[i]);
		}
	}
	FamilyComplete();
}

void Game::SetSeed(int _seed)
{
	seed = _seed;
}

int Game::GetSeed()
{
	return seed;
}

void Game::SetTurnPos(int _turnPos)
{
	turnPos = _turnPos;
}

int Game::GetTurnPos()
{
	return turnPos;
}

void Game::NotifyTurn()
{
	currTurn++;
	std::cout << "Ha cambiado el turno, le toca al Jugador " + currTurn << std::endl;

}

void Game::NotifyCardRequest(std::vector<std::string> str)
{
	CheckCard(str);
	Carta c(str[3], str[4]);
	std::cout << "Jugador " + str[2] + "--->" + " Jugador " + std::to_string(currTurn) + " Carta: " + toString(c.culture) + " " + toString(c.parent) << std::endl;

}

void Game::NotifyHasCard(bool has)
{
	if (has) std::cout << "Ha conseguido la carta" << std::endl;
	else std::cout << "No ha conseguido la carta" << std::endl;
}

void Game::NotifyFamilyCompleted(std::vector<std::string> str)
{
	std::cout << str[1] << std::endl;
}

void Game::ShowCards()
{
	for (int i = 0; i < cartas.size(); i++)
	{
		std::cout << cartas[i].toString() << "-->" << toString(cartas[i].culture) << " " << toString(cartas[i].parent) << std::endl;
	}
}

void Game::Update()
{
	system("CLS");
	std::cout << "Start Game " << numPlayers << " players\n";
	ShowCards();
	while (true)
	{
		if (currTurn == turnPos)
		{
			FamilyComplete();
			CheckTurn();
			ShowCards();
			// 1 Pide carta CheckTurn()
			// 2 Se comprueba que tenga la carta
			// 3 if(tiene carta) FamilyComplete() y vuelve a pedir carta (1)
			// 4 else NextTurn en el paso 1
		}
	}
}

void Game::FamilyComplete()
{
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
			std::vector<Carta>::iterator it;
			for (it = cartas.begin(); it != cartas.end(); it++)
			{
				if (isArab(*it)) 
				{
					return;
				}
			}
			//std::vector<Carta>::iterator it = std::find_if(begin(cartas), end(cartas), isArab);
			cartas.erase(it);
		}
		points++;
		sf::Packet pack;
		std::string texto = GetMessageProtocolFrom(Message_Protocol::FAMILY_COMPLETE) + "ARAB COMPLETE BY " + std::to_string(currTurn);
		pack << texto;
		for (int i = 0; i < clientes.size(); i++) {
			clientes[i]->Send(pack);
		}
	}
	if (bantuCounter == 6)
	{
		for (int i = 0; i < bantuCounter; i++)
		{
			std::vector<Carta>::iterator it;
			for (it = cartas.begin(); it != cartas.end(); it++)
			{
				if (isBantu(*it))
				{
					return;
				}
			}
			cartas.erase(it);
		}
		points++;
		sf::Packet pack;
		std::string texto = GetMessageProtocolFrom(Message_Protocol::FAMILY_COMPLETE) + "BANTU COMPLETE BY " + std::to_string(currTurn);
		pack << texto;
		for (int i = 0; i < clientes.size(); i++) {
			clientes[i]->Send(pack);
		}
	}
	if (chinaCounter == 6)
	{
		for (int i = 0; i < chinaCounter; i++)
		{
			std::vector<Carta>::iterator it;
			for (it = cartas.begin(); it != cartas.end(); it++)
			{
				if (isChinese(*it))
				{
					return;
				}
			}
			cartas.erase(it);
		}
		points++;
		sf::Packet pack;
		std::string texto = GetMessageProtocolFrom(Message_Protocol::FAMILY_COMPLETE) + "CHINA COMPLETE BY " + std::to_string(currTurn);
		pack << texto;
		for (int i = 0; i < clientes.size(); i++) {
			clientes[i]->Send(pack);
		}
	}
	if (inuitCounter == 6)
	{
		for (int i = 0; i < inuitCounter; i++)
		{
			std::vector<Carta>::iterator it;
			for (it = cartas.begin(); it != cartas.end(); it++)
			{
				if (isEsquimal(*it))
				{
					return;
				}
			}
			cartas.erase(it);
		}
		points++;
		sf::Packet pack;
		std::string texto = GetMessageProtocolFrom(Message_Protocol::FAMILY_COMPLETE) + "ESQUIMAL COMPLETE BY " + std::to_string(currTurn);
		pack << texto;
		for (int i = 0; i < clientes.size(); i++) {
			clientes[i]->Send(pack);
		}
	}
	if (indianCounter == 6)
	{
		for (int i = 0; i < indianCounter; i++)
		{
			std::vector<Carta>::iterator it;
			for (it = cartas.begin(); it != cartas.end(); it++)
			{
				if (isIndian(*it))
				{
					return;
				}
			}
			cartas.erase(it);
		}
		points++;
		sf::Packet pack;
		std::string texto = GetMessageProtocolFrom(Message_Protocol::FAMILY_COMPLETE) + "INDIAN COMPLETE BY " + std::to_string(currTurn);
		pack << texto;
		for (int i = 0; i < clientes.size(); i++) {
			clientes[i]->Send(pack);
		}
	}
	if (mexicanCounter == 6)
	{
		for (int i = 0; i < mexicanCounter; i++)
		{
			std::vector<Carta>::iterator it;
			for (it = cartas.begin(); it != cartas.end(); it++)
			{
				if (isMexican(*it))
				{
					return;
				}
			}
			cartas.erase(it);
		}
		points++;
		sf::Packet pack;
		std::string texto = GetMessageProtocolFrom(Message_Protocol::FAMILY_COMPLETE) + "MEXICAN COMPLETE BY " + std::to_string(currTurn);
		pack << texto;
		for (int i = 0; i < clientes.size(); i++) {
			clientes[i]->Send(pack);
		}
	}
	if (tirolCounter == 6)
	{
		for (int i = 0; i < tirolCounter; i++)
		{
			std::vector<Carta>::iterator it;
			for (it = cartas.begin(); it != cartas.end(); it++)
			{
				if (isTirol(*it))
				{
					return;
				}
			}
			cartas.erase(it);
		}
		points++;
		sf::Packet pack;
		std::string texto = GetMessageProtocolFrom(Message_Protocol::FAMILY_COMPLETE) + "TIROL COMPLETE BY " + std::to_string(currTurn) ;
		pack << texto;
		for (int i = 0; i < clientes.size(); i++) {
			clientes[i]->Send(pack);
		}
	}
}
