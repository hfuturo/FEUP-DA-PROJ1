//
// Created by Henrique Silva on 19/03/2023.
//

#ifndef DA_PROJ1_STATIONEDGE_H
#define DA_PROJ1_STATIONEDGE_H

#include <string>
#include <vector>

class Edge;

/************************* Station  **************************/

class Station {
    std::string name;
    std::string district;
    std::string municipality;
    std::string township;
    std::string line;
    std::vector<Edge*> adj;
    std::vector<Edge*> incoming;
public:
    Station(const std::string& name, const std::string& district, const std::string& municipality, const std::string& township, const std::string& line);
    std::string getName() const;
    std::string getDistrict() const;
    std::string getMunicipality() const;
    std::string getTownShip() const;
    std::string getLine() const;
    std::vector<Edge*> getAdj() const;
    std::vector<Edge*> getIncoming() const;
    Edge* addLine(Station* dest, const int capacity, const std::string& service);
    bool removeEdge(std::string name);
    void deleteEdge(Edge* edge);
};

/************************* Edge  **************************/

class Edge {
    Station* origin;
    Station* dest;
    int capacity;
    std::string service;
public:
    Edge(Station* origin, Station* dest, const int capacity, const std::string& service);
    Station* getOrigin() const;
    Station* getDest() const;
    int getCapacity() const;
    std::string getService() const;
};

#endif //DA_PROJ1_STATIONEDGE_H
