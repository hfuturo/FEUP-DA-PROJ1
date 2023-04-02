//
// Created by Henrique Silva on 19/03/2023.
//

#include "../include/StationEdge.h"

/************************* Station  **************************/

Station::Station(const std::string &name, const std::string &district, const std::string &municipality, const std::string &township, const std::string &line):
    name(name), district(district), municipality(municipality), township(township), line(line) {
    this->setVisited(false);
    this->setPath(nullptr);
}

std::string Station::getDistrict() const {
    return this->district;
}

std::string Station::getLine() const {
    return this->line;
}

std::string Station::getMunicipality() const {
    return this->municipality;
}

std::string Station::getName() const {
    return this->name;
}

std::string Station::getTownShip() const {
    return this->township;
}

std::vector<Edge *> Station::getAdj() const {
    return this->adj;
}

std::vector<Edge *> Station::getIncoming() const {
    return this->incoming;
}

bool Station::isVisited() const {
    return this->visited;
}

void Station::setVisited(bool vis) {
    this->visited = vis;
}

Edge *Station::getPath() const {
    return this->path;
}

void Station::setPath(Edge *path) {
    this->path = path;
}

Edge* Station::addLine(Station *dest, const int capacity, const std::string &service) {
    Edge* edge = new Edge(this, dest, capacity, service);
    adj.push_back(edge);
    dest->incoming.push_back(edge);
    return edge;
}

bool Station::removeEdge(std::string name) {
    bool removeEdge = false;
    auto it = adj.begin();
    while (it != adj.end()) {
        Edge* edge = *it;
        Station* s = edge->getDest();
        if (s->getName() == name) {
            it = adj.erase(it);
            deleteEdge(edge);
            removeEdge = true;
        }
        else {
            it++;
        }
    }
    return removeEdge;
}

void Station::deleteEdge(Edge *edge) {
    Station *dest = edge->getDest();
    auto it = dest->incoming.begin();
    while (it != dest->incoming.end()) {
        if ((*it)->getOrigin()->getName() == name) {
            it = dest->incoming.erase(it);
        }
        else {
            it++;
        }
    }
    delete edge;
}

/************************* Edge  **************************/

Edge::Edge(Station *origin, Station *dest, const int capacity, const std::string &service): origin(origin), dest(dest), capacity(capacity), service(service) {
    this->reverse = nullptr;
    this->flow = 0;
}

Station *Edge::getDest() const {
    return this->dest;
}

Station *Edge::getOrigin() const {
    return this->origin;
}

int Edge::getCapacity() const {
    return this->capacity;
}

std::string Edge::getService() const {
    return this->service;
}

void Edge::setReverse(Edge *reverse) {
    this->reverse = reverse;
}

Edge *Edge::getReverse() const {
    return this->reverse;
}

void Edge::setFlow(int flow) {
    this->flow = flow;
}

int Edge::getFlow() const {
    return this->flow;
}

