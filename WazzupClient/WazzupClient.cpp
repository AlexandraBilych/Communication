#include "WazzupClient.h"
#include <stdio.h>

WazzupClient::WazzupClient(char* ipAddress, int port) {
    isConnect = false;
    portno = port;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        std::cout << "ERROR opening socket" << std::endl;

    server = gethostbyname(ipAddress);
    if (server == NULL) {
        std::cout << "ERROR, no such host" << std::endl;
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);

    serv_addr.sin_port = htons(portno);
}


bool WazzupClient::isConnected() {
    return isConnect;
}


void WazzupClient::connect() {
    if ( !isConnect ) {
        isConnect = true;

        if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
            printf("Please enter the message: ");
        }

    }
}


void WazzupClient::stop() {
    if ( isConnect ) {
        isConnect = false;
    }
}
