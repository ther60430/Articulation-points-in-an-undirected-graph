#include"header.h"
int main() {
    system("chcp 65001 > nul");

<<<<<<< HEAD
    adj_graph* graph = createRandomGraph();
=======
    adj_graph* graph = createGraphfromfile("text.txt");
>>>>>>> cf42c66daedb979cd15123f73630574a24b793f2

    GraphVisualizer visualizer(graph);
    visualizer.run();

    delete graph;

    return 0;
}