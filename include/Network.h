#ifndef DA_PROJ1_NETWORK_H
#define DA_PROJ1_NETWORK_H

#include <string>

#include "Station.h"

class Network;

class Network {
    int capacity;
    std::string service;
    Station* origin;
    Station* dest;
public:
    Network(Station* origin, Station* dest, int capacity, const std::string& service);
    Station* getOrigin() const;
    Station* getDest() const;
    int getCapacity() const;
    std::string getService() const;
};

#endif