#include "ClientData.h"

ClientData::ClientData()
{
}

ClientData::ClientData(TcpSocket* _socket)
{
	socket = _socket;
	ipAddress = socket->GetRemoteIP();
	port = socket->GetRemotePort();
}

ClientData::ClientData(IpAddress _ip, unsigned short _port) {
	ipAddress = _ip;
	port = _port;
}
ClientData::ClientData(std::string _ip, unsigned short _port) {
	ipAddress = _ip;
	port = _port;
}
