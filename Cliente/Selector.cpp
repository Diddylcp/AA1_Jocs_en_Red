#include "Selector.h"
#include <iostream>

Selector::Selector()
{
}

Selector::~Selector()
{
	ss.clear();
}

void Selector::Add(TcpSocket* _socket)
{
	ss.add(*_socket->GetSocket());
	//std::cout << _socket->GetRemoteIP() << ":" << _socket->GetRemotePort().port << std::endl;
}

void Selector::Add(TcpListener* _listener)
{
	ss.add(*_listener->GetListener());
}

void Selector::Remove(TcpSocket* _socket)
{
	_socket->Disconnect();
	ss.remove(*_socket->GetSocket());
	delete _socket;
}

void Selector::Remove(TcpListener* _listener)
{
	ss.remove(*_listener->GetListener());
	delete _listener;
}

void Selector::Clear()
{
	ss.clear();
}

bool Selector::Wait()
{
	return ss.wait();
}

bool Selector::IsReady(TcpSocket* _socket)
{
	return ss.isReady(*_socket->GetSocket());
}

bool Selector::IsReady(TcpListener* _listener)
{
	return ss.isReady(*_listener->GetListener());
}
