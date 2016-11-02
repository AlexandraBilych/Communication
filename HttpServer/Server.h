#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <thread>
#include <map>
#include <chrono>
#include <iomanip>

struct Request {
    std::string method;
    std::string requestURI;
    std::string protocol;
    std::string host;
};

struct Responce {

};

class Server {
    private:
        int port;
        std::string root_dir;
        bool isServerRunning;
        int serverSocket, newsockfd, n;
        struct sockaddr_in serv_addr, cli_addr;
        std::thread server;

        std::map<std::string, std::string> getSettingsFromConfig();
        std::string parsingRequest(char clientRequest[]);
        Responce createResponce(Request clientRequest);

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
