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

void PeerClient::RecepcionClient(TcpSocket* socket, std::vector<std::string> message)
{
	Status status;

	port = socket->GetLocalPort();
	socket->Disconnect();
	myGame.SetTurnPos(stoi(message[2]));

	for (int i = 3; i < message.size(); i += 2) {
		TcpSocket* sock = new TcpSocket;
		status = sock->Connect(message[i], stoi(message[i + 1]));
		myGame.clientes.push_back(sock);
	}

	//		3-1 = 2			>=2
	//   if maxUsers-1 > clientsSent
	if (stoi(message[1]) - 1 > stoi(message[2]))
	{
		Status status = listener.Listen(port);
		while (myGame.clientes.size() < stoi(message[1])) {
			TcpSocket* newClient = new TcpSocket;
			status = listener.Accept(newClient);
			std::cout << newClient->GetRemoteIP() + ":" + std::to_string(newClient->GetRemotePort()) + " has conected" << std::endl;

			if (status == Status::Done) {
				myGame.clientes.push_back(newClient);
			}
		}
		listener.Disconnect();
	}
	int seed;
	if (myGame.GetTurnPos() == 0)
	{
		seed = port;
	}
	else 
	{
		seed = myGame.clientes[0]->GetRemotePort();
	}
	myGame.SetSeed(seed);
	myGame.shuffleDeck();
	myGame.Update();
}

void PeerClient::RecepcionMessages()
{
	bool running = true;
	Status status;
	Selector selector;
	for (size_t i = 0; i < myGame.clientes.size(); i++)
	{
		selector.Add(myGame.clientes[i]);
	}

	while (loop) {
		// Make the selector wait for data on any socket
		if (selector.Wait())
		{
			for (size_t i = 0; i < myGame.clientes.size(); i++)
			{
				if (selector.IsReady(myGame.clientes[i]))
				{
					// The client has sent some data, we can receive it
					sf::Packet packet;
					status = myGame.clientes[i]->Receive(packet);
					if (status == Status::Done)
					{
						std::string strRec;
						packet >> strRec;
						std::cout << "He recibido " << strRec << " del puerto " << myGame.clientes[i]->GetRemotePort()<< std::endl;
					}
					else if (status == Status::Disconnected)
					{
						selector.Remove(myGame.clientes[i]);
						loop = false;
						std::cout << "Elimino el socket que se ha desconectado\n";
					}
					else
					{
						loop = false;
						std::cout << "Error al recibir de " << myGame.clientes[i]->GetRemotePort()<< std::endl;
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

		for (size_t i = 0; i < myGame.clientes.size(); i++)
		{
			status = myGame.clientes[i]->Send(packet);

			if (status == Status::Disconnected || message == "exit")
			{
				loop = false;
				break;
			}
		}
	}
}

void PeerClient::Recieve(TcpSocket* socket) 
{
	sf::Packet p;
	
	Status status = socket->Receive(p);
	if (status == Status::Disconnected) 
	{
	
	}
	else if(status == Status::Done)
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
			RecepcionClient(socket, parameters);
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
	}
	else {
		CreateGame(socket);
	}
}

void PeerClient::StartGame()
{
	// Repartir Cartas


}
