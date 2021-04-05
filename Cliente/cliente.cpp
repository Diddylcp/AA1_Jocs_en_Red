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
	myClients.port = sock->GetLocalPort();
	if (okConexion) {
		// TODO: recibir join or create
		while (myClients.loop) {
			myClients.Recieve(sock);
		}
		myClients.loop = true;
		while (myClients.loop) {
			if (myClients.selector.Wait()) {
				for (size_t i = 0; i < myClients.clientes.size(); i++)
				{
					if (myClients.selector.IsReady(myClients.clientes[i])){
						myClients.Recieve(myClients.clientes[i]);
					}
				}
			}
		}
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

