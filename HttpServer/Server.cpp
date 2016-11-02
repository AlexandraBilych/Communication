#include "Server.h"
#include <vector>

//GET \ HTTP\1.1

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
    root_dir.pop_back();
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

std::string Server::parsingRequest(char clientRequest[]) {
    Request request;
    std::string responce;
    std::string buffer;
    std::ifstream searchFile;

    std::istringstream iss(clientRequest);

    iss >> request.method;
    if ( request.method != "GET" ) {
        std::cout << "405 Method Not Allowed" << std::endl;
        responce = "405 Method Not Allowed";
        return responce;
    }

    iss >> request.requestURI;
    if ( request.requestURI == "/" ) {
        searchFile.open(root_dir + request.requestURI + "index.html");
    } else {
        searchFile.open(root_dir + request.requestURI);
    }

    std::cout << (root_dir + request.requestURI) << std::endl;
    std::cout << searchFile.good() << std::endl;

    if ( !searchFile.good() ) {
        std::cout << "404 /some-strange-url.notfound" << std::endl;
        responce = "404 /some-strange-url.notfound";
        return responce;
    }

    iss >> request.protocol;
    if ( request.protocol != "HTTP/1.1" ) {
        std::cout << "400 Bad request" << std::endl;
        responce = "400 Bad request";
        return responce;
    }

    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%a, %e %h %Y %H:%M:%S");

    char buff[256];
    snprintf(buff, sizeof(buff), "HTTP/1.1 200 OK\r\n\
        Connection: close\r\n\
        Content-Length: %d\r\n\
        Content-Type: text/html; charset=UTF-8\r\n\
        Date: %s GMT\r\n\r\n",\
        (int)searchFile.tellg(), (ss.str()).c_str());
    responce = buff;

    std::cout << "200 " << request.requestURI << std::endl;
    //std::cout << request.host << "\n";

    // getline(iss, buffer);
    // std::cout << buffer << "\n";
    // getline(iss, buffer);
    // std::cout << buffer << "\n";

    return responce;
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
    std::string responce;

    for ( ; isServerRunning; ) {
        bzero(buffer,256);

        n = read(clientSocket,buffer,255);
        if ( n < 0 ) {
            std::cout << "ERROR reading from socket" << std::endl;
        }

        responce = parsingRequest(buffer);

        n = write(clientSocket, responce.c_str(), 255);
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
