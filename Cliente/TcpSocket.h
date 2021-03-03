#pragma once
#include <SFML/Network.hpp>
#include "InputMemoryStream.h"
#include "OutputMemoryStream.h"
#include "utils.h"

class TcpSocket
{
	sf::TcpSocket* tcpSocket;

public:
	TcpSocket();
	TcpSocket(sf::TcpSocket* _tcpSocket);
	~TcpSocket();

	sf::TcpSocket* GetSocket();
	void SetSocket(sf::TcpSocket* _tcpSocket);
	std::string GetRemoteIP();
	Port GetRemotePort();
	Port GetLocalPort();
	Status Connect(std::string _ip, Port _port);
	InputMemoryStream* Recieve();
	Status Send(sf::Packet& _packet);
	Status Send(OutputMemoryStream& _oms);

	void Disconnect();
};

