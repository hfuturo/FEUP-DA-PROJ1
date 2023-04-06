#include "../include/UserInterface.h"
#include "../include/Graph.h"
#include <iostream>

int main() {
 //   UserInterface ui{};
 //   ui.showMenu();

    Graph graph{};
    graph.addStation("A", "fill", "fill", "fill", "fill");
    graph.addStation("B", "fill", "fill", "fill", "fill");
    graph.addStation("C", "fill", "fill", "fill", "fill");
    graph.addStation("D", "fill", "fill", "fill", "fill");
    graph.addStation("E", "fill", "fill", "fill", "fill");
    //graph.addStation("6", "fill", "fill", "fill", "fill");

    graph.addBidirectionalLine("A", "B", 6, "STANDARD");
    graph.addBidirectionalLine("A", "D", 1, "STANDARD");
    graph.addBidirectionalLine("B", "D", 2, "STANDARD");
    graph.addBidirectionalLine("B", "E", 2, "STANDARD");
    graph.addBidirectionalLine("B", "C", 5, "STANDARD");
    graph.addBidirectionalLine("D", "E", 1, "STANDARD");
    graph.addBidirectionalLine("E", "C", 5, "ALFA PENDULAR");

    auto s = graph.findStation("A");
    auto t = graph.findStation("C");

/*    for (auto v : graph.getStationSet()) {
        std::cout << v->getName() << std::endl;
        auto edges = v->getAdj();
        for (auto edge : edges) {
            if (edge->getService() == "ALFA PENDULAR") std::cout << "\tALFA PENDULAR\n";
            if (edge->getService() == "STANDARD") std::cout << "\tSTANDARD\n";
        }
    } */
     graph.fill();
     //graph.maxFlowMinCost("Lisboa Oriente", "Pombal");

    for (auto v : graph.getStationSet()) {
        std::cout << v->getName() << std::endl;
        auto edges = v->getAdj();
        for (auto edge : edges) {
            if (edge->getService() == "ALFA PENDULAR") std::cout << "\tALFA PENDULAR\n";
            if (edge->getService() == "STANDARD") std::cout << "\tSTANDARD\n";
        }
    }



   /* auto t = graph.findStation("Pombal");

    auto e = t->getPath();
    while (true) {
        if (e != nullptr) {
            std::cout << "ENTRA NAO NULL";
            std::cout << e->getDest()->getName() << " PATH " << e->getOrigin()->getName() << std::endl;
            e = e->getOrigin()->getPath();
        }
        else {
            std::cout << "ENTRA NULL";
            break;
        }
    } */
    return 0;
}