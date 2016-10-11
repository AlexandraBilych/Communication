#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>

#define PORTNUM 80

int main(int argc, char *argv[]) {
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];

    if ( argc != 2 ) {
       std::cout << "Usage: " << argv[0] << " [url]" << std::endl;
       exit(0);
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if ( sockfd < 0 ) {
        std::cout << "ERROR opening socket" << std::endl;
    }

    server = gethostbyname(argv[1]);
    if (server == NULL ) {
        std::cout << "ERROR, no such host" << argv[1] << std::endl;
        exit(0);
    }

    std::cout << argv[1] << server->h_addr << std::endl;

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(PORTNUM);
    std::cout << "try connect" << std::endl;
    if ( connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr) ) < 0) {
        std::cout << "ERROR connecting" << std::endl;
    }

    std::cout << "client is connecting" << std::endl;

    bzero(buffer,256);
    strcpy(buffer, "GET /wikipedia/commons/9/9c/Image-Porkeri_001.jpg HTTP/1.1\r\nHost: upload.wikimedia.org\r\n\r\n");
    std::cout << "send request" << std::endl;
    n = send(sockfd, buffer, strlen(buffer), 0);
    if ( n < 0 ) {
         std::cout << "ERROR writing to socket" << std::endl;
    }
    std::cout << "wait answer" << std::endl;
    bzero(buffer,256);
    n = read(sockfd, buffer, 255);
    if ( n < 0 ) {
        std::cout << "ERROR reading from socket" << std::endl;
    }
    std::cout << buffer << std::endl;
    close(sockfd);

    return 0;
}
