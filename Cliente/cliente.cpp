#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <mutex>
#include <chrono>
#include "PeerClient.h"

bool Cliente(TcpSocket* _socket, std::string _ip, unsigned short _puerto) {

	//Connect a una ip: puerto
	Status status;
	status = _socket->Connect(_ip, _puerto);

	if (status != Status::Done) {
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

	TcpSocket* sock = new TcpSocket();
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
		myClients.clientes[i]->Disconnect();
		delete myClients.clientes[i];
	}
	myClients.clientes.clear();
	system("pause");
	return 0;
}

