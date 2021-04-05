#include "Game.h"

Game::Game()
{
	points = 0;
	currTurn = 0;
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
}

void Game::CheckTurn()
{
	if (currTurn == turnPos) {
		// Need to make a timer
		std::cout << "Es tu turno! Ahora toca elegir una carta de un jugador\n";
		RequestCard();
	}
}

bool Game::ReceiveCard(std::vector<std::string> parameters)
{
	if (parameters[1] == "yes") {
		std::cout << "Has acertado la carta! Vuelve a elegir otra\n";
		RequestCard();
	}
	else
		NextTurn();
}

void Game::CheckCard(std::vector<std::string> parameters)
{
	if (std::stoi(parameters[2]) == turnPos) {
		sf::Packet pack;
		std::string message;
		bool haveCard = false;
		for (int i = 0; i < cartas.size(); i++) {
			if (toString(cartas[i].culture) == parameters[3] && toString(cartas[i].parent) == parameters[4]) {
				message = GetMessageProtocolFrom(Message_Protocol::RESPONSE_REQUEST_CARD) + "yes";
				haveCard = true;
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
