#include <iostream>
#include <unistd.h>
#include <thread>
#include "Server.h"

int main() {
    Server* server = new Server();

    server->isRunning();      // false
    std::cout << server->isRunning() << std::endl;      // true

    server->start();
    for ( ; server->isRunning(); sleep(10) ) {
    }

    server->stop();
    delete server;

    return 0;
}
