//
// Created by Henrique Silva on 13/03/2023.
//

#include "../include/Station.h"

Station::Station(std::string name, std::string district, std::string municipality, std::string township,
                 std::string line) {

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