#pragma once
#include <SFML/Network.hpp>

struct Port
{
	unsigned short port;

	Port() {};
	Port(Port& _port) {
		port = _port.port;
	}
	Port(unsigned short _port) {
		port = _port;
	}
};

struct IpAddress {
	sf::IpAddress ip;

	IpAddress() {};
	IpAddress(IpAddress& _ip) {
		ip = _ip.ip;
	}
	IpAddress(std::string _ip) {
		ip = sf::IpAddress(_ip);
	}
};

enum class Status
{
	Done,
	Disconnected,
	Error,
	NotReady,
	Partial
};

enum class Message_Protocol 
{
	PEER_CLIENTS,
	PEER_LOGIN,

};

static Status GetStatus(sf::Socket::Status _status)
{
    Status status;
    switch (_status)
    {
    case sf::Socket::Done:
        status = Status::Done;
        break;
    case sf::Socket::NotReady:
        status = Status::NotReady;
        break;
    case sf::Socket::Partial:
        status = Status::Partial;
        break;
    case sf::Socket::Disconnected:
        status = Status::Disconnected;
        break;
    case sf::Socket::Error:
        status = Status::Error;
        break;
    default:
        break;
    }
    return status;
}

struct DataCliente {
	IpAddress ipAddress;
	unsigned short port;

	DataCliente() {	}
	DataCliente(IpAddress _ip, unsigned short _port) {
		ipAddress = _ip;
		port = _port;
	}

	friend sf::Packet& operator>>(sf::Packet& packet, DataCliente& cliente)
	{
		std::string ipStr;
		packet >> ipStr;
		cliente.ipAddress = ipStr;
		packet >> cliente.port;
		return packet;
	}

	friend sf::Packet& operator<<(sf::Packet& packet, DataCliente& cliente)
	{
		packet << cliente.ipAddress.ip.toString();
		packet << cliente.port;
		return packet;
	}
};