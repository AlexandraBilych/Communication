#include "EchoServer.h"
#include <system_error>
#include <vector>


EchoServer::EchoServer(int port) : port(port) {
    isServerRunning = false;
    serverSocket = 0;
}


EchoServer::~EchoServer() {
    if ( serverSocket ) {
        close(serverSocket);
    }
}


bool EchoServer::isRunning() {
    return isServerRunning;
}


int EchoServer::getPort() {
    return port;
}


void EchoServer::ClientSocket(int clientSocket) {
    char buffer[256];

    for ( ; ; ) {
        bzero(buffer,256);

        n = read(clientSocket,buffer,255);
        if ( n < 0 ) {
            std::cout << "ERROR reading from socket" << std::endl;
        }

        if ( strcmp(buffer, "disconnect\r\n") == 0 || strcmp(buffer, "") == 0 ) {
            close(clientSocket);
            break;
        }

        n = write(clientSocket, buffer, 255);
        if ( n < 0 ) {
            std::cout << "ERROR writing to socket" << std::endl;
        }
    }
}


void EchoServer::ServerSocket() {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if ( serverSocket < 0 ) {
        std::cout << "ERROR opening socket" << std::endl;
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if ( bind(serverSocket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0 ) {
        std::cout << "ERROR on binding" << std::endl;
    }

    listen(serverSocket, 5);

    for ( int i = 0; ; i++ ) {
        socklen_t clilen;
        clilen = sizeof(cli_addr);

        newsockfd = accept(serverSocket, (struct sockaddr *) &cli_addr, &clilen);

        if (newsockfd < 0) {
            std::cout << "ERROR on accept" << std::endl;
        }

        std::thread client(&EchoServer::ClientSocket, this, newsockfd);
        client.detach();
    }
}

void EchoServer::start() {
    if ( !isServerRunning ) {
        isServerRunning = true;

        server = std::thread(&EchoServer::ServerSocket, this);
        server.detach();
    }
}


void EchoServer::stop() {
    if ( isServerRunning ) {
        close(serverSocket);
        isServerRunning = false;
    }
}
