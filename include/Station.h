//
// Created by Henrique Silva on 13/03/2023.
//

#ifndef DA_PROJ1_STATION_H
#define DA_PROJ1_STATION_H

#include <string>

class Station;

class Station {
    std::string name;
    std::string district;
    std::string municipality;
    std::string township;
    std::string line;

public:
    Station(std::string name, std::string district, std::string municipality, std::string township, std::string line);

    std::string getName() const;
    std::string getDistrict() const;
    std::string getMunicipality() const;
    std::string getTownShip() const;
    std::string getLine() const;

};
#endif //DA_PROJ1_STATION_H
