#include "WazzupClient.h"
#include "IPv4Address.h"
#include <iostream>

int main(int argc, char *argv[]) {
    std::cout << argv[1] << '\n';
    //std::cout << std::stoi(argv[2]) << '\n';
    WazzupClient client(IPv4Address(argv[1]), 20006);

    client.run();

    std::cout << "Stop" << std::endl;
    client.disconnect();

    return 0;
}
