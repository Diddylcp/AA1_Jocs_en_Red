#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <mutex>
#include <chrono>
#include <string>
#include "Selector.h"
#include "Room.h"



class servidor {
	bool running = true;
	Status status;
	TcpListener listener;
	std::vector<TcpSocket*> clientes;
	std::vector<ClientData*> clientesData;
	std::vector<Room*> rooms;

public:

	servidor (){	
		//Accept para esperar socket nuevo
		status = listener.Listen(50000);
		rooms.resize(5);

		if (status != Status::Done) 
		{
			std::cout << "Error en el listener....cerrando el programa";
			running = false;
		}
		else 
		{
			sf::IpAddress ip = sf::IpAddress::LocalHost;
			std::cout << ip.getLocalAddress() << ":" << std::to_string(50000) << std::endl;
		}
	}

	bool ManageRooms() 
	{
		while (running) 
		{
			TcpSocket* newClient = new TcpSocket;
			status = listener.Accept(newClient);

			if (status != Status::Done) {
				std::cout << "Error en el listener....cerrando el programa";
				return false;
			}
			else 
			{
				// Cliente conectado
				SendJoinOrCreateGame(newClient);

				sf::Packet p;
				newClient->Receive(p);

				//crear partida o unirse

				//si unirse, mostrar todas las partidas

				//si crear preguntar si quiere contraseña

				//
				std::cout << "Tenemos " << clientes.size() << " clientes conectados\n";
				for (size_t i = 0; i < clientes.size(); i++)
				{
					sf::Packet pack;
					pack << *clientesData[i];

					status = newClient->Send(pack);

					ClientData dataAux;
					pack >> dataAux;

					if (status == Status::Disconnected)
					{
						break;
					}
				}
				sf::Packet packet;
				ClientData aux;
				aux.ipAddress = sf::IpAddress::LocalHost.toString();
				aux.port = newClient->GetRemotePort();
				packet << aux;

				status = newClient->Send(packet);
				clientes.push_back(newClient);
				clientesData.push_back(new ClientData(newClient->GetRemoteIP(), newClient->GetRemotePort()));
				std::cout << newClient->GetRemoteIP() << ":" << newClient->GetRemotePort() << "  " << aux.ipAddress.ip;
				newClient->Disconnect();

			}

			if (clientes.size() == 4) 
			{
				std::cout << "Se han conectado 4 personas\n";
				running = false;
				listener.Disconnect();
			}
		}
		for (size_t i = 0; i < clientes.size(); i++)
		{
			clientes[i]->Disconnect();
			delete clientes[i];
		}
		clientes.clear();
		return true;
	}

	void SendJoinOrCreateGame(TcpSocket *socket) 
	{
		sf::Packet pack;
		pack << 0;
		socket->Send(pack);
	}

	void RecieveJoinOrCreateGame(TcpSocket* socket, std::string message)
	{
		std::string parameter = message.substr(1, message.find("_"));

		if (parameter == "0") // Send different rooms to join
		{
			//SendRooms();
		}
		else if (parameter == "1") // create game room
		{
			int roomSize = std::stoi( message.substr(2, message.find("_")) );
			parameter = message.substr(3, message.find("_"));
			

			for (int i = 0; i < rooms.size(); i++)
			{
				ClientData* c = new ClientData(socket->GetRemoteIP(), socket->GetRemotePort());
				if (rooms[i] == nullptr)
				{
					rooms[i] = new Room(roomSize, parameter);
					rooms[i]->AddUserToRoom(c);
				}
			}
		}
	}

	void CreateRoom(TcpSocket* socket) 
	{
		
	}

	void JoinRoom(TcpSocket* socket) 
	{

	}

	void SendRoomInfo(Room* room) {
		for (std::list<ClientData*>::iterator it = room->clients.begin(); it != room->clients.end(); ++it) {
			//std::cout << it;
			
		}
	}

	void SendRooms(TcpSocket* socket)
	{
		// SEND ALL ROOMS TO A CLIENT

	}

	void Recieve(TcpSocket * socket) 
	{
		sf::Packet p;
		Status s = socket->Receive(p);
		if (s == Status::Disconnected) {
			//error
		}
		else 
		{
			std::string s;
			p >> s;
			Message_Protocol mp =  GetMessageProtocol(s.substr(0, s.find("_")));
			
			switch (mp)
			{
			case Message_Protocol::S_JOIN_OR_CREATE:
				RecieveJoinOrCreateGame(socket, s);
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

		//std:string mesaje << ;
		
	}

};

bool Servidor() {
	bool running = true;
	Status status;
	TcpListener listener;
	std::vector<TcpSocket*> clientes;
	std::vector<ClientData*> clientesData;
	std::vector<Room> rooms (5);

	//Accept para esperar socket nuevo
	status = listener.Listen(50000);

	if (status != Status::Done) {
		std::cout << "Error en el listener....cerrando el programa";
		return false;
	}
	else {
		//TODO: limpiar esto
		sf::IpAddress ip = sf::IpAddress::LocalHost;
		std::cout << ip.getLocalAddress() << ":" << std::to_string(50000) << std::endl;

		while (running) {
			TcpSocket* newClient = new TcpSocket;
			status = listener.Accept(newClient);

			if (status != Status::Done) {
				std::cout << "Error en el listener....cerrando el programa";
				return false;
			}
			else {
				std::cout << "Tenemos " << clientes.size() << " clientes conectados\n";
				for (size_t i = 0; i < clientes.size(); i++)
				{
					sf::Packet pack;
					pack << *clientesData[i];

					status = newClient->Send(pack);

					ClientData dataAux;
					pack >> dataAux;

					if (status == Status::Disconnected)
					{
						break;
					}
				}
				sf::Packet packet;
				ClientData aux;
				aux.ipAddress = sf::IpAddress::LocalHost.toString();
				aux.port = newClient->GetRemotePort();
				packet << aux;

				status = newClient->Send(packet);
				clientes.push_back(newClient);
				clientesData.push_back(new ClientData(newClient->GetRemoteIP(), newClient->GetRemotePort()));
				std::cout << newClient->GetRemoteIP() << ":" << newClient->GetRemotePort() << "  " << aux.ipAddress.ip;
				newClient->Disconnect();

			}
			
			if (clientes.size() == 4) {
				std::cout << "Se han conectado 4 personas\n";
				running = false;
				listener.Disconnect();
			}
		}
		for (size_t i = 0; i < clientes.size(); i++)
		{
			clientes[i]->Disconnect();
			delete clientes[i];
		}
		clientes.clear();
		return true;

	}
}



int main() {

	// inicializamos el servidor
	servidor s;

	// le mandamos al servidor gestionar el emparejamiento de partidas
	s.ManageRooms();
	
	return 0;
}

