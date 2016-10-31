#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "IPv4Address.h"


class WazzupClient {
    private:
        bool isConnect;
        int sockfd, port;
        struct sockaddr_in serv_addr;
        IPv4Address ipAddress;

    public:
        WazzupClient(const IPv4Address& ipAddress, int port);
        ~WazzupClient() {};

        bool isConnected();

        void run();
        void disconnect();
};
