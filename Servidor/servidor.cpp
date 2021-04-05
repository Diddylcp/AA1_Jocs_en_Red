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
	sf::IpAddress ip;
	Status status;
	Selector selector;
	TcpListener *listener;
	std::vector<TcpSocket*> clientes;
	std::vector<TcpSocket*> notJoined;
	//std::vector<ClientData*> clientesData;
	std::vector<Room*> rooms;

public:

	servidor (){	
		//Accept para esperar socket nuevo
		listener = new TcpListener();
		status = listener->Listen(50000);
		rooms.resize(5);
		selector.Add(listener);

		if (status != Status::Done) 
		{
			std::cout << ERROR_LISTENER;
			running = false;
		}
		else 
		{
			ip = sf::IpAddress::LocalHost;
		}

	}

	void PrintServerInfo() {
		std::cout << ip.getLocalAddress() << ":" << std::to_string(50000) << std::endl;
	}

	void PrintRooms() {
		for (int i = 0; i < rooms.size(); i++) {
			if (rooms[i] != nullptr) {
				std::cout << rooms[i]->toString() << std::endl; 
				for (std::list<ClientData*>::iterator it = rooms[i]->clients.begin(); it != rooms[i]->clients.end(); ++it)
				{
					std::cout << '\t' + (*it)->toString() << std::endl;
				}
			}
			else 
			{
				std::cout << "---" << std::endl;
			}
		}
	}

	bool ManageRooms() 
	{
		while (running) 
		{
			//system("clear");

			PrintServerInfo();

			PrintRooms();

			if (selector.Wait())
			{
				// Test the listener
				if (selector.IsReady(listener))
				{
					TcpSocket* newClient = new TcpSocket;

					if (listener->Accept(newClient) != Status::Done) {
						std::cout << ERROR_LISTENER;
						return false;
						delete newClient;
					}
					else
					{
						// Cliente conectado
						SendJoinOrCreateGame(newClient);

						sf::Packet p;

						Receive(newClient);

						std::cout << "Tenemos " << clientes.size() << " clientes conectados\n";
					}
				}
				else 
				{

				}
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
		std::string s = GetMessageProtocolFrom(Message_Protocol::S_JOIN_OR_CREATE);
		pack << s;
		socket->Send(pack);
	}

	Room* CreateRoom(const int size, const std::string password) 
	{
		int unusedRooms = 0;
		Room* firstEmptyRoom = nullptr;

		for (int i = 0; i < rooms.size(); i++)
		{
			if (rooms[i] == nullptr)
			{
				if (firstEmptyRoom == nullptr)
				{
					rooms[i] = new Room(i, size, password);
					firstEmptyRoom = rooms[i];
				}
				unusedRooms++;
			}
		}

		if (unusedRooms < rooms.size() / 2) {
			rooms.resize(rooms.size() * 2);
		}

		return firstEmptyRoom;
	}

	void ReceiveJoinOrCreateGame(TcpSocket* socket, std::vector<std::string> message)
	{
		std::string parameter = message[1];

		if (parameter == "0") // Send different rooms to join
		{
			bool hasAvailableRooms = false;
			for (int i = 0; i < rooms.size(); i++) 
			{
				if (rooms[i] != nullptr) 
				{
					hasAvailableRooms = true;
					break;
				}
			}

			if (hasAvailableRooms) {
				SendRooms(socket);
				clientes.push_back(socket);
			}
			else
			{
				// create and add player to default Room
				ClientData* c = new ClientData(socket);
				Room* newRoom = CreateRoom(6, " ");
				newRoom->AddUserToRoom(c);
				clientes.push_back(socket);
			}
		}
		else if (parameter == "1") // create game room
		{
			int roomSize = std::stoi( message[2]);
			parameter = message[3];

			ClientData* c = new ClientData(socket);
			Room* newRoom = CreateRoom(roomSize, parameter);
			newRoom->AddUserToRoom(c);
			clientes.push_back(socket);
		}
	}

	void JoinRoom(TcpSocket* socket, std::string message) 
	{
		//Receive(socket);


	}

	void SendRoomInfo(Room* room) {

		if (room->clients.size() == room->GetMaxUsers()) 
		{


			
		}
		else 
		{
			std::string message = GetMessageProtocolFrom(Message_Protocol::S_ROOM_INFO);
			message += room->clients.size();

			sf::Packet p;
			p << message;
			for (std::list<ClientData*>::iterator it = room->clients.begin(); it != room->clients.end(); ++it)
			{
				//(*it)->socket->Send(p);
			}
		}
	}

	void SendRooms(TcpSocket* socket)
	{

	}


	void SendClientsInfo(Room* room)
	{
		for (std::list<ClientData*>::iterator it = room->clients.begin(); it != room->clients.end(); ++it)
		{
			sf::Packet pack;
			pack << (*it)->toString();

			status = (*it)->socket->Send(pack);

			ClientData dataAux;
			pack >> dataAux;

			if (status == Status::Disconnected)
			{
				break;
			}
		}
	}

	void Receive(TcpSocket * socket) 
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

			std::cout << "Message Received: " << s << std::endl;
			
			std::vector<std::string> parameters = split(s, '_');
			Message_Protocol mp =  GetMessageProtocol(parameters[0]);
			
			switch (mp)
			{
			case Message_Protocol::S_JOIN_OR_CREATE:

				break;
			case Message_Protocol::C_JOIN_OR_CREATE:
				ReceiveJoinOrCreateGame(socket, parameters);
				break;
			case Message_Protocol::GAMES_INFO:
				break;
			case Message_Protocol::GET_GAMES_INFO:

				break;
			case Message_Protocol::SEND_PLAYERS_IP_PORT:

				break;
			case Message_Protocol::GAMES_FILTRE_SEND:

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

/*
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
*/

int main() {

	// inicializamos el servidor
	servidor s;

	// le mandamos al servidor gestionar el emparejamiento de partidas
	s.ManageRooms();
	
	return 0;
}

