#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <thread>

class Server {
    private:
        int port;
        std::string root_dir;
        bool isServerRunning;
        int serverSocket, newsockfd, n;
        struct sockaddr_in serv_addr, cli_addr;
        std::thread server;

        void getSettingsFromConfig();

        void ClientSocket(int clientSocket);
        void ServerSocket();
    public:
        Server();

        ~Server();

        bool isRunning();
        int getPort();

        void start();
        void stop();
};

#endif
