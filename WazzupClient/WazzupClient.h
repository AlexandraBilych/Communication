#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>


class WazzupClient {
    private:
        bool isConnect;
        struct hostent *server;
        int portno, sockfd;
        struct sockaddr_in serv_addr;

    public:
        WazzupClient(char* ipAddress, int port);
        ~WazzupClient() {};

        bool isConnected();

        void connect();
        void stop();
};
