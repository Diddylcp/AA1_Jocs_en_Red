#include "Game.h"

Game::Game()
{
	points = 0;
	currTurn = 0;
}

void Game::RequestCard()
{
	std::cout << "Es tu turno! Ahora toca elegir una carta de un jugador\n";
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
	sf::Packet pack;
	pack << 14 << turnPos << player << static_cast<int>(request.culture) << static_cast<int>(request.parent);
	clientes[player]->Send(pack);

}

void Game::NextTurn()
{
	sf::Packet pack;
	pack << static_cast<int>(Message_Protocol::NEXT_TURN);
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
		RequestCard();
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
