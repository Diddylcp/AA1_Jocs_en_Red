#include "ClientData.h"

ClientData::ClientData()
{
}

ClientData::ClientData(IpAddress _ip, unsigned short _port) {
	ipAddress = _ip;
	port = _port;
}
ClientData::ClientData(std::string _ip, unsigned short _port) {
	ipAddress = _ip;
	port = _port;
}
