#include <iostream>
#include <string>
#include <cctype>

#include "../include/UserInterface.h"
#include "../include/Graph.h"

//TODO: nao esquecer de inicializar grafo no inicio se for sempre igual, tendo em conta alfa pendular e standard
void UserInterface::showMenu() {
    Graph graph{};
    graph.fill();
    bool done = false;
    char userchoice;

    while (!done) {
        std::cout << "======================================" << std::endl;
        std::cout << "\t\t\t\t Menu \t" << std::endl;
        std::cout << "======================================" << std::endl;
        std::cout << "(1) Gestao do numero de comboios." << std::endl;
        std::cout << "Enter 'Q' or 'q' to quit." << std::endl;

        std::cin >> userchoice;
        std::cout << std::endl;

        switch (userchoice) {
            case 'q':
            case 'Q': done = true; break;

            case '1': {
                char userchoice2;

                std::cout << "(1) Maximum number of trains between two stations." << std::endl;
                std::cout << "(2) See what stations require the most amount of trains." << std::endl;
                std::cout << "(3) See which locations (district or municipality) require larger budgets." << std::endl;
                std::cout << "(4) See how many trains can simultaneously arrive at a station (using entire railway grid)." << std::endl;

                std::cin >> userchoice2;
                std::cout << std::endl;


                switch (userchoice2) {
                    case '1': {
                        std::string station1, station2;
                        std::cout << "** Maximum number of trains between two stations **" << std::endl << std::endl << std::endl;
                        std::cout << "Enter the name of the first station: ";
                        std::cin.ignore();
                        std::getline(std::cin, station1);
                        std::cout << "Enter the name of the second station: ";
                        std::getline(std::cin, station2);
                        double flow = graph.maxFlow(station1, station2);
                        if (flow == -2) std::cout << "At least one of the stations does not exist. Make sure both stations are different and exist." << std::endl;
                        else if (flow == -1) std::cout << "There is no path between " << station1 << " to " << station2 << std::endl;
                        else std::cout << "From " << station1 << " to " << station2 << " can travel " << flow << " trains" << std::endl;
                        break;
                    }

                    case '2': {
                        std::cout << "** See what stations require the most amount of trains **" << std::endl << std::endl << std::endl;
                        std::vector<std::pair<double, std::pair<std::string, std::string>>> res = graph.fullMaxFlow();
                        std::cout << "The stations that require the most amount of trains when taking full advantage of the network capacity are: " << std::endl;
                        for (auto p: res) {
                            std::cout << "\t" << p.second.first << " to " << p.second.second << std::endl;
                        }
                        break;
                    }

                    case '3': {
                        int options;
                        char location;
                        std::vector<std::pair<std::string, double>> res;
                        std::cout << "** See which locations (district or municipality) require larger budgets **" << std::endl << std::endl;
                        std::cout << "How many locations do you want to see? ";
                        while (true) {
                            std::cin >> options;
                            if (options > 0) break;
                            std::cout << "The input must be a number and higher than 0." << std::endl;
                            std::cin.clear();
                            std::cin.ignore();
                        }

                        std::cout << "Do you want to see districts (press 1) or municipalities (press 2)? ";

                        while (true) {
                            std::cin >> location;
                            if (std::isdigit(location)) break;
                            std::cout << "Invalid Input. The input must be a number between 1 and 2." << std::endl;
                        }

                        switch (location) {

                            case '1': {
                                res = graph.topDistricts(options);
                                break;
                            }

                            case '2': {
                                res = graph.topMunicipalities(options);
                                break;
                            }
                        }

                        std::cout << "The top " << options << " ";
                        if (location == '1') {
                            std::cout << "Districts";
                        }
                        else {
                            std::cout << "Municipalities";
                        }
                        std::cout << " that require a larger budget are: " << std::endl;

                        for (auto v : res) {
                            std::cout << "\t" <<v.first << std::endl;
                        }

                        break;
                    }

                    case '4': {
                        std::string station;
                        std::cout << "** See how many trains can simultaneously arrive at a station (using entire railway grid) **" << std::endl;
                        std::cin.ignore();
                        std::getline(std::cin, station);
                        if (station.empty()) {
                            std::cout << "Invalid Input. The station can not be empty." << std::endl;
                            break;
                        }
                        double flow = graph.maxFlowGridToStation(station);

                        std::cout << "The maximum number of trains that can simultaneously arrive at " << station << " is " << flow << std::endl;

                        break;
                    }
                }
            }
        }
    }
}
