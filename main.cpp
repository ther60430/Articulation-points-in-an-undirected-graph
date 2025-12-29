#include"header.h"
int main() {
    system("chcp 65001 > nul");
    system("cls");
    while (true) { // 循环运行一级界面，实现“一级→二级→一级”的切换
        // 1. 运行一级界面，获取用户选择
        GraphMainUI mainUI;
        int method = mainUI.runMainUI();

        // 2. 处理一级界面的选择结果
        if (method == -1) { // 一级界面选择“Exit Program”，退出整个程序
            break;
        }
        if (method < 1 || method > 3) { // 无效选择，重新运行一级界面
            continue;
        }

        // 3. 初始化图，进入二级可视化界面
        adj_graph* graph = InitializeGraph(method);
        if (graph == nullptr) { // 图创建失败，返回一级界面
            std::cout << "Failed to create graph, return to main UI." << endl;
            // 等待控制台输入提示，避免快速刷新
            system("pause > nul");
            continue;
        }

        // 4. 运行二级界面，获取返回状态
        GraphVisualizer visualizer(graph);
        int status = visualizer.run(); // 捕获二级界面返回状态（2=返回一级，-1=退出程序）

        // 释放资源
        delete graph;
        graph = nullptr;

        if (status == -1) { // 二级界面选择退出程序
            break;
        }
        // 状态为2时，返回一级界面，继续循环
    }

    cout << "Program exited normally." << endl;
    return 0;
}