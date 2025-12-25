#include"header.h"
int main() {
    system("chcp 65001 > nul");

    adj_graph* graph = createExampleGraph();

    GraphVisualizer visualizer(graph);
    visualizer.run();

    delete graph;

    return 0;
}