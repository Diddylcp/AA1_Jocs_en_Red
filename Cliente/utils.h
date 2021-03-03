#pragma once


struct Port
{
	unsigned short port;

	Port() {}
	Port(unsigned short _port) {
		port = _port;
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

