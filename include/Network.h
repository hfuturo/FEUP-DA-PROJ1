//
// Created by Henrique Silva on 13/03/2023.
//

#ifndef DA_PROJ1_EDGE_H
#define DA_PROJ1_EDGE_H

#include <string>

#include "Station.h"

class Network;

class Network {
    Station* origin;
    Station* dest;
    unsigned int capacity;
    std::string service;
public:
    Network(Station* origin, Station* dest, int capacity, std::string serv);
    Station* getOrigin() const;
    Station* getDest() const;
    unsigned int getCapacity() const;
    std::string getService() const;
};

#endif //DA_PROJ1_EDGE_H
