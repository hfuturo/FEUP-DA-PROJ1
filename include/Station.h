#ifndef DA_PROJ1_STATION_H
#define DA_PROJ1_STATION_H

#include <string>
#include <vector>

#include "Network.h"

class Station;

class Station {
    std::string name;
    std::string district;
    std::string municipality;
    std::string township;
    std::string line;
    //std::vector<Network*> adj;
    //std::vector<Network*> incoming;
public:
    Station(const std::string& name, const std::string& district, const std::string& municipality, const std::string& township, const std::string& line);
    std::string getName() const;
    std::string getDistrict() const;
    std::string getMunicipality() const;
    std::string getTownShip() const;
    std::string getLine() const;
    void addLine(Station* dest, const int capacity, const std::string& service) const;
};
#endif //DA_PROJ1_STATION_H
