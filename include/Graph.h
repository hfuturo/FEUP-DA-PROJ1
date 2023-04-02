#ifndef DA_PROJ1_GRAPH_H
#define DA_PROJ1_GRAPH_H

#include <vector>
#include <queue>

#include "StationEdge.h"

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
    bool addBidirectionalLine(const std::string& origin, const std::string& dest, const int& capacity, const std::string& service);
    int maxFlow(const std::string& source, const std::string& target);
    bool findAugmentingPath(Station* s, Station* t);
    int findMinResidualAlongPath(Station* s, Station* t);
    void augmentFlowAlongPath(Station* s, Station* t, int f);
    void testAndVisit(std::queue<Station*>& q, Edge* e, Station* w, int residual);
    bool dfs(const std::string& source, const std::string& dest);
    bool dfsVisit(Station* s, const std::string& dest);
    void fullMaxFlow();
};

#endif