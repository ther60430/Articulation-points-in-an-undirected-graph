#include"header.h"
<<<<<<< HEAD
int main()
{
=======
int main() {
    system("chcp 65001 > nul");

    adj_graph* graph = createExampleGraph();

    GraphVisualizer visualizer(graph);
    visualizer.run();

    delete graph;

    return 0;
>>>>>>> 8c539fcf625c1adf861dbd9ca31815d9bb2a8b75
}