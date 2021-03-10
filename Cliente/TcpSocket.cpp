#include "TcpSocket.h"

TcpSocket::TcpSocket()
{
    tcpSocket = new sf::TcpSocket();
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
    return GetStatus(sockStatus);
}

Status TcpSocket::Receive(sf::Packet& _pack)
{
    return GetStatus(tcpSocket->receive(_pack));
}

Status TcpSocket::Receive(InputMemoryStream* _ims)
{
    char* data = new char;
    std::size_t size = 0;
    std::size_t received = 0;

    Status s = GetStatus(tcpSocket->receive(data, size, received));
    _ims->Read(data, size);
    return s;
}

void TcpSocket::Disconnect()
{
    tcpSocket->disconnect();
}

sf::TcpSocket* TcpSocket::GetSocket()
{
    return tcpSocket;
}

void TcpSocket::SetSocket(sf::TcpSocket* _tcpSocket)
{
    tcpSocket = _tcpSocket;
}

std::string TcpSocket::GetRemoteIP()
{
    return tcpSocket->getRemoteAddress().toString();
}

Port TcpSocket::GetRemotePort()
{
    return tcpSocket->getRemotePort();
}

Port TcpSocket::GetLocalPort()
{
    return tcpSocket->getLocalPort();
}

Status TcpSocket::Send(OutputMemoryStream& _oms)
{
    sf::Socket::Status sockStatus = tcpSocket->send(_oms.GetBufferPtr(), _oms.GetLength());
    return GetStatus(sockStatus);
}

Status TcpSocket::Send(sf::Packet& _packet)
{
    sf::Socket::Status sockStatus = tcpSocket->send(_packet);
    return GetStatus(sockStatus);
}