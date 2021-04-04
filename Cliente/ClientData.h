#pragma once
#include "utils.h"
#include "TcpSocket.h"

class ClientData
{
public:
	IpAddress ipAddress;
	unsigned short port;
	TcpSocket* socket;

	ClientData();
	ClientData(IpAddress _ip, unsigned short _port);
	ClientData(std::string _ip, unsigned short _port);
	
	friend sf::Packet& operator>>(sf::Packet& packet, ClientData& cliente)
	{
		std::string ipStr;
		packet >> ipStr;
		cliente.ipAddress = ipStr;
		packet >> cliente.port;
		return packet;
	}

	friend sf::Packet& operator<<(sf::Packet& packet, ClientData& cliente)
	{
		packet << cliente.ipAddress.ip.toString();
		packet << cliente.port;
		return packet;
	}
	
};

