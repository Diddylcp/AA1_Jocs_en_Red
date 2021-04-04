#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <mutex>
#include <chrono>
#include "PeerClient.h"

struct RoomsInfo {
	int maxClients;
	int countClients;
	bool hasPassword;
};

bool Cliente(TcpSocket* _socket, std::string _ip, unsigned short _puerto) {

	//Connect a una ip: puerto
	Status status;
	status = _socket->Connect(_ip, _puerto);

	if (status != Status::Done) 
	{
		std::cout << "Error en el connect....cerrando el programa";
		return false;
	}
	else 
	{

		return true;
	}

}

void JoinCreateRecived(TcpSocket* socket) {
	std::cout << JOIN_OR_CREATE;
	int option;
	do
	{
		std::cin >> option;
	} while (option != 0 && option != 1);

	if (option == 0) {
		JoinGame(socket);
	}
	else {
		CreateGame(socket);
	}
}
void CreateGame(TcpSocket *socket)
{
	std::cout << "How many players? (3-6)";
	int roomSize;
	std::cin >> roomSize;
	
	if (roomSize < 3)roomSize = 3;
	else if (roomSize > 6) roomSize = 6;

	std::cout << "Write your password? ";
	sf::Packet p;
	socket->Send(p);


}

void JoinGame(TcpSocket* socket) 
{
	
}




void Recieve(TcpSocket *socket) {
	sf::Packet p;
	socket->Receive(p);

	std::string s;
	p >> s;


	Message_Protocol mp = GetMessageProtocol(s.substr(0, s.find("_")));
	switch (mp)
	{
		case Message_Protocol::S_JOIN_OR_CREATE:
			JoinCreateRecived(socket);
			break;
		case Message_Protocol::C_JOIN_OR_CREATE:

			break;
		case Message_Protocol::GAMES_INFO:
			
			break;
		case Message_Protocol::GET_GAMES_INFO:
			
			break;
		case Message_Protocol::SEND_PLAYERS_IP_PORT:
			
			break;
		case Message_Protocol::GAMES_FILTRE_SEND:
			
			break;
		case Message_Protocol::CREATE_GAME:
			
			break;
		case Message_Protocol::JOIN_GAME:
			
			break;
		default:
			break;
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
		// TODO: recibir join or create
		Recieve(sock);

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

