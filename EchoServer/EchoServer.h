#ifndef ECHO_SERVER_H
#define ECHO_SERVER_H

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <thread>

#define DEFAULT_PORT 20007

class EchoServer {
    private:
        int port;
        bool isServerRunning;
        int serverSocket, newsockfd, n;
        struct sockaddr_in serv_addr, cli_addr;
        std::thread server;

        void ClientSocket(int clientSocket);
        void ServerSocket();
    public:
        EchoServer() : EchoServer(DEFAULT_PORT) {};
        EchoServer(int port);

        ~EchoServer();

        bool isRunning();
        int getPort();

        void start();
        void stop();
};

#endif
