#include "Game.h"

void Game::RequestCard()
{

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
