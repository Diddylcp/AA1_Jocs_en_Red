#pragma once
#include <SFML/Network.hpp>
#include <cstdio>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <iostream>

#define SEPARATOR_MESSAGE_PROTOCOL "_"
#define SEPARATOR_MESSAGE_THINGS ":"
#define JOIN_OR_CREATE "Do you want to join an existing game or create a new one? (0 join, 1 create)"
#define SELECT_ROOM "Wich room do you want to join? (numbers)"
#define INTRODUCE_IP "Introduce la ip....."
#define ERROR_LISTENER "Error en el listener....cerrando el programa"
#define CONNECTED_CLIENTS(x) "Tenemos %d clientes conectados", x



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
    NEXT_TURN,
    START_GAME,
    REQUEST_CARD,
    RESPONSE_REQUEST_CARD,
    FAMILY_COMPLETE
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

static std::vector<std::string> split(const std::string& s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
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

int myRandom(int _seed)
{
    return std::rand() % _seed;
}