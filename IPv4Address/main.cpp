#include <iostream>
#include "IPv4Address.h"

using namespace std;

int main() {
    IPv4Address* ip = new IPv4Address("127.12.45.22");
    std::cout << ip->toString() << std::endl;  // 127.12.45.22
    std::cout << *ip << std::endl;  // 2131504406

    IPv4Address* ip2 = new IPv4Address(2131504406);
    std::cout << ip2->toString() << std::endl;  // 127.12.45.22
    std::cout << *ip2 << std::endl;    // 2131504406

    std::cout << ip->equals(IPv4Address("127.12.45.22")) << std::endl;        // true
    std::cout << ip->equals(IPv4Address(2131504406L)) << std::endl;           // true
    std::cout << ip->equals(IPv4Address(0xF834AD02L)) << std::endl;           // false
    std::cout << ip->equals(IPv4Address("189.11.23.211")) << std::endl;       // false
    std::cout << ip->greaterThan(IPv4Address("131.16.34.66")) << std::endl;   // false
    std::cout << ip->lessThan(IPv4Address("131.16.34.66")) << std::endl;      // true
    return 0;
}
