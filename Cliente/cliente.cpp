#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <mutex>
#include <chrono>
#include "PeerClient.h"

struct RoomsInfo {
	int idRoom;
	int maxClients;
	int countClients;
	bool hasPassword;

	RoomsInfo();
	RoomsInfo(std::string _idRoom, std::string _maxClients, std::string _countClients, std::string _hasPassword) {
		idRoom = std::stoi(_idRoom);
		maxClients = std::stoi(_maxClients);
		countClients = std::stoi(_countClients);
		hasPassword = (_hasPassword == "0"? true : false);
	}

	std::string to_string() {
		std::string s = std::to_string(idRoom) +" | ("+ std::to_string(countClients) + " of " + std::to_string(maxClients) + ") " + (hasPassword ? "CLOSED" : "OPEN");
		return s;
	}
};

bool Cliente(TcpSocket* _socket, std::string _ip, unsigned short _puerto)
{
	return _socket->Connect(_ip, _puerto) == Status::Done;
}

void SendingMessages(PeerClient* myClients) 
{
	myClients->SendMessages();
}

int main() {
	bool okConexion;

	TcpSocket* sock = new TcpSocket();
	//Pedimos ip:port y pasar por parametro
	std::cout << INTRODUCE_IP << std::endl;
	std::string ip;
	std::cin >> ip;

	unsigned short puerto = 50000;

	PeerClient myClients;

	okConexion = Cliente(sock, ip, puerto);


	
	if (okConexion) {
		// TODO: recibir join or create
		myClients.Recieve(sock);

		myClients.RecepcionClient(sock);
		std::thread tSend(SendingMessages, &myClients);
		tSend.detach();
		myClients.RecepcionMessages();
	}
	else
	{
		std::cout << "Error en el connect....cerrando el programa";
	}

	for (size_t i = 0; i < myClients.clientes.size(); i++)
	{
		myClients.clientes[i]->Disconnect();
		delete myClients.clientes[i];
	}
	myClients.clientes.clear();
	system("pause");
	return 0;
}

