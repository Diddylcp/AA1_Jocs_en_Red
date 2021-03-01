#include "PeerClient.h"

void PeerClient::RecepcionClient(sf::TcpSocket* sock)
{
	sf::Socket::Status status;
	while (loop)
	{
		sf::Packet pack;
		myPort = sock->getLocalPort();
		status = sock->receive(pack);

		if (status == sf::Socket::Done) {
			DataCliente aux;
			pack >> aux;
			std::cout << aux.ip << ":" << aux.port << std::endl;
			if (aux.ip == sf::IpAddress::LocalHost) {

				sock->disconnect();
				std::cout << "He recibido el LocalHost\n";
				break;
			}
			else {
				sf::TcpSocket* socket = new sf::TcpSocket;
				status = socket->connect(aux.ip, aux.port);

				if (status != sf::Socket::Status::Done) {
					break;
				}

				clientes.push_back(socket);

			}
		}
		else if (status == sf::Socket::Disconnected) {
			break;
		}

	}
	if (status == sf::Socket::Done) {
		//sf::TcpListener listener;
		sf::Socket::Status status = listener.listen(myPort);
		while (clientes.size() < 3) {
			sf::TcpSocket* newClient = new sf::TcpSocket;
			status = listener.accept(*newClient);
			if (status == sf::Socket::Done) {
				clientes.push_back(newClient);
			}
		}
		listener.close();
	}
	
}

void PeerClient::RecepcionMessages()
{
	bool running = true;
	sf::Socket::Status status;
	sf::SocketSelector selector;
	for (size_t i = 0; i < clientes.size(); i++)
	{
		selector.add(*clientes[i]);
	}
	//selector.add(listener);

	while (loop) {
		// Make the selector wait for data on any socket
		if (selector.wait())
		{
			for (size_t i = 0; i < clientes.size(); i++)
			{
				if (selector.isReady(*clientes[i]))
				{
					// The client has sent some data, we can receive it
					sf::Packet packet;
					status = clientes[i]->receive(packet);
					if (status == sf::Socket::Done)
					{
						std::string strRec;
						packet >> strRec;
						std::cout << "He recibido " << strRec << " del puerto " << clientes[i]->getRemotePort() << std::endl;
					}
					else if (status == sf::Socket::Disconnected)
					{
						selector.remove(*clientes[i]);
						loop = false;
						std::cout << "Elimino el socket que se ha desconectado\n";
					}
					else
					{
						loop = false;
						std::cout << "Error al recibir de " << clientes[i]->getRemotePort() << std::endl;
					}
				}
			}
		}
	}
	listener.close();
}

void PeerClient::SendMessages()
{
	
	sf::Socket::Status status;

	while (loop) {
		std::string message;
		std::getline(std::cin, message);

		sf::Packet packet;
		packet << message;

		for (size_t i = 0; i < clientes.size(); i++)
		{
			status = clientes[i]->send(packet);

			if (status == sf::Socket::Status::Disconnected || message == "exit")
			{
				loop = false;
				break;
			}
		}
	}
}
