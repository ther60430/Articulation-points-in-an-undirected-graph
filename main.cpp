#include"header.h"
int main() {
    system("chcp 65001 > nul");
    cout << "input method:\n";
    cout << "1:create graph by hand\n";
    cout << "2:create random graph\n";
    cout << "3:create graph from file\n";
    int method;
    cin >> method;
    adj_graph *graph=InitializeGraph(method);

    GraphVisualizer visualizer(graph);
    
    visualizer.run();

    delete graph;

    return 0;
}