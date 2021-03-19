#pragma once
#include "TcpListener.h"
class Selector
{
	sf::SocketSelector ss;

public:
	Selector();
	~Selector();

	void Add(TcpSocket* _socket);
	void Add(TcpListener* _listener);
	void Remove(TcpSocket* _socket);
	void Remove(TcpListener* _listener);
	void Clear();
	bool Wait();
	bool IsReady(TcpSocket* _socket);
	bool IsReady(TcpListener* _listener);
};

