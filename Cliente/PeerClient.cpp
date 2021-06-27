#include "PeerClient.h"


struct PeerClient::RoomsInfo {
	int idRoom;
	int maxClients;
	int countClients;
	bool hasPassword;

	RoomsInfo();
	RoomsInfo(std::string _idRoom, std::string _maxClients, std::string _countClients, std::string _hasPassword) {
		idRoom = std::stoi(_idRoom);
		maxClients = std::stoi(_maxClients);
		countClients = std::stoi(_countClients);
		hasPassword = (_hasPassword == "0" ? true : false);
	}

	std::string toString() {
		std::string s = std::to_string(idRoom) + " | (" + std::to_string(countClients) + " of " + std::to_string(maxClients) + ") " + (hasPassword ? "CLOSED" : "OPEN");
		return s;
	}
};

void PeerClient::RecepcionClient(TcpSocket* sock)
{
	Status status;
	while (loop)
	{
		sf::Packet pack;
		port = sock->GetLocalPort();
		status = sock->Receive(pack);

		if (status == Status::Done) {
			ClientData aux;
			pack >> aux;
			std::cout << aux.ipAddress.ip << ":" << aux.port << std::endl;
			if (aux.ipAddress.ip == sf::IpAddress::LocalHost) {

				sock->Disconnect();
				std::cout << "He recibido el LocalHost\n";
				break;
			}
			else {
				TcpSocket* socket = new TcpSocket;
				status = socket->Connect(aux.ipAddress.ip.toString(), aux.port);

				if (status != Status::Done) {
					break;
				}

				clientes.push_back(socket);

			}
		}
		else if (status == Status::Disconnected) {
			break;
		}

	}
	if (status == Status::Done) {
		Status status = listener.Listen(port);
		while (clientes.size() < 3) {
			TcpSocket* newClient = new TcpSocket;
			status = listener.Accept(newClient);
			if (status == Status::Done) {
				clientes.push_back(newClient);
			}
		}
		listener.Disconnect();
	}
}

void PeerClient::RecepcionMessages()
{
	bool running = true;
	Status status;
	Selector selector;
	for (size_t i = 0; i < clientes.size(); i++)
	{
		selector.Add(clientes[i]);
	}

	while (loop) {
		// Make the selector wait for data on any socket
		if (selector.Wait())
		{
			for (size_t i = 0; i < clientes.size(); i++)
			{
				if (selector.IsReady(clientes[i]))
				{
					// The client has sent some data, we can receive it
					sf::Packet packet;
					status = clientes[i]->Receive(packet);
					if (status == Status::Done)
					{
						std::string strRec;
						packet >> strRec;
						std::cout << "He recibido " << strRec << " del puerto " << clientes[i]->GetRemotePort()<< std::endl;
					}
					else if (status == Status::Disconnected)
					{
						selector.Remove(clientes[i]);
						loop = false;
						std::cout << "Elimino el socket que se ha desconectado\n";
					}
					else
					{
						loop = false;
						std::cout << "Error al recibir de " << clientes[i]->GetRemotePort()<< std::endl;
					}
				}
			}
		}
	}
	listener.Disconnect();
}

void PeerClient::SendMessages()
{
	
	Status status;

	while (loop) {
		std::string message;
		std::getline(std::cin, message);

		sf::Packet packet;
		packet << message;

		for (size_t i = 0; i < clientes.size(); i++)
		{
			status = clientes[i]->Send(packet);

			if (status == Status::Disconnected || message == "exit")
			{
				loop = false;
				break;
			}
		}
	}
}

void PeerClient::Recieve(TcpSocket* socket) {
	sf::Packet p;
	
	Status status = socket->Receive(p);
	if (status == Status::Disconnected) 
	{

	}
	else
	{
		std::string s;
		p >> s;
		std::vector<std::string> parameters = split(s,'_');
		std::cout << "Message recived: " << s << std::endl;

		Message_Protocol mp = GetMessageProtocol(parameters[0]);
		switch (mp)
		{
		case Message_Protocol::S_JOIN_OR_CREATE:
			JoinCreateRecived(socket);
			break;
		case Message_Protocol::C_JOIN_OR_CREATE:

			break;
		case Message_Protocol::GAMES_INFO:
			ShowGamesInfo(socket, parameters);
			break;
		case Message_Protocol::GET_GAMES_INFO:

			break;
		case Message_Protocol::SEND_PLAYERS_IP_PORT:

			break;
		case Message_Protocol::GAMES_FILTRE_SEND:

			break;
		case Message_Protocol::START_GAME:
			myGame.initDeck();
			break;
		case Message_Protocol::NEXT_TURN:
			myGame.NotifyTurn();
			break;
		case Message_Protocol::REQUEST_CARD:
			myGame.NotifyCardRequest(parameters);
			break;
		case Message_Protocol::RESPONSE_REQUEST_CARD:
			myGame.ReceiveCard(parameters);
			break;
		case Message_Protocol::FAMILY_COMPLETE:
			myGame.NotifyFamilyCompleted(parameters);
			break;

		default:
			break;
		}
	}
	
}

void PeerClient::GetGames(TcpSocket* socket)
{
	std::string msg = GetMessageProtocolFrom(Message_Protocol::GET_GAMES_INFO);
	sf::Packet pack;
	pack << msg;

	socket->Send(pack);
}

void PeerClient::ShowGamesInfo(TcpSocket* socket, std::vector<std::string> message)
{
	std::cout << std::endl;
	for (int i = 0; i < stoi(message[1]); i+=2)
	{
		std::cout << message[2 + i] + "\n";
	}
	JoinGame(socket, message);
	
}

void PeerClient::CreateGame(TcpSocket* socket)
{
	std::cout << "How many players? (3-6) " << std::endl;
	int roomSize;
	std::cin >> roomSize;

	if (roomSize < 3)roomSize = 3;
	else if (roomSize > 6) roomSize = 6;

	std::cout << "Do you want to add a password? (y, n) " << std::endl;
	char opt;
	std::cin >> opt;

	std::string passwd;
	if (opt == 'y' || opt == 'Y') {
		std::cout << "Write your password? " << std::endl;
		std::cin >> passwd;
	}
	else
	{
		passwd = " ";
	}

	std::string message =
		GetMessageProtocolFrom(Message_Protocol::C_JOIN_OR_CREATE)
		+ "1_"
		+ std::to_string(roomSize) + "_"
		+ passwd;

	sf::Packet p;
	p << message;
	socket->Send(p);
}

void PeerClient::JoinGame(TcpSocket* socket, std::vector<std::string> message)
{
	const int jump = 3;
	int idRoom;
	std::cin >> idRoom;

	std::string msg = GetMessageProtocolFrom(Message_Protocol::C_JOIN_OR_CREATE);

	msg += "0_" + std::to_string(idRoom) + "_";

	std::string password = "";
	if (message[jump + idRoom] == "1") 
	{
		while (password.empty()) {
			std::cout << "Use password: \n";
			std::cin >> password;
		}
		msg += password;

	}
	sf::Packet pack;
	pack << msg;

	socket->Send(pack);

}

void PeerClient::JoinCreateRecived(TcpSocket* socket) {
	std::cout << JOIN_OR_CREATE;
	int option;
	do
	{
		std::cin >> option;
	} while (option != 0 && option != 1);

	if (option == 0) {
		GetGames(socket);
		Recieve(socket);
	}
	else {
		CreateGame(socket);
		Recieve(socket);
	}
}

void PeerClient::StartGame()
{
	// Repartir Cartas


}
