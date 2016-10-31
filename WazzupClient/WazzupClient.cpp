#include "WazzupClient.h"
#include <stdio.h>

WazzupClient::WazzupClient(const IPv4Address& ipAddress, int port) {
    isConnect = false;
    this->ipAddress = ipAddress;
    this->port = port;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        std::cout << "ERROR opening socket" << std::endl;

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(ipAddress.toInt());
    serv_addr.sin_port = htons(port);
}


bool WazzupClient::isConnected() {
    return isConnect;
}


void WazzupClient::run() {
    if ( !isConnect ) {
        int n;
        char buffer[256];

        isConnect = true;

        if ( connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0 ) {
            std::cout << "ERROR connecting" << std::endl;
        }


        std::cout << "Please enter the message: " << std::endl;
        bzero(buffer, 256);
        fgets(buffer, 255, stdin);

        n = write(sockfd, buffer, strlen(buffer));
        if ( n < 0 ) {
             std::cout << "ERROR writing to socket" << std::endl;
        }

        for ( ; (n = read(sockfd, buffer, 255)) > 0; ) {
            std::cout << buffer << std::endl;
            bzero(buffer, 256);
        }
    }
}


void WazzupClient::disconnect() {
    if ( isConnect ) {
        isConnect = false;
        close(sockfd);
    }
}
