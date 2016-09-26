#include "Router.h"


Router::Router(std::list<Route> routes) : routes(routes) {}

void Router::addRoute(Route route) {
    routes.push_back(route);
}

void Router::removeRoute(Route route) {
    routes.remove(route);
}

Route Router::getRouteForAddress(IPv4Address address) {
    Route tempRoute = *routes.begin();
    int mask = 0;

    for ( auto& route: routes ) {
        if ( (route.getNetwork()).contains(address) ) {
            if ( (route.getNetwork()).getMaskLength() > mask ) {
                std::cout << "BEFORE: " << mask << std::endl;
                tempRoute = route;
                mask = (route.getNetwork()).getMaskLength();
                std::cout << "AFTER: " << mask << std::endl;
            }
        }
    }

    return tempRoute;
}

const std::list<Route>& Router::getRoutes() const {
    return routes;
}
