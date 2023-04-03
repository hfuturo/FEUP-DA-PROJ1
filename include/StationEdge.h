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
private:
    std::string name;
    std::string district;
    std::string municipality;
    std::string township;
    std::string line;
    std::vector<Edge*> adj;
    std::vector<Edge*> incoming;
    bool visited;
    Edge* path;
public:
    Station(const std::string& name, const std::string& district, const std::string& municipality, const std::string& township, const std::string& line);
    std::string getName() const;
    std::string getDistrict() const;
    std::string getMunicipality() const;
    std::string getTownShip() const;
    std::string getLine() const;
    std::vector<Edge*> getAdj() const;
    std::vector<Edge*> getIncoming() const;
    Edge* addLine(Station* dest, const double capacity, const std::string& service);
    bool removeEdge(std::string name);
    void removeOutgoingEdges();
    void deleteEdge(Edge* edge);
    void setVisited(bool vis);
    bool isVisited() const;
    void setPath(Edge* path);
    Edge* getPath() const;
};

/************************* Edge  **************************/

class Edge {
private:
    Station* origin;
    Station* dest;
    double capacity;
    std::string service;
    Edge* reverse;
    double flow;
public:
    Edge(Station* origin, Station* dest, const double capacity, const std::string& service);
    Station* getOrigin() const;
    Station* getDest() const;
    double getCapacity() const;
    std::string getService() const;
    void setReverse(Edge* reverse);
    Edge* getReverse() const;
    double getFlow() const;
    void setFlow(double flow);
};

#endif
