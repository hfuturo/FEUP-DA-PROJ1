//
// Created by Henrique Silva on 13/03/2023.
//

#include <utility>

#include "../include/Network.h"

Network::Network(Station *origin, Station *dest, int capacity, std::string service): origin(origin), dest(dest), capacity(capacity), service(std::move(service)){}

unsigned int Network::getCapacity() const {
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
