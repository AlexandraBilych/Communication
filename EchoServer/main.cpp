#include <iostream>
#include <unistd.h>
#include <thread>
#include "EchoServer.h"

int main() {
    EchoServer* server = new EchoServer();

    server->isRunning();      // false
    std::cout << server->getPort() << std::endl;        // value of DEFAULT_PORT
    std::cout << server->isRunning() << std::endl;      // true

    server->start();
    for ( ; server->isRunning(); sleep(10) ) {
    }

    server->stop();
    delete server;

    return 0;
}
