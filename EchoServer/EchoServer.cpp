#include "EchoServer.h"



EchoServer::EchoServer(int port) : port(port){
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if ( server_socket < 0 ) {
        std::cout << "ERROR opening socket" << std::endl;
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        std::cout << "ERROR on binding" << std::endl;
    }

    listen(server_socket, 2);

    clilen = sizeof(cli_addr);
    newsockfd = accept(server_socket, (struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0)
        std::cout << "ERROR on accept" << std::endl;

    bzero(buffer,256);
    n = read(newsockfd,buffer,255);
    if ( n < 0 ) {
        std::cout << "ERROR reading from socket" << std::endl;
    }
    std::cout << "Here is the message: " << buffer << std::endl;

    n = write(newsockfd,"I got your message",18);
    if ( n < 0 ) {
        std::cout << "ERROR writing to socket" << std::endl;
    }

    close(newsockfd);
    close(server_socket);

    isServerRunning = false;
}


bool EchoServer::isRunning() {
    return isServerRunning;
}


int EchoServer::getPort() {
    return port;
}


void EchoServer::start() {
    if ( !isServerRunning ) {

        isServerRunning = true;
    }
}


void EchoServer::stop() {
    if ( isServerRunning ) {

        isServerRunning = false;
    }
}
