#include "../include/Graph.h"
#include <iostream>

int main() {
    Graph graph{};
    graph.fill();
   // graph.dfs("Porto Campanhã", "Lisboa Oriente");
   graph.maxFlow("Porto Campanhã", "Lisboa Oriente");
    return 0;
}