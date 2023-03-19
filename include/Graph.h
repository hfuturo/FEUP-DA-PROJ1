#ifndef DA_PROJ1_GRAPH_H
#define DA_PROJ1_GRAPH_H

#include <vector>

#include "Station.h"

class Graph;

class Graph {
    std::vector<Station*> stationSet;

public:
    Graph(){};
    std::vector<Station*> getStationSet() const;
    void fill();
    void readStations();
    void readNetwork();
    bool addStation(const std::string& name, const std::string& district, const std::string& municipality, const std::string& township, const std::string& line);
    bool addLine(const std::string& origin, const std::string& dest, const int& capacity, const std::string& service);
    Station* findStation(const std::string& name) const;

};

#endif