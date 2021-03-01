#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <mutex>
#include <chrono>
#include <SFML/Network.hpp>
#include "PeerClient.h"

bool Cliente(sf::TcpSocket* _socket, std::string _ip, unsigned short _puerto) {

	//Connect a una ip: puerto
	sf::Socket::Status status;
	status = _socket->connect(_ip, _puerto);

	if (status != sf::Socket::Status::Done) {
		std::cout << "Error en el connect....cerrando el programa";
		return false;
	}
	else {
		return true;
	}

}

void SendingMessages(PeerClient* myClients) {
	myClients->SendMessages();
}

int main() {
	bool okConexion;

	sf::TcpSocket* sock = new sf::TcpSocket();
	//Pedimos ip:port y pasar por parametro
	std::cout << "Introduce la ip....." << std::endl;
	std::string ip;
	std::cin >> ip;

	unsigned short puerto = 50000;

	PeerClient myClients;

	okConexion = Cliente(sock, ip, puerto);
	if (okConexion) {
		myClients.RecepcionClient(sock);
		std::thread tSend(SendingMessages, &myClients);
		tSend.detach();
		myClients.RecepcionMessages();
	}
	for (size_t i = 0; i < myClients.clientes.size(); i++)
	{
		myClients.clientes[i]->disconnect();
		delete myClients.clientes[i];
	}
	myClients.clientes.clear();
	return 0;
}

