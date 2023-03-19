//
// Created by Henrique Silva on 13/03/2023.
//

#include "../include/Network.h"
#include "../include/Station.h"

Network::Network(Station *origin, Station *dest, int capacity, const std::string& service): origin(origin), dest(dest), capacity(capacity), service(service){}

int Network::getCapacity() const {
    return this->capacity;
}

std::string Network::getService() const {
    return this->service;
}

Station* Network::getOrigin() const {
    return this->origin;
}

Station* Network::getDest() const {
    return this->dest;
}
