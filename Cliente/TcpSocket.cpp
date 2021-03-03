#include "TcpSocket.h"


TcpSocket::TcpSocket()
{

}

TcpSocket::TcpSocket(sf::TcpSocket* _tcpSocket)
{
    tcpSocket = _tcpSocket;
}

TcpSocket::~TcpSocket()
{
    delete (tcpSocket);
}


Status TcpSocket::Connect(std::string _ip, Port _port)
{
    sf::Socket::Status sockStatus = tcpSocket->connect(_ip, _port.port);
    Status status;

    switch (sockStatus)
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

void TcpSocket::Disconnect()
{

}

sf::TcpSocket* TcpSocket::GetSocket()
{
    return nullptr;
}

void TcpSocket::SetSocket(sf::TcpSocket* _tcpSocket)
{
}

std::string TcpSocket::GetRemoteIP()
{
    return std::string();
}

Port TcpSocket::GetRemotePort()
{
    return Port();
}

Port TcpSocket::GetLocalPort()
{
    return Port();
}

InputMemoryStream* TcpSocket::Recieve()
{
    return nullptr;
}

Status TcpSocket::Send(OutputMemoryStream& _oms)
{
    sf::Socket::Status sockStatus;// = tcpSocket->send();
    return GetStatus(sockStatus);
}

Status TcpSocket::Send(sf::Packet& _packet)
{
    sf::Socket::Status sockStatus = tcpSocket->send(_packet);
    return GetStatus(sockStatus);
}

Status GetStatus(sf::Socket::Status _status) 
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
}
