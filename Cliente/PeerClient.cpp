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
			DataCliente aux;
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
			TcpSocket newClient;
			status = listener.Accept(&newClient);
			if (status == Status::Done) {
				clientes.push_back(&newClient);
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
						std::cout << "He recibido " << strRec << " del puerto " << clientes[i]->GetRemotePort().port << std::endl;
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
						std::cout << "Error al recibir de " << clientes[i]->GetRemotePort().port << std::endl;
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
