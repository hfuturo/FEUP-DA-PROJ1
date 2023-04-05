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
        std::cout << "(2)" << std::endl;
        std::cout << "(3) Reliability and Sensitivity to Line Failures" << std::endl;
        std::cout << "Enter 'Q' or 'q' to quit." << std::endl;

        std::cin >> userchoice;
        std::cout << std::endl;
        std::cin.ignore(); //testing this

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
                break;
            }

            case '3': {
                char userchoice2;
                std::vector<std::pair<std::string, std::string>> linesToBeRemoved;

                std::cout << "Enter the name of the stations that you want the lines to be removed (Press 'Q' to exit)" << std::endl;

                while (true) {
                    std::string s, t;
                    std::getline(std::cin, s);
                    if (s.size() == 1 && (s.at(0) == 'q' || s.at(0) == 'Q')) break;
                    if (s.empty()) {
                        std::cout << "The name of the station can not be empty" << std::endl;
                        continue;
                    }

                    std::getline(std::cin, t);
                    if (t.size() == 1 && (t.at(0) == 'q' || t.at(0) == 'Q')) break;
                    linesToBeRemoved.emplace_back(s,t);
                }

                if (linesToBeRemoved.empty()) {
                    std::cout << "You need to give at least 2 stations." << std::endl;
                    break;
                }

                std::cout << "(1) Calculate the maximum number of trains that can simultaneously travel between two specific stations in a network of reduced connectivity";
                std::cout << std::endl;
                std::cout << "(2) Stations most affected" << std::endl;

                std::cin >> userchoice2;
                std::cin.ignore();

                switch (userchoice2) {
                    case '1': {
                        std::string origin, target;
                        std::cout << "** Maximum number of trains that can simultaneously travel between two specific stations in a network of reduced connectivity **";
                        std::cout << std::endl;
                        //std::cin.ignore();

                        std::cout << "Insert the name of the first station: ";
                        while (true) {
                            std::getline(std::cin, origin);
                            if (origin.empty()) {
                                std::cout << "The name can not be empty." << std::endl;
                                continue;
                            }
                            break;
                        }

                        std::cout << "Insert the name of the second station: ";
                        while (true) {
                            std::getline(std::cin, target);
                            if (target.empty()) {
                                std::cout << "The name can not be empty." << std::endl;
                                continue;
                            }
                            break;
                        }

                        double flow = graph.maxFlowSubGraph(linesToBeRemoved, origin, target);

                        if (flow == -1) {
                            std::cout << "There is no path between " << origin << " and " << target << std::endl;
                            break;
                        }
                        if (flow == -2) {
                            std::cout << "Invalid input. Either one of the stations do not exist or both stations are the same." << std::endl;
                        }

                        std::cout << "The maximum number of trains that can simultaneously arrive at " << origin << " from " << target << " is " << flow << std::endl;

                        break;
                    }

                    case '2': {
                        int n;
                        std::cout << "Insert the number os stations: ";

                        while (true) {
                            std::cin >> n;
                            if (n <= 0) {
                                std::cout << "The number must be higher than 0." << std::endl;
                                continue;
                            }
                            std::cin.clear();
                            std::cin.ignore();
                            break;
                        }
                        graph.topStationsAffected(linesToBeRemoved, 5);

                       // std::cout << graph.topStationsAffected(linesToBeRemoved, 5);
                    }
                }
                break;
            }
        }
    }
}
