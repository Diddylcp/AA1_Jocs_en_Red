#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <mutex>
#include <chrono>
#include "PeerClient.h"

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

