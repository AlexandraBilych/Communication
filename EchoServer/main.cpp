#include <iostream>
#include "EchoServer.h"

int main() {
    EchoServer* server = new EchoServer();

    server->isRunning();      // false
    std::cout << server->getPort() << std::endl;        // value of DEFAULT_PORT

    server->start();
    std::cout << server->isRunning() << std::endl;      // true
    server->stop();
    std::cout << server->isRunning() << std::endl;      // false

    delete server;

    server = new EchoServer(54321);
    std::cout << server->getPort() << std::endl;        // 54321
    std::cout << server->isRunning() << std::endl;      // false
    server->start();

    return 0;
}
