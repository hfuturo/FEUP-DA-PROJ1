//
// Created by Henrique Silva on 13/03/2023.
//

#ifndef DA_PROJ1_EDGE_H
#define DA_PROJ1_EDGE_H

#include <string>

class Network;

class Network {
    std::string station_a;
    std::string station_b;
    unsigned int capacity;
    std::string service;

public:
    Network(std::string station_a, std::string station_b, unsigned int capacity, std::string service);

    std::string getStation_a() const;
    std::string getStation_b() const;
    unsigned int getCapacity() const;
    std::string getService() const;
};

#endif //DA_PROJ1_EDGE_H
