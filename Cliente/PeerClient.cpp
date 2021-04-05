#include "PeerClient.h"



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
	
}

void PeerClient::ShowGamesInfo(TcpSocket* socket, std::vector<std::string> message)
{
	int number = std::stoi(message[1]);

	std::string roomsInfoString;
	std::vector<bool> hasPassword;

	for (int i = 0; i < number; i++)
	{
		for (int j = 2; j < 4; j += 4)
		{
			roomsInfoString += message[j] + " | "
				+ message[j + 1] + " of " + message[j + 2]
				+ " | " + message[j + 3] + "\n";
			// guardamos si tiene contraseña para pedirla si es necesario
			hasPassword.push_back((message[j + 3] == "0") ? true : false);
		}
		std::cout << roomsInfoString;
	}

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

void PeerClient::JoinGame(TcpSocket* socket)
{
	
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
		Recieve(socket);
	}
	else {
		CreateGame(socket);
		Recieve(socket);
	}
}
