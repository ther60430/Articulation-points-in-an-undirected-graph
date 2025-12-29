#include"header.h"
int main() {
    system("chcp 65001 > nul");
    system("cls");

    while (true) {
        // 运行一级界面
        GraphMainUI mainUI;
        int method = mainUI.runMainUI();

        if (method == -1) { // 退出程序
            break;
        }
        if (method < 1 || method > 3) { // 无效选择
            continue;
        }

        // 初始化图，进入二级可视化界面
        adj_graph* graph = InitializeGraph(method);
        if (graph == nullptr) {
            cout << "Failed to create graph, return to main UI." << endl;
            continue;
        }

        GraphVisualizer visualizer(graph);
        int status = visualizer.run(); // 运行二级界面，获取状态

        // 注意：GraphVisualizer 的析构函数已负责释放 graph（拥有所有权），
        // 因此此处不再重复 delete，防止 double-delete 导致异常。

        if (status == -1) { // 二级界面选择退出程序
            break;
        }
        // 状态为2时，返回一级界面，继续循环
    }

    return 0;
}