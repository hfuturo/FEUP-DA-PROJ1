#include <iostream>
#include <string>
#include <cctype>

#include "../include/UserInterface.h"
#include "../include/Graph.h"

void UserInterface::showMenu() {
    Graph graph{};
    graph.fill();
    bool done = false;
    char userchoice;

    while (!done) {
        std::cout << "======================================" << std::endl;
        std::cout << "\t\t\t\t Menu \t" << std::endl;
        std::cout << "======================================" << std::endl;
        std::cout << "(1) Basic Service Metrics" << std::endl;
        std::cout << "(2) Operation Cost Optimization" << std::endl;
        std::cout << "(3) Reliability and Sensitivity to Line Failures" << std::endl;
        std::cout << "(Q) Quit" << std::endl;

        std::cin >> userchoice;
        std::cout << std::endl;
        std::cin.ignore(); //testing this

        switch (userchoice) {
            case 'q':
            case 'Q': done = true; break;

            case '1': {
                char userchoice2;

                std::cout << "*** Basic Service Metrics ***\n\n";

                std::cout << "(1) Maximum number of trains between two stations." << std::endl;
                std::cout << "(2) See what stations require the most amount of trains." << std::endl;
                std::cout << "(3) See which locations (district or municipality) require larger budgets." << std::endl;
                std::cout << "(4) See how many trains can simultaneously arrive at a station (using entire railway grid)." << std::endl;

                std::cout << "\nInsert the option number you want to select." <<std::endl;
                std::cin >> userchoice2;
                std::cout << std::endl;

                switch (userchoice2) {
                    case '1': {
                        std::string station1, station2;
                        std::cout << "** Maximum number of trains between two stations **\n\n";
                        std::cout << "Enter the name of the first station: ";
                        std::cin.ignore();
                        std::getline(std::cin, station1);
                        std::cout << "Enter the name of the second station: ";
                        std::getline(std::cin, station2);
                        std::cout << std::endl;
                        double flow = graph.maxFlow(station1, station2);
                        if (flow == -2) std::cout << "Invalid Input. Make sure both stations exist and that they are different.\n\n";
                        else if (flow == -1) std::cout << "There is no path between " << station1 << " to " << station2 << std::endl << std::endl;
                        else std::cout << "From " << station1 << " to " << station2 << " can travel " << flow << " trains.\n\n";
                        break;
                    }

                    case '2': {
                        std::cout << "** See what stations require the most amount of trains **\n\n";
                        std::cout << "Loading...\n\n";
                        std::vector<std::pair<double, std::pair<std::string, std::string>>> res = graph.fullMaxFlow();
                        std::cout << "The stations that require the most amount of trains when taking full advantage of the network capacity are: " << std::endl;
                        for (auto& p: res) {
                            std::cout << "\t" << p.second.first << " to " << p.second.second << " with a capacity of " <<  p.first << std::endl;
                        }
                        std::cout << std::endl;
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

                        std::cout << std::endl;

                        std::cout << "Loading...\n\n";
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

                        for (int i = 0; i < res.size(); i++) {
                            std::cout << "(" << i+1 << ")\t";
                            if (i+1 < 10) std::cout << "\t";
                            std::cout << res.at(i).first << std::endl;
                        }

                        std::cout << std::endl;

                        break;
                    }

                    case '4': {
                        std::string station;
                        std::cout << "** See how many trains can simultaneously arrive at a station (using entire railway grid) **" << std::endl << std::endl;
                        std::cin.ignore();
                        std::cout << "Insert the name of the station: ";
                        while (true) {
                            std::getline(std::cin, station);
                            if (!station.empty()) {
                                break;
                            }
                            std::cout << "\nInvalid Input. The station can not be empty.\n" << std::endl;
                        }
                        std::cout << std::endl;
                        double flow = graph.maxFlowGridToStation(station);

                        if (flow == -1) {
                            std::cout << "Invalid input. Either one of the stations do not exist or both stations are the same.\n\n";
                            break;
                        }

                        std::cout << "\nThe maximum number of trains that can simultaneously arrive at " << station << " is " << flow << std::endl << std::endl;

                        break;
                    }

                    default: {
                        std::cout << "Invalid input.\n\n";
                    }
                }
                break;
            }

            case '2': {
                std::string origin, target, service;
                while (true) {
                    std::cout << "Insert the name of the first station: ";
                    while (true) {
                        std::getline(std::cin, origin);
                        if (!origin.empty()) break;
                        std::cout << "The station can not be empty." << std::endl;
                    }
                    std::cout << "Insert the name of the second station: ";
                    while(true) {
                        std::getline(std::cin, target);
                        if (!target.empty()) break;
                        std::cout << "The station can not be empty." << std::endl;
                    }
                    if (origin != target) break;
                }
                std::cout << std::endl;
                std::pair<double, double> res = graph.maxFlowMinCost(origin, target, service);
                if (res.first == -2 && res.second == -2) {
                    std::cout << "At least one of the stations do not exist.\n\n";
                    break;
                }
                if (res.first == -1 && res.second == -1) {
                    std::cout << "There is no path between " << origin << " and " << target << std::endl << std::endl;
                    break;
                }
                std::cout << "The maximum amount of trains that can simultaneously travel between " << origin << " and " << target << " is " << res.second;
                std::cout << " with a total cost of " << res.first << " using the " << service << " service\n\n";
                break;
            }

            case '3': {
                char userchoice2;
                std::vector<std::pair<std::string, std::string>> linesToBeRemoved;
                std::cout << "*** Reliability and Sensitivity to Line Failures ***\n\n";
                std::cout << "Enter the name of the stations you want the lines between them to be removed.\n";
                std::cout << "(You need to enter stations in PAIRS OF TWO!)\n\n";

                while (true) {
                    std::string s, t;
                    bool exit = false;
                    while (true) {
                        std::cout << "Enter the name of the first station. (Press 'Q' to exit.): ";
                        std::getline(std::cin, s);
                        if (s.empty()) {
                            std::cout << "\nThe station can not be empty.\n\n";
                            continue;
                        }
                        if (s.size() == 1 && (s.at(0) == 'q' || s.at(0) == 'Q')) {
                            exit = true;
                        }
                        break;
                    }

                    if (exit) {
                        if (linesToBeRemoved.empty()) {
                            std::cout << "\nYou need to enter at least one pair of stations.\n\n";
                            continue;
                        }
                        else
                            break;
                    }

                    while (true) {
                        std::cout << "Enter the name of the second station: ";
                        std::getline(std::cin, t);
                        if (t.empty()) {
                            std::cout << "\nThe station can not be empty.\n\n";
                            continue;
                        }
                        if (t.size() == 1 && (t.at(0) == 'q' || t.at(0) == 'Q')) {
                            std::cout << "\nYou can not leave. You need to enter one more station.\n";
                            std::cout << "Remember that you need to add stations in pairs of two.\n\n";
                            continue;
                        }
                        break;
                    }
                    std::cout << std::endl;
                    linesToBeRemoved.emplace_back(s,t);
                }

                std::cout << "\n(1) Calculate the maximum number of trains that can simultaneously travel between two specific stations in a network of reduced connectivity";
                std::cout << std::endl;
                std::cout << "(2) Stations most affected" << std::endl;

                std::cout << "\nInsert the option number of the option you want to select.\n";
                std::cin >> userchoice2;
                std::cin.ignore();
                std::cout << std::endl;
                switch (userchoice2) {
                    case '1': {
                        std::string origin, target;
                        std::cout << "** Maximum number of trains that can simultaneously travel between two specific stations in a network of reduced connectivity **\n";
                        std::cout << std::endl;

                        while (true) {
                            std::cout << "Insert the name of the first station: ";
                            std::getline(std::cin, origin);
                            if (origin.empty()) {
                                std::cout << "\nThe name can not be empty.\n\n";
                                continue;
                            }
                            break;
                        }

                        while (true) {
                            std::cout << "Insert the name of the second station: ";
                            std::getline(std::cin, target);
                            if (target.empty()) {
                                std::cout << "\nThe name can not be empty.\n\n";
                                continue;
                            }
                            break;
                        }

                        std::cout << std::endl;

                        double flow = graph.maxFlowSubGraph(linesToBeRemoved, origin, target);

                        if (flow == -1) {
                            std::cout << "There is no path between " << origin << " and " << target << std::endl << std::endl;
                            break;
                        }
                        if (flow == -2) {
                            std::cout << "Invalid input. Either one of the stations do not exist or both stations are the same.\n\n";
                            break;
                        }

                        std::cout << "The maximum number of trains that can simultaneously arrive at " << origin << " from " << target << " is " << flow << std::endl;
                        std::cout << std::endl;

                        break;
                    }

                    case '2': {
                        int n;
                        bool error;

                        while (true) {
                            std::cout << "Insert the number os stations: ";
                            std::cin >> n;
                            if (n > 0) {
                                break;
                            }
                            std::cout << "\nThe number of stations must be higher than 0\n\n";
                            std::cin.clear();
                            std::cin.ignore();
                        }

                        std::cout << "\nLoading...\n\n";

                        auto res = graph.topStationsAffected(linesToBeRemoved, n, error);

                        if (error) {
                            std::cout << "One or more stations that you provided do not exist.\n\n";
                            break;
                        }

                        if (res.empty()) {
                            std::cout << "No stations were affected.\n\n";
                            break;
                        }

                        int counter = 0;
                        for (auto& u : res) {
                            std::cout << "Line from " << linesToBeRemoved.at(counter).first << " to "
                                      << linesToBeRemoved.at(counter).second << " removed:" << std::endl;
                            if (u.empty()) {
                                std::cout << "\tNo stations were affected\n\n";
                                continue;
                            }
                            if (u.size() < n) {
                                std::cout << "\tOnly " << u.size() << " stations were affected.\n\n";
                            }
                            for (auto& v : u) {
                                std::cout << "\t" << "Station " << v.first->getName() << " got its flow affected by " << v.second << std::endl;
                            }
                            counter++;
                            std::cout << std::endl;
                        }
                        std::cout << std::endl;
                        break;
                    }
                }
                linesToBeRemoved.clear();
                break;
            }

            default: {
                std::cout << "Invalid input.\n\n";
            }
        }
        if (done) break;
        std::cout << "Enter a random key to continue.\n";
        char filler;
        std::cin >> filler;
    }
}
