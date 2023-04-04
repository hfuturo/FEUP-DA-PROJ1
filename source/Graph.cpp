#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>
#include <queue>
#include <unordered_map>

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

bool Graph::addLine(const std::string &origin, const std::string &dest, const double &capacity, const std::string &service) {
    auto s1 = findStation(origin);
    auto s2 = findStation(dest);
    if (s1 == nullptr || s2 == nullptr) {
        return false;
    }
    s1->addLine(s2, capacity, service);
    return true;
}

bool Graph::addBidirectionalLine(const std::string &origin, const std::string &dest, const double &capacity, const std::string &service) {
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

bool Graph::removeStation(const Station *station) {
    std::vector<Station*> &vs = stationSet;
    int count = 0;
    for (auto& v : vs) {
        if(v->getName() == station->getName()) {
            for (auto& u : vs) {
                u->removeEdge(station->getName());
            }

            v->removeOutgoingEdges();
            vs.erase(vs.begin() + count);
            delete v;
            return true;
        }
        count++;
    }

    return false;
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
        addBidirectionalLine(origin, dest, std::stod(capacity), service);
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

double Graph::findMinResidualAlongPath(Station *s, Station *t) {
    double f = INT32_MAX;
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

void Graph::augmentFlowAlongPath(Station *s, Station *t, double f) {
    for (auto v = t; v != s; ) {
        auto e = v->getPath();
        double flow = e->getFlow();
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

double Graph::maxFlow(const std::string &source, const std::string &target) {
    Station* s = findStation(source);
    Station* t = findStation(target);
    if (s == nullptr || t == nullptr || s == t) {
        return -2;
    }

    if (!dfs(source, target)) return -1;

    // Reset the flows
    for (auto v : stationSet) {
        for (auto e: v->getAdj()) {
            e->setFlow(0);
        }
    }
    // Loop to find augmentation paths
    while( findAugmentingPath(s, t) ) {
        double f = findMinResidualAlongPath(s, t);
        augmentFlowAlongPath(s, t, f);
    }

    double flow = 0;

    for (auto e : t->getIncoming()) {
        flow += e->getFlow();
    }

    return flow;
}

void Graph::testAndVisit(std::queue<Station *> &q, Edge *e, Station *w, double residual) {
    if (! w->isVisited() && residual > 0) {
        w->setVisited(true);
        w->setPath(e);
        q.push(w);
    }
}

//TODO: remover pares "repetidos" (u,v) (v,u) do vetor e melhorar complexidade temporal
std::vector<std::pair<double, std::pair<std::string, std::string>>> Graph::fullMaxFlow() {
    std::vector<std::pair<double, std::pair<std::string, std::string>>> res;
    double max = INT32_MIN;
    double flow = 0;

    for (auto v : getStationSet()) {
        for (auto u : getStationSet()) {
            if (v != u) {
                flow = maxFlow(v->getName(), u->getName());
                if (flow > max) {
                    max = flow;
                    res.clear();
                    res.push_back({flow, {v->getName(), u->getName()}});
                } else if (flow == max) {
                    res.push_back({flow, {v->getName(), u->getName()}});
                }
            }
        }
    }

    return res;
}

std::vector<std::pair<std::string, double>> Graph::topDistricts(int n) {
    std::vector<std::pair<std::string, double>> res;

    for (auto v : getStationSet()) {
        bool exists = false;
        for (auto p : res) {
            if (v->getDistrict() == p.first) {
                exists = true;
                break;
            }
        }

        if (!exists) {
            res.emplace_back(v->getDistrict(), 0);
        }
    }

    for (auto v : getStationSet()) {
        for (auto u : getStationSet()) {
            if (v != u) {
                double flow = maxFlow(v->getName(), u->getName());
                for (auto& p : res) {
                    if (p.first == u->getDistrict()) {
                        p.second += flow;
                    }
                }
            }
        }
    }

    std::sort(res.begin(), res.end(), [](std::pair<std::string, double>& p1, std::pair<std::string, double>& p2) {return p1.second < p2.second;});

    if (n > res.size()) {
        return res;
    }

    return {res.begin(), res.begin() + n};
}

std::vector<std::pair<std::string, double>> Graph::topMunicipalities(int n) {
    std::vector<std::pair<std::string, double>> res;

    for (auto v : getStationSet()) {
        bool exists = false;
        for (auto p : res) {
            if (v->getMunicipality() == p.first) {
                exists = true;
                break;
            }
        }

        if (!exists) {
            res.emplace_back(v->getMunicipality(), 0);
        }
    }

    for (auto v : getStationSet()) {
        for (auto u : getStationSet()) {
            if (v != u) {
                double flow = maxFlow(v->getName(), u->getName());
                for (auto& p : res) {
                    if (p.first == u->getMunicipality()) {
                        p.second += flow;
                    }
                }
            }
        }
    }

    std::sort(res.begin(), res.end(), [](std::pair<std::string, double>& p1, std::pair<std::string, double>& p2) {return p1.second < p2.second;});

    if (n > res.size()) {
        return res;
    }

    return {res.begin(), res.begin() + n};
}

double Graph::maxFlowGridToStation(const std::string &dest) {
    Station* target = findStation(dest);
    if (target == nullptr) {
        return -1;
    }

    if(!addStation("super source", "filler", "filler", "filler", "filler")) return -2;

    for (auto& v : getStationSet()) {
        if (v != target && v->getAdj().size() == 1) {
            addBidirectionalLine("super source", v->getName(), INT32_MAX, "filler");
        }
    }

    double flow = maxFlow("super source", target->getName());

    auto supersource = findStation("super source");

    removeStation(supersource);

    return flow;
}

double Graph::maxFlowSubGraph(const std::vector<std::pair<std::string, std::string>> &linesToRemove, const std::string& origin, const std::string& dest) {
    std::vector<std::pair<Station*, Station*>> stations;
    std::vector<std::pair<std::pair<Station*, Station*>, std::pair<double, std::string>>> removedEdges;

    Station* source = findStation(origin);
    Station* sink = findStation(dest);

    if (source == nullptr || sink == nullptr || source == sink) return -1;

    for (auto& name : linesToRemove) {
        Station* station1 = findStation(name.first);
        Station* station2 = findStation(name.second);
        if (station1 == nullptr || station2 == nullptr) return -2;
        stations.emplace_back(station1, station2);
    }

/*    for (auto& s : stations) {
        auto edges1 = s.first->getAdj();
        auto out1 = s.first->getIncoming();
        auto edges2 = s.second->getAdj();
        auto out2 = s.second->getIncoming();

        std::cout << s.first->getName() << std::endl;
        std::cout << "OUT" << std::endl;
        for (auto& e : edges1) {
            std::cout << "\t" << e->getOrigin()->getName() << " " << e->getDest()->getName() << " " << e->getCapacity() << " " << e->getService();
        }

        std::cout << "IN" << std::endl;
        for (auto& e : out1) {
            std::cout << "\t" << e->getOrigin()->getName() << " " << e->getDest()->getName() << " " << e->getCapacity() << " " << e->getService();
        }

        std::cout << s.second->getName() << std::endl;
        std::cout << "OUT" << std::endl;
        for (auto& e : edges2) {
            std::cout << "\t" << e->getOrigin()->getName() << " " << e->getDest()->getName() << " " << e->getCapacity() << " " << e->getService();
        }

        std::cout << "IN" << std::endl;
        for (auto& e : edges1) {
            std::cout << "\t" << e->getOrigin()->getName() << " " << e->getDest()->getName() << " " << e->getCapacity() << " " << e->getService();
        }

    } */

    for (auto s : stations) {
        std::cout << s.first->getName() << " " << s.first->getAdj().size() << " " <<  s.first->getIncoming().size() << std::endl;
        std::cout << s.second->getName() << " " << s.second->getAdj().size() << " " << s.second->getIncoming().size() << std::endl;
    }

    for (auto& p : stations) {
        Edge* edge = p.first->removeAndStoreEdge(p.second);
        if (edge == nullptr) continue;
        removedEdges.push_back({{p.first, p.second}, {edge->getCapacity(), edge->getService()}});
        p.second->removeAndStoreEdge(p.first);
    }

    for (auto s : stations) {
        std::cout << s.first->getName() << " " << s.first->getAdj().size() <<" " <<  s.first->getIncoming().size() << std::endl;
        std::cout << s.second->getName() << " " << s.second->getAdj().size() << " " << s.second->getIncoming().size() << std::endl;
    }

    double flow = maxFlow(origin, dest);

 /*   for (auto& p : removedEdges) {
        addBidirectionalLine(p.first.first->getName(), p.first.second->getName(), p.second.first, p.second.second);
    } */

 /*   for (auto& s : stations) {
        auto edges1 = s.first->getAdj();
        auto out1 = s.first->getIncoming();
        auto edges2 = s.second->getAdj();
        auto out2 = s.second->getIncoming();

        std::cout << s.first->getName() << std::endl;
        std::cout << "OUT" << std::endl;
        for (auto& e : edges1) {
            std::cout << "\t" << e->getOrigin()->getName() << " " << e->getDest()->getName() << " " << e->getCapacity() << " " << e->getService();
        }

        std::cout << "IN" << std::endl;
        for (auto& e : out1) {
            std::cout << "\t" << e->getOrigin()->getName() << " " << e->getDest()->getName() << " " << e->getCapacity() << " " << e->getService();
        }

        std::cout << s.second->getName() << std::endl;
        std::cout << "OUT" << std::endl;
        for (auto& e : edges2) {
            std::cout << "\t" << e->getOrigin()->getName() << " " << e->getDest()->getName() << " " << e->getCapacity() << " " << e->getService();
        }

        std::cout << "IN" << std::endl;
        for (auto& e : edges1) {
            std::cout << "\t" << e->getOrigin()->getName() << " " << e->getDest()->getName() << " " << e->getCapacity() << " " << e->getService();
        }

    } */

    if (flow == -2) return -2;
    if (flow == -1) return -1;

    return flow;
}
