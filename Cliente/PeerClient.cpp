#include "PeerClient.h"


void PeerClient::ConnectToClients(std::vector<std::string> parameters)
{
	
	Status status;
	int numberofClients = std::stoi(parameters[1]);
	int maxConnectClients = std::stoi(parameters[2]);
	maxConnectClients--;
	std::cout << numberofClients << std::endl;
	for (int i = 0; i<numberofClients; i++) {
		int j = i * 2 + 3;
		TcpSocket* socket = new TcpSocket;
		std::cout << "Try to connect to ip: " + parameters[j] + parameters[j + 1];
		status = socket->Connect(parameters[j], std::stoul(parameters[j + 1]));

		while (status != Status::Done) {
			std::cout << "Trying again";
			status = socket->Connect(parameters[j], std::stoul(parameters[j + 1]));
		}

		clientes.push_back(socket);
	}

	status = listener.Listen(port);
	std::cout << "Estoy buscando clientes\n";
	while (clientes.size() < maxConnectClients) {
		TcpSocket* newClient = new TcpSocket;
		status = listener.Accept(newClient);
		if (status == Status::Done) {
			clientes.push_back(newClient);
			std::cout << "Cliente encontrado\n";
		}
	}
	listener.Disconnect();
	std::cout << "He encontrado todos los clientes: " << clientes.size();
	for (int i = 0; i < clientes.size(); i++) {
		selector.Add(clientes[i]);
	}
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
		loop = false;
	}
	else
	{
		std::string s;
		p >> s;
		std::vector<std::string> parameters = split(s,'_');
		
		std::cout << "Message received: " << s << std::endl;

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
		case Message_Protocol::S_ROOM_INFO:
			RoomInfo(socket, parameters);
			break;
		case Message_Protocol::SEND_PLAYERS_IP_PORT:
			socket->Disconnect();
			ConnectToClients(parameters);
			break;
		case Message_Protocol::GAMES_FILTRE_SEND:

			break;
		case Message_Protocol::JOIN_GAME:

			break;

		default:
			break;
		}
	}
	
}

void PeerClient::ShowGamesInfo(TcpSocket* socket, std::vector<std::string> message)
{
	int number = std::stoi(message[1]);

	std::string roomsInfoString;
	std::vector<bool> hasPassword;
	//?
	for (int i = 0; i < number; i++)
	{
		int j = i * 4 + 2;

		roomsInfoString += message[j] + " | "
			+ message[j + 1] + " of " + message[j + 2] + " | ";

		if (message[j + 3] == "0") {
			hasPassword.push_back(true);
			roomsInfoString += "CLOSSED\n";

		}
		else
		{
			roomsInfoString += "OPPEN\n";
			hasPassword.push_back(false);

		}
		// guardamos si tiene contraseña para pedirla si es necesario
		hasPassword.push_back((message[j + 3] == "0") ? true : false);

	}
	std::cout << roomsInfoString;

	int room;
	std::cout << SELECT_ROOM << "\n";
	std::cin >> room;

	std::string out = GetMessageProtocolFrom(Message_Protocol::C_JOIN_OR_CREATE);
	out += "0_" + std::to_string(room);

	if (hasPassword[room]) {
		std::cout << "Insert password: ";
		std::string pass;
		std::cin >> pass;
		out += SEPARATOR_MESSAGE_PROTOCOL + pass;
	}
	else
	{
		out += SEPARATOR_MESSAGE_PROTOCOL;
		out += " ";
	}

	sf::Packet pack;
	pack << out;

	socket->Send(pack);
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
		passwd = "";
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

void PeerClient::RoomInfo(TcpSocket* socket, std::vector<std::string> message)
{
	for (int i = 0; i < message.size(); i++) 
	{

	}

}

void PeerClient::JoinCreateRecived(TcpSocket* socket) {
	std::cout << JOIN_OR_CREATE;
	int option;
	do
	{
		std::cin >> option;
	} while (option != 0 && option != 1);

	if (option == 0) {
		std::string message = GetMessageProtocolFrom(Message_Protocol::GET_GAMES_INFO);
		sf::Packet p;
		p << message;
		socket->Send(p);

	}
	else {
		CreateGame(socket);
		//Recieve(socket);
	}
}


