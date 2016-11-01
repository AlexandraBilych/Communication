#include "Server.h"
#include <vector>

std::map<std::string, std::string> Server::getSettingsFromConfig() {
    std::ifstream file;
    std::string key, value;

    file.open("http.conf", std::ios::in);

    std::map<std::string, std::string> map;
    for ( ; file >> key >> value; ) {
        map[key] = value;
    }

    file.close();

    return map;
}

Server::Server() {
    std::map<std::string, std::string> config;
    config = getSettingsFromConfig();

    port = stoi(config["port"]);
    root_dir = config["root_dir"];
    std::cout << port << "---" << root_dir << "\n";

    isServerRunning = false;
    serverSocket = 0;
}

Server::~Server() {
    if ( serverSocket ) {
        close(serverSocket);
    }
}

bool Server::isRunning() {
    return isServerRunning;
}

int Server::getPort() {
    return port;
}

Request Server::parsingRequest(char clientRequest[]) {
    Request result;
    std::string buffer;

    std::istringstream iss(clientRequest);
    // iss >> result.method >> result.requestURI >> result.protocol ;
    getline(iss, buffer);
    std::cout << buffer << "\n";
    getline(iss, buffer);
    std::cout << buffer << "\n";

    return result;
}

Responce Server::createResponce(Request clientRequest) {
    Responce result;

    if ( clientRequest.method != "GET" ) {

    }
    //iss >> result.host;

    return result;
}

void Server::ClientSocket(int clientSocket) {
    char buffer[256];
    Request request;
    Responce responce;

    for ( ; isServerRunning; ) {
        bzero(buffer,256);

        n = read(clientSocket,buffer,255);
        if ( n < 0 ) {
            std::cout << "ERROR reading from socket" << std::endl;
        }

        request = parsingRequest(buffer);
        responce = createResponce(request);

        // std::cout << request.method << "\n";
        // std::cout << request.requestURI << "\n";
        // std::cout << request.protocol << "\n";
        // std::cout << request.host << "\n";

        if ( strcmp(buffer, "disconnect\r\n") == 0 || strcmp(buffer, "") == 0 ) {
            break;
        }

        if ( strcmp(buffer, "stop\r\n") == 0 ) {
            stop();
            break;
        }

        n = write(clientSocket, buffer, 255);
        if ( n < 0 ) {
            std::cout << "ERROR writing to socket" << std::endl;
        }
    }

    close(clientSocket);
}

void Server::ServerSocket() {
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

        std::thread client(&Server::ClientSocket, this, newsockfd);
        client.detach();
    }
}

void Server::start() {
    if ( !isServerRunning ) {
        isServerRunning = true;

        server = std::thread(&Server::ServerSocket, this);
        server.detach();
    }
}

void Server::stop() {
    if ( isServerRunning ) {
        close(serverSocket);
        isServerRunning = false;
    }
}
