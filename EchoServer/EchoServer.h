#ifndef ECHO_SERVER_H
#define ECHO_SERVER_H

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#define DEFAULT_PORT 20007

class EchoServer {
    private:
        int port;
        bool isServerRunning;
        int server_socket, newsockfd, n;
        struct sockaddr_in serv_addr, cli_addr;
        socklen_t clilen;
        char buffer[256];

    public:
        EchoServer() : EchoServer(DEFAULT_PORT) {};
        EchoServer(int port);

        bool isRunning();
        int getPort();

        void start();
        void stop();
};

#endif
