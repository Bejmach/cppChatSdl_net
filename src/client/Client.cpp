#include <SDL2/SDL_net.h>
#include <SDL3/SDL.h>

#include <thread>

#include <iostream>

const char* SERVER_IP = "127.0.0.1";
const int PORT = 3350;
const int BUFFER_SIZE = 512;

bool running = true;

void receiveMessages(TCPsocket socket) {
    char buffer[BUFFER_SIZE];
    while (running) {
        int received = SDLNet_TCP_Recv(socket, buffer, BUFFER_SIZE);
        if (received <= 0) {
            std::cout << "Disconnected from server.\n";
            running = false;
            break;
        }
        std::cout << "\n[Server]: " << buffer << "\n> ";
        std::cout.flush(); // Fix input formatting
    }
}

void sendMessages(TCPsocket socket) {
    char buffer[BUFFER_SIZE];
    while (running) {
        std::cout << "> ";
        std::cin.getline(buffer, BUFFER_SIZE);
        SDLNet_TCP_Send(socket, buffer, strlen(buffer) + 1);
    }
}

int main(int argc, char* argv[]) {
    if (SDLNet_Init() < 0) {
        std::cerr << "SDLNet_Init failed: " << SDLNet_GetError() << std::endl;
        return 1;
    }

    IPaddress ip;
    if (SDLNet_ResolveHost(&ip, SERVER_IP, PORT) < 0) {
        std::cerr << "SDLNet_ResolveHost failed: " << SDLNet_GetError() << std::endl;
        return 1;
    }

    TCPsocket socket = SDLNet_TCP_Open(&ip);
    if (!socket) {
        std::cerr << "SDLNet_TCP_Open failed: " << SDLNet_GetError() << std::endl;
        return 1;
    }

    std::cout << "Connected to server! Type messages to send.\n";

    char buffer[BUFFER_SIZE];
    // Start the receiving and sending threads
    std::thread receiveThread(receiveMessages, socket);
    std::thread sendThread(sendMessages, socket);

    receiveThread.join();
    sendThread.join();

    SDLNet_TCP_Close(socket);
    SDLNet_Quit();
	return 0;
}
