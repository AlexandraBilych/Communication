#include <iostream>
#include <string>
#include <vector>

#include "../IPv4Address/IPv4Address.h"
#include "InvalidMaskLengthException.h"


class Network {
    private:
        IPv4Address address;
        uint8_t maskLength;
        std::string string_mask;
        std::string string_network;

        std::string maskToString() const;

    public:
        Network(const IPv4Address& address, int maskLength);
        ~Network() {};


        bool contains(const IPv4Address& address) const;
        IPv4Address getAddress() const;
        IPv4Address getBroadcastAddress() const;
        IPv4Address getFirstUsableAddress() const;
        IPv4Address getLastUsableAddress() const;
        long getMask() const;
        std::string getMaskString() const;
        int getMaskLength() const;
        std::vector< Network > getSubnets() const; // produce two half-sized subnets
        long getTotalHosts() const; // excluding network and broadcast
        bool isPublic() const;

        std::string toString() const;
};

std::ostream& operator<<(std::ostream& out, const Network& net);
