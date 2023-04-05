#include "../include/UserInterface.h"
#include "../include/Graph.h"
#include <iostream>

int main() {
//    UserInterface ui{};
//    ui.showMenu();

    Graph graph{};
    graph.addStation("A", "fill", "fill", "fill", "fill");
    graph.addStation("B", "fill", "fill", "fill", "fill");
    graph.addStation("C", "fill", "fill", "fill", "fill");
    graph.addStation("D", "fill", "fill", "fill", "fill");
    graph.addStation("E", "fill", "fill", "fill", "fill");
    //graph.addStation("6", "fill", "fill", "fill", "fill");

    graph.addBidirectionalLine("A", "B", 6, "s");
    graph.addBidirectionalLine("A", "D", 1, "s");
    graph.addBidirectionalLine("B", "D", 2, "s");
    graph.addBidirectionalLine("B", "E", 2, "s");
    graph.addBidirectionalLine("B", "C", 5, "s");
    graph.addBidirectionalLine("D", "E", 1, "s");
    graph.addBidirectionalLine("E", "C", 5, "s");

    auto s = graph.findStation("A");
    auto t = graph.findStation("C");

    graph.dijkstra(s, t, "s");

    auto e = t->getPath();
    while (true) {
        if (e != nullptr) {
            std::cout << e->getDest()->getName() << " PATH " << e->getOrigin()->getName() << std::endl;
            e = e->getOrigin()->getPath();
        }
        else {
            break;
        }
    }
    return 0;
}