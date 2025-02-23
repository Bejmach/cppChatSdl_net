#include <SDL2/SDL_net.h>
#include <SDL3/SDL.h>

#include <iostream>
#include <vector>

const int PORT = 3350;
const int MAX_CLIENTS = 10;
const int BUFFER_SIZE = 512;

struct Client{
	TCPsocket socket;
	IPaddress ip;
};

std::vector<Client* > clients;

void SendMessage(const char* message, Client* sender = nullptr){
	for(int i=0; i<clients.size(); i++){
		if(sender!=nullptr && clients[i]->socket == sender->socket) continue;

		SDLNet_TCP_Send(clients[i]->socket, message, strlen(message)+1);
	}
}

int main(int argc, char* arcv[]){
	if(SDLNet_Init() < 0){
		std::cerr<<"SDLNet_Init failed: "<<SDLNet_GetError()<<std::endl;
		return 1;
	}
	
	IPaddress ip;
	if(SDLNet_ResolveHost(&ip, NULL, PORT) < 0){
		std::cerr<<"SDlNet_ResolveHost failed: "<<SDLNet_GetError()<<std::endl;
		return 1;
	}

	TCPsocket server = SDLNet_TCP_Open(&ip);
	if(!server){
		std::cerr<<"SDLNet_TCP_Open failed: "<<SDLNet_GetError()<<std::endl;
		return 1;
	}

	std::cout<<"server listening on port "<<PORT<<"...\n";

	SDLNet_SocketSet socketSet = SDLNet_AllocSocketSet(MAX_CLIENTS+1);
	SDLNet_TCP_AddSocket(socketSet, server);

	char buffer[BUFFER_SIZE];

	while(true){
		SDLNet_CheckSockets(socketSet, 1000);

		TCPsocket clientSocket = SDLNet_TCP_Accept(server);
		if(clientSocket){
			Client* newClient = new Client({clientSocket, *SDLNet_TCP_GetPeerAddress(clientSocket)});
			clients.push_back(newClient);
			SDLNet_TCP_AddSocket(socketSet, clientSocket);
			std::cout<<"New client connected"<<std::endl;
		}

		for(size_t i=0; i<clients.size(); i++){
			if(SDLNet_SocketReady(clients[i]->socket)){
				int received = SDLNet_TCP_Recv(clients[i]->socket, buffer, BUFFER_SIZE);
				if(received<=0){
					std::cout<<"client disconected"<<std::endl;
					SDLNet_TCP_DelSocket(socketSet, clients[i]->socket);
					SDLNet_TCP_Close(clients[i]->socket);
					delete clients[i];
					clients.erase(clients.begin()+i);
					i--;
				}
				else{
					std::cout<<"Message received: "<<buffer<<std::endl;
					SendMessage(buffer, clients[i]);
				}
			}
		}
	}

	SDLNet_TCP_Close(server);
	SDLNet_Quit();
	return 0;
}
