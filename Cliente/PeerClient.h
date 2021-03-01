#pragma once
#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <mutex>
#include <chrono>
#include <SFML/Network.hpp>
class PeerClient
{
public:
	std::vector<sf::TcpSocket*> clientes;
	unsigned short myPort = 0;
	bool loop = true;
	sf::TcpListener listener;

	PeerClient() {};

	struct DataCliente {


		DataCliente() {	}
		DataCliente(sf::IpAddress _ip, unsigned short _port) {
			ip = _ip;
			port = _port;
		}
		sf::IpAddress ip;
		unsigned short port;

		friend sf::Packet& operator>>(sf::Packet& packet, DataCliente& cliente)
		{
			std::string ipStr;
			packet >> ipStr;
			cliente.ip = sf::IpAddress(ipStr);
			packet >> cliente.port;
			return packet;
		}

		friend sf::Packet& operator<<(sf::Packet& packet, DataCliente& cliente)
		{
			packet << cliente.ip.toString();
			packet << cliente.port;
			return packet;
		}
	};

	void RecepcionClient(sf::TcpSocket* sock);
	void RecepcionMessages();
	void SendMessages();
};

#pragma region Sobrecargado de Operadores



#pragma endregion