#ifndef ROUTE_H
#define ROUTE_H

#include <string>

#include "IPv4Address.h"
#include "Network.h"

class No_Gateway_Exception {};

class Route {
    private:
        Network network;
        IPv4Address gateway;
        bool isGateway;
        std::string interfaceName;
        int metric;
        std::string route;

    public:
        Route(Network network, IPv4Address gateway, const std::string& interfaceName, int metric);
        Route(Network network, const std::string& interfaceName, int metric);

        IPv4Address getGateway() const;
        const std::string& getInterfaceName() const;
        int getMetric() const;
        Network getNetwork() const;
        const std::string& toString() const;

        bool operator==(const Route& route);
};

std::ostream& operator<<(std::ostream& out, const Route& route);

#endif // ROUTE_H
