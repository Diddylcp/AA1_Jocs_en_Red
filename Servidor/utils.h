#pragma once
#include <SFML/Network.hpp>

#define SEPARATOR_MESSAGE_PROTOCOL "_"
#define SEPARATOR_MESSAGE_THINGS ":"
#define ERROR_LISTENER "Error en el listener....cerrando el programa"

typedef unsigned short Port;

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
	S_JOIN_OR_CREATE,
	C_JOIN_OR_CREATE,
	GAMES_INFO,
	GET_GAMES_INFO,
	S_ROOM_INFO,
	SEND_PLAYERS_IP_PORT,
	GAMES_FILTRE_SEND,
	JOIN_GAME,

};

static std::string GetMessageProtocolFrom(Message_Protocol index)
{
    std::string str;

    int identifier = static_cast<int>(index);
    str = std::to_string(identifier) + "_";

    return str;
}

static Message_Protocol GetMessageProtocol(std::string index)
{
	return static_cast<Message_Protocol>(std::stoi(index));
}

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