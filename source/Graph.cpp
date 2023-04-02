#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <queue>

#include "../include/Graph.h"
#include "../include/constants.h"
#include "../include/StationEdge.h"

std::vector<Station*> Graph::getStationSet() const {
    return this->stationSet;
}

bool Graph::addStation(const std::string& name, const std::string& district, const std::string& municipality, const std::string& township, const std::string& line) {
    if (name.empty() || district.empty() || municipality.empty() || township.empty() || line.empty()) return false;
    stationSet.push_back(new Station(name, district, municipality, township, line));
    return true;
}

void Graph::readStations() {
    std::ifstream stationFile(STATIONS_FILE_PATH);

    if (stationFile.fail()) return;

    std::string fileLine, name, district, municipality, township, line;

    getline(stationFile, fileLine);

    while (getline(stationFile, fileLine)) {
        std::stringstream ss(fileLine);
        getline(ss, name, ',');
        getline(ss, district, ',');
        getline(ss, municipality, ',');
        getline(ss, township, ',');
        getline(ss, line, ',');

        if (findStation(name) != nullptr) continue;
        if (!addStation(name, district, municipality, township, line)) continue;
    }

    stationFile.close();
}

Station *Graph::findStation(const std::string &name) const {
    for (auto& s : getStationSet()) {
        if (s->getName() == name) {
            return s;
        }
    }
    return nullptr;
}

bool Graph::addLine(const std::string &origin, const std::string &dest, const int &capacity, const std::string &service) {
    auto s1 = findStation(origin);
    auto s2 = findStation(dest);
    if (s1 == nullptr || s2 == nullptr) {
        return false;
    }
    s1->addLine(s2, capacity, service);
    return true;
}

bool Graph::addBidirectionalLine(const std::string &origin, const std::string &dest, const int &capacity, const std::string &service) {
    auto s1 = findStation(origin);
    auto s2 = findStation(dest);
    if (s1 == nullptr || s2 == nullptr) {
        return false;
    }
    auto l1 = s1->addLine(s2, capacity, service);
    auto l2 = s2->addLine(s1, capacity, service);
    l1->setReverse(l2);
    l2->setReverse(l1);
    return true;
}

void Graph::readNetwork() {
    std::ifstream networkFile(NETWORK_FILE_PATH);

    if (networkFile.fail()) return;

    std::string fileLine, origin, dest, service, capacity;

    getline(networkFile, fileLine);

    while (getline(networkFile, fileLine)) {
        std::stringstream ss(fileLine);
        getline(ss, origin, ',');
        getline(ss, dest, ',');
        getline(ss, capacity, ',');
        getline(ss, service, ',');
        addLine(origin, dest, stoi(capacity), service);
    }

}

void Graph::fill() {
    readStations();
    readNetwork();
}

bool Graph::dfs(const std::string &source, const std::string &dest) {
    auto s = findStation(source);
    auto d = findStation(dest);

    if (s == nullptr || d == nullptr) {
        return false;
    }

    for (auto& station : getStationSet()) {
        s->setVisited(false);
    }

    return dfsVisit(s, dest);
}

bool Graph::dfsVisit(Station *s, const std::string &dest) {
    s->setVisited(true);
    if (!s->getAdj().empty()) {
        for (auto& e : s->getAdj()) {
            auto neighbor = e->getDest();
            if (e->getDest()->getName() == dest) return true;
            if (!neighbor->isVisited()) {
                if (dfsVisit(neighbor, dest)) return true;
            }
        }
    }
    return false;
}

bool Graph::findAugmentingPath(Station *s, Station *t) {
    for(auto v : stationSet) {
        v->setVisited(false);
    }
    s->setVisited(true);
    std::queue<Station *> q;
    q.push(s);
    while( ! q.empty() && ! t->isVisited()) {
        auto v = q.front();
        q.pop();
        for(auto e: v->getAdj()) {
            testAndVisit(q, e, e->getDest(), e->getCapacity() - e->getFlow());
        }
        for(auto e: v->getIncoming()) {
            testAndVisit(q, e, e->getOrigin(), e->getFlow());
        }
    }
    return t->isVisited();
}

int Graph::findMinResidualAlongPath(Station *s, Station *t) {
    int f = INT32_MAX;
    for (auto v = t; v != s; ) {
        auto e = v->getPath();
        if (e->getDest() == v) {
            f = std::min(f, e->getCapacity() - e->getFlow());
            v = e->getOrigin();
        }
        else {
            f = std::min(f, e->getFlow());
            v = e->getDest();
        }
    }
    return f;
}

void Graph::augmentFlowAlongPath(Station *s, Station *t, int f) {
    for (auto v = t; v != s; ) {
        auto e = v->getPath();
        int flow = e->getFlow();
        if (e->getDest() == v) {
            e->setFlow(flow + f);
            v = e->getOrigin();
        }
        else {
            e->setFlow(flow - f);
            v = e->getDest();
        }
    }
}

int Graph::maxFlow(const std::string &source, const std::string &target) {
    Station* s = findStation(source);
    Station* t = findStation(target);
    if (s == nullptr || t == nullptr || s == t)
        throw std::logic_error("Invalid source and/or target vertex");

    if (!dfs(source, target)) return 0;

    // Reset the flows
    for (auto v : stationSet) {
        for (auto e: v->getAdj()) {
            e->setFlow(0);
        }
    }
    // Loop to find augmentation paths
    while( findAugmentingPath(s, t) ) {
        int f = findMinResidualAlongPath(s, t);
        augmentFlowAlongPath(s, t, f);
    }

    int flow = 0;

    for (auto e : t->getIncoming()) {
        flow += e->getFlow();
    }

    return flow;
}

void Graph::testAndVisit(std::queue<Station *> &q, Edge *e, Station *w, int residual) {
    if (! w->isVisited() && residual > 0) {
        w->setVisited(true);
        w->setPath(e);
        q.push(w);
    }
}

void Graph::fullMaxFlow() {
    int max = INT32_MIN;
    std::vector<std::pair<std::string, std::string>> res;

    for (auto v : getStationSet()) {
        for (auto u : getStationSet()) {
            if (u != v) {
                int flow = maxFlow(v->getName(), u->getName());
                if (flow > max) {
                    max = flow;
                    res.clear();
                    res.emplace_back(v->getName(), u->getName());
                } else if (flow == max) {
                    res.emplace_back(v->getName(), u->getName());
                }
            }
        }
    }

    for (auto v : res) {
        std::cout << v.first << " -> " << v.second << std::endl;
    }
}