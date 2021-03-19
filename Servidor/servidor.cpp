#include <iostream>
#include <thread>
#include <string>
#include <vector>
#include <mutex>
#include <chrono>
#include "Selector.h"
#include "ClientData.h"


bool Servidor() {
	bool running = true;
	Status status;
	TcpListener listener;
	std::vector<TcpSocket*> clientes;
	std::vector<ClientData*> clientesData;

	//Accept para esperar socket nuevo
	status = listener.Listen(50000);

	if (status != Status::Done) {
		std::cout << "Error en el listener....cerrando el programa";
		return false;
	}
	else {
		//TODO: limpiar esto
		IpAddress ip = sf::IpAddress::LocalHost.toString();
		std::cout << ip.ip << ":" << std::to_string(50000) << std::endl;

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
					pack << clientesData[i];

					status = newClient->Send(pack);

					if (status == Status::Disconnected)
					{
						break;
					}
				}
				sf::Packet packet;
				ClientData aux;
				aux.ipAddress = sf::IpAddress::LocalHost.toString();
				aux.port = newClient->GetRemotePort().port;
				packet << aux;

				status = newClient->Send(packet);
				clientes.push_back(newClient);
				clientesData.push_back(new ClientData(newClient->GetRemoteIP(), newClient->GetRemotePort().port));
				std::cout << newClient->GetRemoteIP() << ":" << newClient->GetRemotePort().port << "  " << aux.ipAddress.ip;
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

	//Pedimos si queremos ser cliente o servidor


	Servidor();



	return 0;
}

