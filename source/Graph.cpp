#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

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
       // std::cout << origin << " " << dest << " " << capacity << " " << service << std::endl;
        addLine(origin, dest, stoi(capacity), service);
    }

    for (auto& s : getStationSet()) {
        auto e = s->getAdj();
        std::cout << s->getName() << " : " << std::endl;
        for (auto edge : e) {
            std::cout << "\t";
            std::cout << edge->getOrigin()->getName() << " -> " << edge->getDest()->getName() << " ( " << edge->getCapacity() << " ) " << edge->getService() << std::endl;
        }
    }

}

void Graph::fill() {
    readStations();
    readNetwork();
}
