#include "TcpListener.h"

TcpListener::TcpListener()
{
	listener = new sf::TcpListener();
}

TcpListener::~TcpListener()
{
	delete listener;
}

sf::TcpListener* TcpListener::GetListener()
{
	return listener;
}

Status TcpListener::Listen(Port _port)
{
	return Status(listener->listen(_port.port));
}

Status TcpListener::Accept(TcpSocket* _socket)
{
	return Status(listener->accept(*_socket->GetSocket()));
}

void TcpListener::Disconnect()
{
	listener->close();
}
