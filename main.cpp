#include"header.h"
int main() {
    system("chcp 65001 > nul");
    system("cls");
    while (true) 
    {
        GraphMainUI mainUI;
        int method = mainUI.runMainUI();
        if (method == -1) 
        { 
            break;
        }
        if (method < 1 || method > 3) 
        { 
            continue;
        }
        adj_graph* graph = InitializeGraph(method);
        if (graph == nullptr) 
        { 
            std::cout << "Failed to create graph, return to main UI." << endl;
            system("pause > nul");
            continue;
        }
        GraphVisualizer visualizer(graph);
        visualizer.run();
    }
    cout << "Program exited normally." << endl;
    return 0;
}