#include "../include/Graph.h"
#include <iostream>

int main() {
    Graph graph{};
    graph.fill();
   // graph.dfs("Porto Campanhã", "Valongo-Vouga");
    //std::cout << graph.maxFlow("Porto Campanhã", "Viana do Castelo") << std::endl;
    graph.fullMaxFlow();
    return 0;
}