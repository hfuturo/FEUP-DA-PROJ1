#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <queue>
#include <map>
#include <unordered_map>
#include <iostream>

#include "../include/Graph.h"
#include "../include/constants.h"
#include "../include/MutablePriorityQueue.h"

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
        getline(ss, service);
        addBidirectionalLine(origin, dest, std::stod(capacity), service);
    }

}

void Graph::fill() {
    readStations();
    readNetwork();
}

bool Graph::dfs(const std::string &source, const std::string &dest, const std::string& service) {
    auto s = findStation(source);
    auto d = findStation(dest);

    if (s == nullptr || d == nullptr) {
        return false;
    }

    for (auto& station : getStationSet()) {
        station->setVisited(false);
    }


    if (service == "ALL") return dfsVisit(s, dest);
    if (service == "STANDARD") return dfsVisitStandard(s, dest);
    if (service == "ALFA PENDULAR") return dfsVisitAlfaPendular(s, dest);
    return false;
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

bool Graph::dfsVisitStandard(Station *s, const std::string &dest) {
    s->setVisited(true);
    if (!s->getAdj().empty()) {
        for (auto& e : s->getAdj()) {
            if (e->getService().size()-1 == 8) {
                auto neighbor = e->getDest();
                if (e->getDest()->getName() == dest) return true;
                if (!neighbor->isVisited()) {
                    if (dfsVisitStandard(neighbor, dest)) return true;
                }
            }
        }
    }
    return false;
}

bool Graph::dfsVisitAlfaPendular(Station *s, const std::string &dest) {
    s->setVisited(true);
    if (!s->getAdj().empty()) {
        for (auto& e : s->getAdj()) {
            if (e->getService().size()-1 == 13) {
                auto neighbor = e->getDest();
                if (e->getDest()->getName() == dest) return true;
                if (!neighbor->isVisited()) {
                    if (dfsVisitAlfaPendular(neighbor, dest)) return true;
                }
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

    if (!dfs(source, target, "ALL")) return -1;

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

std::vector<std::pair<double, std::pair<std::string, std::string>>> Graph::fullMaxFlow() {
    std::map<std::pair<std::string, std::string>, double> map;
    std::vector<std::pair<double, std::pair<std::string, std::string>>> res;

    for (int i = 0; i < getStationSet().size() - 1; i++) {
        Station* u = getStationSet().at(i);
        for (int j = i+1; j < getStationSet().size(); j++) {
            Station* v = getStationSet().at(j);
            double flow = maxFlow(u->getName(), v->getName());
            if (flow == -1 || flow == -2) continue;
            map.insert({{u->getName(), v->getName()}, flow});
        }
    }

    //O(V)
    for (auto& it : map) {
        res.emplace_back(it.second, it.first);
    }

    //O(logn)
    std::sort(res.begin(), res.end(), [](std::pair<double, std::pair<std::string, std::string>>& p1, std::pair<double, std::pair<std::string, std::string>>& p2){
        return p1.first > p2.first;
    });

    double max = res.front().first;
    int counter = 1;

    //O(n)
    for (int i = 1; i < res.size(); i++) {
        if (res.at(i).first < max) break;
        counter++;
    }

    std::vector<std::pair<double, std::pair<std::string, std::string>>> final(res.begin(), res.begin() + counter);
    return final;
}

std::vector<std::pair<std::string, double>> Graph::topDistricts(int n) {
    std::unordered_map<std::string, double> map;

    //O(v)
    for (auto v : getStationSet()) {
        map.insert({v->getDistrict(), 0});
    }

    //calcula flow entre estacoes do mesmo distrito
    for (auto v : getStationSet()) { // O(V)
        for (auto u : getStationSet()) { // O(V)
            if (v != u) {
                if (u->getDistrict() == v->getDistrict()) {
                    double flow = maxFlow(v->getName(), u->getName()); // O(VE^2)
                    if (flow == -1 || flow == -2) continue;
                    map[u->getDistrict()] += flow;
                }
            }
        }
    }

    //calcula flow entre estacoes de fora para dentro do distrito
    for (auto v : getStationSet()) {
        auto edges = v->getAdj();
        for (auto e : edges) {
            Station* neighbor = e->getDest();
            map[neighbor->getDistrict()] += e->getCapacity();
        }
    }

    std::vector<std::pair<std::string, double>> res;

    for (auto& it : map) {
        res.emplace_back(it);
        std::cout << it.first << " " << it.second << std::endl << std::endl;
    }

    std::sort(res.begin(), res.end(), [](std::pair<std::string, double>& p1, std::pair<std::string, double>& p2) {return p1.second > p2.second;});

    if (n > res.size()) {
        return res;
    }

    return {res.begin(), res.begin() + n};
}

std::vector<std::pair<std::string, double>> Graph::topMunicipalities(int n) {
    std::unordered_map<std::string, double> map;

    for (auto v : getStationSet()) {
        map.insert({v->getMunicipality(), 0});
    }

    //calcula flow entre estacoes do mesmo distrito
    for (auto v : getStationSet()) {
        for (auto u : getStationSet()) {
            if (v != u) {
                if (u->getMunicipality() == v->getMunicipality()) {
                    double flow = maxFlow(v->getName(), u->getName());
                    if (flow == -1 || flow == -2) continue;
                    map[u->getMunicipality()] += flow;
                }
            }
        }
    }

    //calcula flow entre estacoes de fora para dentro do distrito
    for (auto v : getStationSet()) {
        auto edges = v->getAdj();
        for (auto e : edges) {
            Station* neighbor = e->getDest();
            map[neighbor->getMunicipality()] += e->getCapacity();
        }
    }

    std::vector<std::pair<std::string, double>> res;

    for (auto& it : map) {
        res.emplace_back(it);
    }

    std::sort(res.begin(), res.end(), [](std::pair<std::string, double>& p1, std::pair<std::string, double>& p2) {return p1.second > p2.second;});

    if (n > res.size()) {
        return res;
    }

    return {res.begin(), res.begin() + n};
}

double Graph::maxFlowGridToStation(const std::string &dest) {
    Station* target = findStation(dest); //O(V)
    if (target == nullptr) {
        return -1;
    }

    if(!addStation("super source", "filler", "filler", "filler", "filler")) return -2;

    //O(V^2)
    for (auto& v : getStationSet()) {
        if (v != target && v->getAdj().size() == 1) {
            addBidirectionalLine("super source", v->getName(), INT32_MAX, "filler");
        }
    }

    //O(VE^2)
    double flow = maxFlow("super source", target->getName());

    auto supersource = findStation("super source");

    removeStation(supersource);

    return flow;
}

double Graph::maxFlowSubGraph(const std::vector<std::pair<std::string, std::string>> &linesToRemove, const std::string& origin, const std::string& dest) {
    std::vector<std::pair<Station*, Station*>> stations;
    std::vector<std::pair<std::pair<Station*, Station*>, std::pair<double, std::string>>> removedEdges;

    for (auto& name : linesToRemove) {
        Station* station1 = findStation(name.first);
        Station* station2 = findStation(name.second);
        if (station1 == nullptr || station2 == nullptr) return -2;
        stations.emplace_back(station1, station2);
    }

    for (auto& p : stations) {
        Edge* edge = p.first->removeAndStoreEdge(p.second);
        if (edge == nullptr) continue;
        removedEdges.push_back({{p.first, p.second}, {edge->getCapacity(), edge->getService()}});
        Edge* edge2 = p.second->removeAndStoreEdge(p.first);
        delete edge;
        delete edge2;
    }

    double flow = maxFlow(origin, dest);

    for (auto& p : removedEdges) {
        addBidirectionalLine(p.first.first->getName(), p.first.second->getName(), p.second.first, p.second.second);
    }

    return flow;
}

std::vector<std::vector<std::pair<Station*, double>>> Graph::topStationsAffected(const std::vector<std::pair<std::string, std::string>> &linesToRemove, const int n, bool& error) {
    std::map<std::pair<Station*,Station*>, double> map;
    std::vector<std::pair<Station*, Station*>> stations;
    std::vector<std::vector<std::pair<Station*, double>>> res;
    std::vector<std::pair<std::pair<Station*, Station*>, std::pair<double, std::string>>> removedEdges;

    for (auto& name : linesToRemove) {
        Station* station1 = findStation(name.first);
        Station* station2 = findStation(name.second);
        if (station1 == nullptr || station2 == nullptr) {
            error = true;
            return res;
        }
        stations.emplace_back(station1, station2);
    }

    for (int i = 0; i < getStationSet().size() - 1; i++) {
        Station* u = getStationSet().at(i);
        for (int j = i+1; j < getStationSet().size(); j++) {
            Station* v = getStationSet().at(j);
            double flow = maxFlow(u->getName(), v->getName());
            if (flow == -1 || flow == -2) continue;
            map.insert({{u, v}, flow});
        }
    }

    for (auto& p : stations) {
        std::vector<std::pair<Station*, double>> aux;

        //remove aresta do grafo
        Edge* edge = p.first->removeAndStoreEdge(p.second);
        if (edge == nullptr) continue;
        removedEdges.push_back({{p.first, p.second}, {edge->getCapacity(), edge->getService()}});
        Edge* edge2 = p.second->removeAndStoreEdge(p.first);
        delete edge;
        delete edge2;

        double flow = maxFlow(p.first->getName(), p.second->getName());
        if (flow == -1 || flow == -2) continue;
        double maximumFlow = map[{p.first, p.second}];

        //calcula stations mais afetadas e guarda em aux
        aux.emplace_back(p.first, std::abs(flow - maximumFlow));
        aux.emplace_back(p.second, std::abs(flow - maximumFlow));

        //ordena elementos
        std::sort(aux.begin(), aux.end(), [](std::pair<Station*, double>& p1, std::pair<Station*, double>& p2) {return p1.second > p2.second;});

        if (n < aux.size()) {
            std::vector<std::pair<Station*, double>> final(aux.begin(), aux.begin() + n);
            res.push_back(final);
        }
        else {
            res.push_back(aux);
        }

        for (auto& pair : removedEdges) {
            addBidirectionalLine(pair.first.first->getName(), pair.first.second->getName(), pair.second.first, pair.second.second);
        }

        aux.clear();
    }

    return res;
}

std::pair<double, double> Graph::maxFlowMinCost(const std::string &origin, const std::string &dest, std::string& service) {
    auto source = findStation(origin);
    auto target = findStation(dest);

    if (source == nullptr || target == nullptr || source == target) {
        return {-2, -2};
    }

    bool existsPath = false;
    int alfaPaths, standardPaths;
    double alfaCost, standardCost, standardTrains, alfaTrains;
    alfaCost = standardCost = INT_MAX;

    if (dfs(source->getName(), target->getName(), "ALFA PENDULAR")) {
        dijkstra(source, target, "ALFA PENDULAR");
        alfaTrains = calculateCost(source, target, alfaPaths);
        alfaCost = alfaTrains * ALFA_PENDULAR_COST * alfaPaths;
        existsPath = true;
    }

    if (dfs(source->getName(), target->getName(), "STANDARD")) {
        dijkstra(source, target, "STANDARD");
        standardTrains = calculateCost(source, target, standardPaths);
        standardCost = standardTrains * STANDARD_COST * standardPaths;
        existsPath = true;
    }

    if (!existsPath) return {-1, -1};

    if (alfaCost < standardCost) {
        service = "ALFA PENDULAR";
        return {alfaCost, alfaTrains};
    }
    else if (alfaCost == standardCost) {
        if (alfaTrains > standardTrains) {
            service = "ALFA PENDULAR";
            return {alfaCost, alfaTrains};
        }
    }
    service = "STANDARD";
    return {standardCost, standardTrains};
}

double Graph::calculateCost(Station *origin, Station *dest, int& nPath) const {
    double maxFlow, cost;
    nPath = 0;
    maxFlow = INT_MAX;
    auto e = dest->getPath();

    while (true) {

        if (e != nullptr) {
            cost = e->getCapacity();
            if (cost < maxFlow) maxFlow = cost;
            e = e->getOrigin()->getPath();
            nPath++;
        }
        else {
            break;
        }
    }
    return maxFlow;
}

void Graph::dijkstra(Station* origin, Station* dest, const std::string& service) {
    for (auto& s : getStationSet()) {
        s->setVisited(false);
        s->setPath(nullptr);
        s->setCost(INT32_MAX);
    }

    origin->setCost(0);

    MutablePriorityQueue<Station> q;

    for (auto& v : getStationSet()) {
        q.insert(v);
    }

    while (!q.empty()) {
        auto u = q.extractMin();
        u->setVisited(true);

        if (u == dest) return;

        for (auto& e : u->getAdj()) {
            if (e->getService().size() - 1 == service.size()) {
                Station* neighbor = e->getDest();
                if (!neighbor->isVisited() && e->getCapacity() + u->getCost() < neighbor->getCost()) {
                    neighbor->setPath(e);
                    neighbor->setCost(e->getCapacity() + u->getCost());
                    q.decreaseKey(neighbor);
                }
            }
        }
    }
}
