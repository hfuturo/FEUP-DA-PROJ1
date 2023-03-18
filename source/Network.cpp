//
// Created by Henrique Silva on 13/03/2023.
//

#include "../include/Network.h"

Network::Network(std::string station_a, std::string station_b, unsigned int capacity, std::string service) {

}

unsigned int Network::getCapacity() const {
    return this->capacity;
}

std::string Network::getService() const {
    return this->service;
}

std::string Network::getStation_a() const {
    return this->station_a;
}

std::string Network::getStation_b() const {
    return this->station_b;
}