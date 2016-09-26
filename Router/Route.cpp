#include "Route.h"


Route::Route(Network network, IPv4Address gateway, const std::string& interfaceName, int metric) :
    network(network),
    gateway(gateway),
    isGateway(true),
    interfaceName(interfaceName),
    metric(metric) {
    route = "net: " + network.toString() + ". gateway: " + gateway.toString() + ". interface: " + interfaceName + ". metric: " + std::to_string(metric);
}

Route::Route(Network network, const std::string& interfaceName, int metric) :
    network(network),
    gateway(0),
    isGateway(false),
    interfaceName(interfaceName),
    metric(metric) {
    route = "net: " + network.toString() + ". interface: " + interfaceName + ". metric: " + std::to_string(metric);
}

IPv4Address Route::getGateway() const {
    if ( isGateway == true ) {
        return gateway;
    }

    throw new No_Gateway_Exception();
}

const std::string& Route::getInterfaceName() const {
    return interfaceName;
}

int Route::getMetric() const {
    return metric;
}

Network Route::getNetwork() const {
    return network;
}

const std::string& Route::toString() const {
    return route;
}

bool Route::operator==(const Route& route) {
    return network == route.getNetwork() && metric == route.getMetric();
}

std::ostream& operator<<(std::ostream& out, const Route& route) {
    out << route.toString();
    return out;
}
