#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <mutex>
#include <chrono>
#include <SFML/Network.hpp>

struct DataCliente {

	
	DataCliente() {	}
	DataCliente(sf::IpAddress _ip, unsigned short _port) {
		ip = _ip;
		port = _port;
	}
	sf::IpAddress ip;
	unsigned short port;

	
};

#pragma region Sobrecargado de Operadores

sf::Packet& operator>>(sf::Packet& packet, DataCliente cliente)
{
	std::string ipStr;
	packet >> ipStr;
	cliente.ip = sf::IpAddress(ipStr);
	return packet >> cliente.port;
}

sf::Packet& operator<<(sf::Packet& packet, DataCliente cliente)
{
	packet << cliente.ip.toString();
	packet << cliente.port;
	return packet;
}

#pragma endregion



bool Servidor() {
	bool running = true;
	sf::Socket::Status status;
	sf::TcpListener listener;
	std::vector<sf::TcpSocket*> clientes;
	std::vector<DataCliente> clientesData;

	//Accept para esperar socket nuevo
	status = listener.listen(50000);

	if (status != sf::Socket::Done) {
		std::cout << "Error en el listener....cerrando el programa";
		return false;
	}
	else {
		sf::IpAddress ip = sf::IpAddress::LocalHost;
		std::cout << ip.getLocalAddress() << ":" << std::to_string(50000) << std::endl;

		while (running) {
			sf::TcpSocket* newClient = new sf::TcpSocket;
			status = listener.accept(*newClient);

			if (status != sf::Socket::Done) {
				std::cout << "Error en el listener....cerrando el programa";
				return false;
			}
			else {
				std::cout << "Tenemos " << clientes.size() << " clientes conectados\n";
				for (size_t i = 0; i < clientes.size(); i++)
				{
					sf::Packet pack;
					pack << clientesData[i];

					status = newClient->send(pack);

					if (status == sf::Socket::Status::Disconnected)
					{
						break;
					}
				}
				sf::Packet packet;
				DataCliente aux;
				aux.ip = sf::IpAddress::LocalHost;
				aux.port = newClient->getRemotePort();
				packet << aux;

				status = newClient->send(packet);
				clientes.push_back(newClient);
				clientesData.push_back(DataCliente(newClient->getRemoteAddress(), newClient->getRemotePort()));
				newClient->disconnect();

			}
			
			if (clientes.size() == 4) {
				std::cout << "Se han conectado 4 personas\n";
				running = false;
				listener.close();
			}
		}
		for (size_t i = 0; i < clientes.size(); i++)
		{
			clientes[i]->disconnect();
			delete clientes[i];
		}
		clientes.clear();
		return true;

	}
}

int main() {

	//Pedimos si queremos ser cliente o servidor


	Servidor();



	return 0;
}

