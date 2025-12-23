#pragma once

#include <graphics.h>
#include <conio.h>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <algorithm>
using namespace std;

// 颜色定义
const COLORREF BACKGROUND_COLOR = RGB(240, 245, 250);
const COLORREF NODE_COLOR = RGB(70, 130, 180);
const COLORREF NODE_HIGHLIGHT = RGB(220, 20, 60);
const COLORREF EDGE_COLOR = RGB(105, 105, 105);
const COLORREF CUT_NODE_COLOR = RGB(255, 69, 0);
const COLORREF TEXT_COLOR = RGB(50, 50, 50);
const COLORREF INFO_BG_COLOR = RGB(255, 250, 240);
const COLORREF BUTTON_COLOR = RGB(100, 149, 237);
const COLORREF BUTTON_HOVER_COLOR = RGB(65, 105, 225);
const COLORREF BUTTON_DISABLED_COLOR = RGB(180, 180, 180);
const COLORREF TRANSFORM_NODE_COLOR = RGB(50, 205, 50);

class EdgeNode
{
public:
    int adjvex;                              //邻接点
    int weight;                              //权值
    EdgeNode* next;
    EdgeNode(int adjvex, int weight) :adjvex(adjvex), weight(weight), next(nullptr) {}

};
class Vertex
{
public:
    EdgeNode* firstnode;
    Vertex() :firstnode(nullptr) {}
    ~Vertex()
    {
        EdgeNode* cur = firstnode;
        while (cur != nullptr)
        {
            EdgeNode* temp = cur;
            cur = cur->next;
            delete temp;
        }
        firstnode = nullptr;
    }
    void deepCopy(const Vertex& other) {
        EdgeNode* cur = firstnode;
        while (cur != nullptr) {
            EdgeNode* temp = cur;
            cur = cur->next;
            delete temp;
        }
        firstnode = nullptr;
        if (other.firstnode == nullptr) return;
        firstnode = new EdgeNode(other.firstnode->adjvex, other.firstnode->weight);
        EdgeNode* newCur = firstnode;
        EdgeNode* otherCur = other.firstnode->next;
        while (otherCur != nullptr) {
            newCur->next = new EdgeNode(otherCur->adjvex, otherCur->weight);
            newCur = newCur->next;
            otherCur = otherCur->next;
        }
    }
};
class adj_graph
{
public:
    int vertexnum;
    Vertex* adjlist;
    int time;
    int* dfn, * low, * parent;
    bool* is_cut_vertex, * visited;

    adj_graph(int num)
    {
        if (num < 0)
        {
            cout << "Vertex number must be non-negative.";
            return;
        }
        else
        {
            time = 0;
            vertexnum = num;
            adjlist = new Vertex[vertexnum];
            dfn = new int[vertexnum];
            low = new int[vertexnum];
            parent = new int[vertexnum];
            is_cut_vertex = new bool[vertexnum];
            visited = new bool[vertexnum];
            for (int i = 0; i < vertexnum; i++)
            {
                dfn[i] = 0;
                low[i] = 0;
                parent[i] = -1;
                is_cut_vertex[i] = false;
                visited[i] = false;
            }
        }
    }

    // 拷贝构造函数
    adj_graph(const adj_graph& other) {
        vertexnum = other.vertexnum;
        time = other.time;

        adjlist = new Vertex[vertexnum];
        dfn = new int[vertexnum];
        low = new int[vertexnum];
        parent = new int[vertexnum];
        is_cut_vertex = new bool[vertexnum];
        visited = new bool[vertexnum];

        for (int i = 0; i < vertexnum; i++) {
            adjlist[i].deepCopy(other.adjlist[i]);
            dfn[i] = other.dfn[i];
            low[i] = other.low[i];
            parent[i] = other.parent[i];
            is_cut_vertex[i] = other.is_cut_vertex[i];
            visited[i] = other.visited[i];
        }
    }

    ~adj_graph()
    {
        delete[] adjlist;
        delete[] dfn;
        delete[] low;
        delete[] is_cut_vertex;
        delete[] visited;
        delete[] parent;
    }

    bool is_exist(int src, int dest)
    {
        EdgeNode* cur = adjlist[src].firstnode;
        while (cur != nullptr)
        {
            if (cur->adjvex == dest)
                return true;
            cur = cur->next;
        }
        return false;
    }

    void add_edge(int src, int dest, int weight)
    {
        if (src < 0 || dest < 0 || weight < 0 || src >= vertexnum || dest >= vertexnum)
        {
            return;
        }
        else if (!is_exist(src, dest))
        {
            // 添加src->dest的边
            EdgeNode* newnode = new EdgeNode(dest, weight);
            newnode->next = adjlist[src].firstnode;
            adjlist[src].firstnode = newnode;

            // 添加dest->src的边（无向图）
            newnode = new EdgeNode(src, weight);
            newnode->next = adjlist[dest].firstnode;
            adjlist[dest].firstnode = newnode;
        }
        else
        {
            // 如果边已存在，更新权重为更小的值
            EdgeNode* cur = adjlist[src].firstnode;
            while (cur != nullptr)
            {
                if (cur->adjvex == dest)
                {
                    if (weight < cur->weight)
                    {
                        cur->weight = weight;
                    }
                    break;
                }
                cur = cur->next;
            }

            // 更新反向边
            cur = adjlist[dest].firstnode;
            while (cur != nullptr)
            {
                if (cur->adjvex == src)
                {
                    if (weight < cur->weight)
                    {
                        cur->weight = weight;
                    }
                    break;
                }
                cur = cur->next;
            }
        }
    }

    void reset_visited()
    {
        for (int i = 0; i < vertexnum; i++)
            visited[i] = false;
    }

    void DFS_cut_vertex(int u)
    {
        if (u < 0 || u >= vertexnum || adjlist == nullptr)
        {
            return;
        }
        int children = 0;
        dfn[u] = low[u] = ++time;
        visited[u] = true;
        EdgeNode* cur = adjlist[u].firstnode;
        while (cur)
        {
            int v = cur->adjvex;
            if (!visited[v])
            {
                children++;
                parent[v] = u;
                DFS_cut_vertex(v);
                low[u] = min(low[u], low[v]);
                if (low[v] >= dfn[u] && parent[u] != -1)
                {
                    is_cut_vertex[u] = true;
                }
                if (children > 1 && parent[u] == -1)
                {
                    is_cut_vertex[u] = true;
                }
            }
            else if (v != parent[u])
            {
                low[u] = min(low[u], dfn[v]);
            }
            cur = cur->next;
        }
    }

    void find_cut_vertex()
    {
        if (vertexnum == 0 || adjlist == nullptr || is_cut_vertex == nullptr)
        {
            return;
        }
        reset_visited();
        time = 0;
        for (int i = 0; i < vertexnum; i++)
        {
            is_cut_vertex[i] = false;
            parent[i] = -1;
        }
        for (int i = 0; i < vertexnum; i++)
        {
            if (!visited[i])
            {
                DFS_cut_vertex(i);
            }
        }
    }

    void addvertex() {
        if (vertexnum < 0) return;
        int newvertexnum = vertexnum + 1;
        Vertex* newadjlist = new Vertex[newvertexnum];
        int* newdfn = new int[newvertexnum];
        int* newlow = new int[newvertexnum];
        int* newparent = new int[newvertexnum];
        bool* newis_cut_vertex = new bool[newvertexnum];
        bool* newvisited = new bool[newvertexnum];

        for (int i = 0; i < vertexnum; i++) {
            newadjlist[i].deepCopy(adjlist[i]);
            newdfn[i] = dfn[i];
            newlow[i] = low[i];
            newparent[i] = parent[i];
            newis_cut_vertex[i] = is_cut_vertex[i];
            newvisited[i] = visited[i];
        }
        newdfn[newvertexnum - 1] = 0;
        newlow[newvertexnum - 1] = 0;
        newparent[newvertexnum - 1] = -1;
        newis_cut_vertex[newvertexnum - 1] = false;
        newvisited[newvertexnum - 1] = false;

        delete[] adjlist;
        delete[] dfn;
        delete[] low;
        delete[] parent;
        delete[] is_cut_vertex;
        delete[] visited;

        adjlist = newadjlist;
        dfn = newdfn;
        low = newlow;
        parent = newparent;
        is_cut_vertex = newis_cut_vertex;
        visited = newvisited;
        vertexnum = newvertexnum;
    }

    int get_cut_vertex_count() {
        int count = 0;
        for (int i = 0; i < vertexnum; i++) {
            if (is_cut_vertex[i]) count++;
        }
        return count;
    }

    int* get_cut_vertices() {
        int count = get_cut_vertex_count();
        int* cut_vertices = new int[count];
        int j = 0;
        for (int i = 0; i < vertexnum; i++) {
            if (is_cut_vertex[i]) {
                cut_vertices[j++] = i;
            }
        }
        return cut_vertices;
    }

    adj_graph& TCV_AddRedundantEdges(int* cut_vertex, int cut_vertex_number)
    {
        for (int c = 0; c < cut_vertex_number; c++)
        {
            int count = 0;
            EdgeNode* edge = adjlist[cut_vertex[c]].firstnode;
            while (edge != nullptr)
            {
                count++;
                edge = edge->next;
            }
            if (count < 2) continue;

            int* adj_vertex = new int[count];
            int k = 0;
            edge = adjlist[cut_vertex[c]].firstnode;
            while (edge != nullptr)
            {
                adj_vertex[k] = edge->adjvex;
                k++;
                edge = edge->next;
            }
            for (int i = 0; i < count - 1; i++)
            {
                add_edge(adj_vertex[i], adj_vertex[i + 1], 1);
            }
            add_edge(adj_vertex[count - 1], adj_vertex[0], 1);
            delete[] adj_vertex;
        }
        return *this;
    }

    adj_graph& TCV_CopyNode(int* cut_vertex, int cut_vertex_number)
    {
        for (int c = 0; c < cut_vertex_number; c++)
        {
            int count = 0;
            EdgeNode* edge = adjlist[cut_vertex[c]].firstnode;
            while (edge != nullptr)
            {
                count++;
                edge = edge->next;
            }
            int* adj_vertex = new int[count];
            int k = 0;
            edge = adjlist[cut_vertex[c]].firstnode;
            while (edge != nullptr)
            {
                adj_vertex[k] = edge->adjvex;
                k++;
                edge = edge->next;
            }
            addvertex();
            int new_vertex = vertexnum - 1;
            add_edge(cut_vertex[c], new_vertex, 1);
            for (int i = 0; i < count; i++)
            {
                add_edge(adj_vertex[i], new_vertex, 1);
            }
            delete[] adj_vertex;
        }
        return *this;
    }

    adj_graph& transform_cut_vertex(int method)
    {
        find_cut_vertex();

        int count = get_cut_vertex_count();
        cout << "Found " << count << " cut vertices to transform" << endl;

        if (count == 0) {
            cout << "No cut vertices to transform" << endl;
            return *this;
        }

        int* cut_vertex = get_cut_vertices();

        cout << "Cut vertices: ";
        for (int i = 0; i < count; i++) {
            cout << cut_vertex[i] << " ";
        }
        cout << endl;

        switch (method)
        {
        case 0:
            cout << "Applying redundant edges transformation..." << endl;
            TCV_AddRedundantEdges(cut_vertex, count);
            break;
        case 1:
            cout << "Applying node copy transformation..." << endl;
            TCV_CopyNode(cut_vertex, count);
            break;
        default:
            break;
        }

        delete[] cut_vertex;

        find_cut_vertex();
        cout << "After transformation, cut vertices: " << get_cut_vertex_count() << endl;

        return *this;
    }
};

wstring intToWString(int value) {
    return to_wstring(value);
}

class GraphVisualizer {
private:
    adj_graph* graph;
    adj_graph* originalGraph;
    int windowWidth;
    int windowHeight;
    vector<POINT> nodePositions;
    bool showCutVertices;
    bool showDFSInfo;
    bool isTransformed;
    int transformMethod;

    // 按钮交互相关
    struct ButtonInfo { int x, y, w, h; wstring text; bool hover; bool disabled; };
    vector<ButtonInfo> buttons;

public:
    GraphVisualizer(adj_graph* g) : graph(g), showCutVertices(true), showDFSInfo(false),
        isTransformed(false), transformMethod(0) {
        windowWidth = 1200;
        windowHeight = 800;
        initgraph(windowWidth, windowHeight);
        setbkcolor(BACKGROUND_COLOR);
        cleardevice();

        originalGraph = new adj_graph(*g);

        calculateNodePositions();

        // 初始化按钮
        prepareButtons();
    }

    ~GraphVisualizer() {
        closegraph();
        delete originalGraph;
    }

    void calculateNodePositions() {
        nodePositions.clear();
        int centerX = windowWidth / 2;
        int centerY = windowHeight / 2;
        int radius = min(centerX, centerY) - 150;

        for (int i = 0; i < graph->vertexnum; i++) {
            double angle = 2 * 3.1415926 * i / graph->vertexnum;
            int x = centerX + radius * cos(angle);
            int y = centerY + radius * sin(angle);
            nodePositions.push_back({ x, y });
        }
    }

    void drawNode(int nodeId, bool isCutVertex = false) {
        if (nodeId >= nodePositions.size()) return;

        POINT pos = nodePositions[nodeId];
        int radius = 35;

        if (isCutVertex && showCutVertices) {
            setfillcolor(CUT_NODE_COLOR);
            setlinecolor(CUT_NODE_COLOR);
        }
        else if (isTransformed && nodeId >= originalGraph->vertexnum) {
            setfillcolor(TRANSFORM_NODE_COLOR);
            setlinecolor(TRANSFORM_NODE_COLOR);
        }
        else {
            setfillcolor(NODE_COLOR);
            setlinecolor(NODE_COLOR);
        }

        fillcircle(pos.x, pos.y, radius);

        setlinecolor(WHITE);
        circle(pos.x, pos.y, radius);

        settextcolor(WHITE);
        settextstyle(20, 0, _T("Arial"));
        setbkmode(TRANSPARENT);

        wstring label = intToWString(nodeId);

        int textWidth = textwidth(label.c_str()) / 2;
        int textHeight = textheight(label.c_str()) / 2;
        outtextxy(pos.x - textWidth, pos.y - textHeight, label.c_str());

        if (showDFSInfo && nodeId < graph->vertexnum) {
            settextcolor(TEXT_COLOR);
            settextstyle(14, 0, _T("Arial"));

            wstring info = L"[" + intToWString(graph->dfn[nodeId]) +
                L"," + intToWString(graph->low[nodeId]) + L"]";

            int infoWidth = textwidth(info.c_str()) / 2;
            outtextxy(pos.x - infoWidth, pos.y + radius + 5, info.c_str());
        }
    }

    void drawEdge(int src, int dest, int weight) {
        if (src >= nodePositions.size() || dest >= nodePositions.size()) return;

        POINT srcPos = nodePositions[src];
        POINT destPos = nodePositions[dest];

        double dx = destPos.x - srcPos.x;
        double dy = destPos.y - srcPos.y;
        double length = sqrt(dx * dx + dy * dy);
        if (length == 0) return;

        dx /= length;
        dy /= length;

        int startX = srcPos.x + dx * 35;
        int startY = srcPos.y + dy * 35;
        int endX = destPos.x - dx * 35;
        int endY = destPos.y - dy * 35;

        setlinecolor(EDGE_COLOR);
        setlinestyle(PS_SOLID, 2);
        line(startX, startY, endX, endY);

        if (weight != 1) {
            int midX = (startX + endX) / 2;
            int midY = (startY + endY) / 2;

            int offsetX = -dy * 20;
            int offsetY = dx * 20;

            setfillcolor(INFO_BG_COLOR);
            setlinecolor(EDGE_COLOR);
            fillcircle(midX + offsetX, midY + offsetY, 15);

            settextcolor(TEXT_COLOR);
            settextstyle(12, 0, _T("Arial"));

            wstring weightStr = intToWString(weight);

            int textWidth = textwidth(weightStr.c_str()) / 2;
            int textHeight = textheight(weightStr.c_str()) / 2;
            outtextxy(midX + offsetX - textWidth, midY + offsetY - textHeight, weightStr.c_str());
        }

        setlinestyle(PS_SOLID, 1);
    }

    void drawInfoPanel() {
        setfillcolor(INFO_BG_COLOR);
        setlinecolor(RGB(200, 200, 200));
        fillrectangle(windowWidth - 300, 20, windowWidth - 20, windowHeight - 150);

        settextcolor(TEXT_COLOR);
        settextstyle(24, 0, _T("Arial"));
        outtextxy(windowWidth - 280, 40, L"Graph Information");

        settextstyle(16, 0, _T("Arial"));
        int y = 90;

        wstring info = L"Vertices: " + intToWString(graph->vertexnum);
        outtextxy(windowWidth - 280, y, info.c_str());

        int edgeCount = 0;
        for (int i = 0; i < graph->vertexnum; i++) {
            EdgeNode* cur = graph->adjlist[i].firstnode;
            while (cur != nullptr) {
                if (i <= cur->adjvex) edgeCount++;
                cur = cur->next;
            }
        }

        y += 40;
        info = L"Edges: " + intToWString(edgeCount);
        outtextxy(windowWidth - 280, y, info.c_str());

        y += 40;
        info = isTransformed ? L"Status: Transformed" : L"Status: Original";
        outtextxy(windowWidth - 280, y, info.c_str());

        y += 60;
        outtextxy(windowWidth - 280, y, L"Cut Vertices:");

        y += 30;
        bool hasCutVertices = false;
        for (int i = 0; i < graph->vertexnum; i++) {
            if (graph->is_cut_vertex[i]) {
                hasCutVertices = true;
                info = L"  Vertex " + intToWString(i);
                outtextxy(windowWidth - 260, y, info.c_str());
                y += 25;
            }
        }

        if (!hasCutVertices) {
            outtextxy(windowWidth - 260, y, L"  None");
        }
    }

    // 将按钮布局与文本统一准备
    void prepareButtons() {
        buttons.clear();
        int buttonY = windowHeight - 120;
        int buttonWidth = 180;
        int buttonHeight = 40;
        int spacing = 20;

        int x1 = 50;
        int x2 = x1 + buttonWidth + spacing;
        int x3 = x2 + buttonWidth + spacing;
        int x4 = x3 + buttonWidth + spacing;
        int x5 = x4 + buttonWidth + spacing;

        buttons.resize(5);
        buttons[0] = { x1, buttonY, buttonWidth, buttonHeight, showCutVertices ? L"Hide Cut Vertices" : L"Show Cut Vertices", false, false };
        buttons[1] = { x2, buttonY, buttonWidth, buttonHeight, showDFSInfo ? L"Hide DFS Info" : L"Show DFS Info", false, false };
        buttons[2] = { x3, buttonY, buttonWidth, buttonHeight, L"Find Cut Vertices", false, false };
        buttons[3] = { x4, buttonY, buttonWidth, buttonHeight, isTransformed ? L"Reset Graph" : L"Add Redundant Edges", false, false };
        // Copy Nodes 在已转换时禁用
        buttons[4] = { x5, buttonY, buttonWidth, buttonHeight, L"Copy Nodes", false, isTransformed };
    }

    // 绘制单个按钮，不再在这里读取鼠标事件
    void drawButton(const ButtonInfo& b) {
        if (b.disabled) {
            setfillcolor(BUTTON_DISABLED_COLOR);
        }
        else if (b.hover) {
            setfillcolor(BUTTON_HOVER_COLOR);
        }
        else {
            setfillcolor(BUTTON_COLOR);
        }

        setlinecolor(RGB(50, 50, 50));
        fillroundrect(b.x, b.y, b.x + b.w, b.y + b.h, 10, 10);

        settextcolor(WHITE);
        settextstyle(18, 0, _T("Arial"));
        int textWidth = textwidth(b.text.c_str());
        int textHeight = textheight(b.text.c_str());
        outtextxy(b.x + (b.w - textWidth) / 2, b.y + (b.h - textHeight) / 2, b.text.c_str());
    }

    // 判断点是否在按钮区域内
    static bool pointInRect(int px, int py, const ButtonInfo& b) {
        return (px >= b.x && px <= b.x + b.w && py >= b.y && py <= b.y + b.h);
    }

    // 仅在这里读取并处理鼠标消息，避免重复消费
    void processMouseInput() {
        bool anyMouse = false;
        MOUSEMSG m;
        // 读取所有可用的鼠标消息，保留最后一条用于 hover / click 处理
        MOUSEMSG lastMsg = { 0 };
        while (MouseHit()) {
            m = GetMouseMsg();
            lastMsg = m;
            anyMouse = true;
        }

        // 如果没有鼠标消息则不修改 hover（避免消耗其他地方的事件）
        if (!anyMouse) return;

        // 更新 hover 状态（基于最后一条消息的位置）
        int mx = lastMsg.x;
        int my = lastMsg.y;
        for (auto& btn : buttons) {
            btn.hover = (!btn.disabled) && pointInRect(mx, my, btn);
        }

        // 处理点击：只在最后消息为左键按下时触发（避免多次触发）
        if (lastMsg.uMsg == WM_LBUTTONDOWN) {
            // 按钮 0: 显示/隐藏割点
            if (pointInRect(mx, my, buttons[0])) {
                showCutVertices = !showCutVertices;
                cout << "Toggled cut vertices display: " << (showCutVertices ? "ON" : "OFF") << endl;
            }
            // 按钮 1: 显示/隐藏 DFS 信息
            else if (pointInRect(mx, my, buttons[1])) {
                showDFSInfo = !showDFSInfo;
                cout << "Toggled DFS info display: " << (showDFSInfo ? "ON" : "OFF") << endl;
            }
            // 按钮 2: 计算割点
            else if (pointInRect(mx, my, buttons[2])) {
                cout << "Finding cut vertices..." << endl;
                graph->find_cut_vertex();
                int cutCount = graph->get_cut_vertex_count();
                cout << "Found " << cutCount << " cut vertices" << endl;
            }
            // 按钮 3: 添加冗余边或重置
            else if (pointInRect(mx, my, buttons[3])) {
                if (!isTransformed) {
                    cout << "Adding redundant edges..." << endl;
                    adj_graph* tempGraph = new adj_graph(*graph);
                    tempGraph->transform_cut_vertex(0);

                    delete graph;
                    graph = tempGraph;
                    isTransformed = true;
                    transformMethod = 0;
                    cout << "Redundant edges added successfully!" << endl;
                }
                else {
                    cout << "Resetting graph..." << endl;
                    delete graph;
                    graph = new adj_graph(*originalGraph);
                    isTransformed = false;
                    cout << "Graph reset successfully!" << endl;
                }
                calculateNodePositions();
            }
            // 按钮 4: 复制节点（仅当未转换且按钮未禁用时）
            else if (pointInRect(mx, my, buttons[4]) && !buttons[4].disabled) {
                cout << "Copying nodes..." << endl;
                adj_graph* tempGraph = new adj_graph(*graph);
                tempGraph->transform_cut_vertex(1);

                delete graph;
                graph = tempGraph;
                isTransformed = true;
                transformMethod = 1;
                cout << "Nodes copied successfully!" << endl;
                calculateNodePositions();
            }

            // 点击后更新按钮文本和可用性
            prepareButtons();
        }
    }

    void drawControls() {
        // 每次绘制前确保按钮文本和位置同步
        prepareButtons();
        for (const auto& b : buttons) {
            drawButton(b);
        }
    }

    void render() {
        cleardevice();

        // 绘制所有边
        for (int i = 0; i < graph->vertexnum; i++) {
            EdgeNode* cur = graph->adjlist[i].firstnode;
            while (cur != nullptr) {
                if (i <= cur->adjvex) {
                    drawEdge(i, cur->adjvex, cur->weight);
                }
                cur = cur->next;
            }
        }

        // 绘制所有节点
        for (int i = 0; i < graph->vertexnum; i++) {
            drawNode(i, graph->is_cut_vertex[i]);
        }

        drawInfoPanel();
        drawControls();
        processMouseInput();  // 处理鼠标输入（现在只在此处读取鼠标事件）

        settextcolor(TEXT_COLOR);
        settextstyle(32, 0, _T("Arial"));
        outtextxy(50, 30, L"Graph Visualization");
        settextstyle(16, 0, _T("Arial"));
        wstring subtitle = isTransformed ?
            (transformMethod == 0 ? L"With Redundant Edges Transformation" : L"With Node Copy Transformation") :
            L"Adjacency List with Cut Vertex Detection";
        outtextxy(50, 80, subtitle.c_str());

        FlushBatchDraw();
    }

    void run() {
        BeginBatchDraw();

        // 初始计算割点
        graph->find_cut_vertex();
        cout << "Initial cut vertices: " << graph->get_cut_vertex_count() << endl;

        while (true) {
            render();

            if (_kbhit()) {
                int key = _getch();
                if (key == 27) {
                    break;
                }
                else if (key == 'c' || key == 'C') {
                    showCutVertices = !showCutVertices;
                    cout << "Toggled cut vertices display: " << (showCutVertices ? "ON" : "OFF") << endl;
                    prepareButtons();
                }
                else if (key == 'd' || key == 'D') {
                    showDFSInfo = !showDFSInfo;
                    cout << "Toggled DFS info display: " << (showDFSInfo ? "ON" : "OFF") << endl;
                    prepareButtons();
                }
                else if (key == 'r' || key == 'R') {
                    calculateNodePositions();
                    cout << "Recalculated node positions" << endl;
                }
                else if (key == 'f' || key == 'F') {
                    graph->find_cut_vertex();
                    cout << "Recalculated cut vertices" << endl;
                }
            }

            Sleep(50);
        }

        EndBatchDraw();
    }
};

adj_graph* createExampleGraph() {
    adj_graph* graph = new adj_graph(8);

    graph->add_edge(0, 1, 4);
    graph->add_edge(0, 2, 3);
    graph->add_edge(1, 2, 5);
    graph->add_edge(1, 3, 2);
    graph->add_edge(2, 3, 7);
    graph->add_edge(3, 4, 1);
    graph->add_edge(4, 5, 6);
    graph->add_edge(4, 6, 3);
    graph->add_edge(5, 6, 4);
    graph->add_edge(6, 7, 2);
    graph->add_edge(3, 7, 8);

    return graph;
}

