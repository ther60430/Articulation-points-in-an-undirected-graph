#include"header.h"
int main() {
    system("chcp 65001 > nul");
    while (true)
    {
        cout << "input method:\n";
        cout << "1:create graph by hand\n";
        cout << "2:create random graph\n";
        cout << "3:create graph from file\n";
        cout << "4.Esc\n";
		cout << "Your choice (1-4): ";
        int method;
        cin >> method;
        if(!method || method < 1 || method > 4) {
            cout << "Invalid input! Please input again (1-4): ";
            cin.clear();
            cin.ignore(25565, '\n');
            continue;
        }
        if(method == 4) {
            break;
		}
        adj_graph* graph = InitializeGraph(method);
        GraphVisualizer visualizer(graph);
        visualizer.run();
        graph->~adj_graph();
        graph = nullptr;
    }
}