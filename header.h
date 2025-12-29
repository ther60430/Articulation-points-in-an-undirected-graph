#pragma once
#include<fstream>
#include <graphics.h>
#include <conio.h>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <ctime>
using namespace std;

// 引入全局图形引用计数，防止多次 init/close 导致异常
static int s_graphicsRefCount = 0;
static void EnsureGraphicsInit(int width, int height)
{
    if (s_graphicsRefCount == 0) {
        initgraph(width, height);
    }
    ++s_graphicsRefCount;
}
static void EnsureGraphicsClose()
{
    if (s_graphicsRefCount <= 0) return;
    --s_graphicsRefCount;
    if (s_graphicsRefCount == 0) {
        closegraph();
    }
}

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
    EdgeNode* next;
    EdgeNode(int adjvex) :adjvex(adjvex), next(nullptr) {}
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
        firstnode = new EdgeNode(other.firstnode->adjvex);
        EdgeNode* newCur = firstnode;
        EdgeNode* otherCur = other.firstnode->next;
        while (otherCur != nullptr) {
            newCur->next = new EdgeNode(otherCur->adjvex);
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
        else if(num == 0)
        {
            vertexnum = 0;
            adjlist = nullptr;
            dfn = nullptr;
            low = nullptr;
            parent = nullptr;
            is_cut_vertex = nullptr;
            visited = nullptr;
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

    void add_edge(int src, int dest)
    {
        if (src < 0 || dest < 0 || src >= vertexnum || dest >= vertexnum)
        {
            return;
        }
        else if (!is_exist(src, dest))
        {
            // 添加src->dest的边
            EdgeNode* newnode = new EdgeNode(dest);
            newnode->next = adjlist[src].firstnode;
            adjlist[src].firstnode = newnode;
            // 添加dest->src的边（无向图）
            newnode = new EdgeNode(src);
            newnode->next = adjlist[dest].firstnode;
            adjlist[dest].firstnode = newnode;
        }
        else
        {
            return;
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
                add_edge(adj_vertex[i], adj_vertex[i + 1]);
            }
            add_edge(adj_vertex[count - 1], adj_vertex[0]);
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
            add_edge(cut_vertex[c], new_vertex);
            for (int i = 0; i < count; i++)
            {
                add_edge(adj_vertex[i], new_vertex);
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

    // 按钮交互相关（原有）
    struct ButtonInfo { int x, y, w, h; wstring text; bool hover; bool disabled; };
    vector<ButtonInfo> buttons;

    // 新增：返回一级界面的标识（无需额外成员，复用按钮数组即可）
public:
    GraphVisualizer(adj_graph* g) : graph(g), showCutVertices(true), showDFSInfo(false),
        isTransformed(false), transformMethod(0) {
        windowWidth = 1200;
        windowHeight = 800;
        // 使用引用计数保证 initgraph/closegraph 配对
        EnsureGraphicsInit(windowWidth, windowHeight);
        setbkcolor(BACKGROUND_COLOR);
        cleardevice();

        originalGraph = new adj_graph(*g);

        calculateNodePositions();

        // 初始化按钮（包含新增返回按钮）
        prepareButtons();
    }

    ~GraphVisualizer() {
        
        delete originalGraph;
        // 删除当前图对象，GraphVisualizer 对传入的 graph 拥有所有权
        delete graph;
        EnsureGraphicsClose();
    }

    void calculateNodePositions() {
        nodePositions.clear();
        int centerX = windowWidth / 2-100;
        int centerY = windowHeight / 2;
        int radius = 250;

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
        int radius = 20;

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

    void drawEdge(int src, int dest) {
        if (src >= nodePositions.size() || dest >= nodePositions.size()) return;

        POINT srcPos = nodePositions[src];
        POINT destPos = nodePositions[dest];

        double dx = destPos.x - srcPos.x;
        double dy = destPos.y - srcPos.y;
        double length = sqrt(dx * dx + dy * dy);
        if (length == 0) return;

        dx /= length;
        dy /= length;

        int startX = srcPos.x + dx * 20;
        int startY = srcPos.y + dy * 20;
        int endX = destPos.x - dx * 20;
        int endY = destPos.y - dy * 20;

        setlinecolor(EDGE_COLOR);
        setlinestyle(PS_SOLID, 2);
        line(startX, startY, endX, endY);

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

        y += 20;
        info = L"Edges: " + intToWString(edgeCount);
        outtextxy(windowWidth - 280, y, info.c_str());

        y += 20;
        info = isTransformed ? L"Status: Transformed" : L"Status: Original";
        outtextxy(windowWidth - 280, y, info.c_str());

        y += 30;
        outtextxy(windowWidth - 280, y, L"Cut Vertices:");

        y += 20;
        bool hasCutVertices = false;
        for (int i = 0; i < graph->vertexnum; i++) {
            if (graph->is_cut_vertex[i]) {
                hasCutVertices = true;
                info = L"  Vertex " + intToWString(i);
                outtextxy(windowWidth - 260, y, info.c_str());
                y += 20;
            }
        }

        if (!hasCutVertices) {
            outtextxy(windowWidth - 260, y, L"  None");
        }
    }

    //新增返回按钮，保持原有按钮布局不变
    void prepareButtons() {
        buttons.clear();
        int buttonY = windowHeight - 120;
        int buttonWidth = 180;
        int buttonHeight = 40;
        int spacing = 20;
        int exitWidth = 100;

        int x1 = 50;
        int x2 = x1 + buttonWidth + spacing;
        int x3 = x2 + buttonWidth + spacing;
        int x4 = x3 + buttonWidth + spacing;
        int x5 = x4 + buttonWidth + spacing;
        int x6 = x5 + buttonWidth + spacing;

        // 6个按钮
        buttons.resize(6);
        buttons[0] = { x1, buttonY, buttonWidth, buttonHeight, showCutVertices ? L"Hide Cut Vertices" : L"Show Cut Vertices", false, false };
        buttons[1] = { x2, buttonY, buttonWidth, buttonHeight, showDFSInfo ? L"Hide DFS Info" : L"Show DFS Info", false, false };
        buttons[2] = { x3, buttonY, buttonWidth, buttonHeight, L"Find Cut Vertices", false, false };
        buttons[3] = { x4, buttonY, buttonWidth, buttonHeight, isTransformed ? L"Reset Graph" : L"Add Redundant Edges", false, false };
        buttons[4] = { x5, buttonY, buttonWidth, buttonHeight, L"Copy Nodes", false, isTransformed };
        buttons[5] = { x6, buttonY, exitWidth, buttonHeight, L"Return", false, false };
    }

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

    // 原有点在矩形内判断，无改动
    static bool pointInRect(int px, int py, const ButtonInfo& b) {
        return (px >= b.x && px <= b.x + b.w && py >= b.y && py <= b.y + b.h);
    }

    //处理返回按钮点击，返回2表示回退到一级界面
    int processMouseInput() {
        bool anyMouse = false;
        MOUSEMSG m;
        MOUSEMSG lastMsg = { 0 };
        while (MouseHit()) {
            m = GetMouseMsg();
            lastMsg = m;
            anyMouse = true;
        }

        if (!anyMouse) return 0;

        int mx = lastMsg.x;
        int my = lastMsg.y;
        for (auto& btn : buttons) {
            btn.hover = (!btn.disabled) && pointInRect(mx, my, btn);
        }

        if (lastMsg.uMsg == WM_LBUTTONDOWN) {
            if (pointInRect(mx, my, buttons[0])) {
                showCutVertices = !showCutVertices;
                cout << "Toggled cut vertices display: " << (showCutVertices ? "ON" : "OFF") << endl;
            }
            else if (pointInRect(mx, my, buttons[1])) {
                showDFSInfo = !showDFSInfo;
                cout << "Toggled DFS info display: " << (showDFSInfo ? "ON" : "OFF") << endl;
            }
            else if (pointInRect(mx, my, buttons[2])) {
                cout << "Finding cut vertices..." << endl;
                graph->find_cut_vertex();
                int cutCount = graph->get_cut_vertex_count();
                cout << "Found " << cutCount << " cut vertices" << endl;
            }
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
            // 新增：处理返回一级界面按钮
            else if (pointInRect(mx, my, buttons[5])) {
                return 2; // 标识返回一级界面
            }
            prepareButtons();
        }
        return 0;
    }

    void drawControls() {
        prepareButtons();
        for (const auto& b : buttons) {
            drawButton(b);
        }
    }

    // 改造：返回状态值，支持回退一级界面
    int render() {
        cleardevice();

        // 绘制所有边
        for (int i = 0; i < graph->vertexnum; i++) {
            EdgeNode* cur = graph->adjlist[i].firstnode;
            while (cur != nullptr) {
                if (i <= cur->adjvex) {
                    drawEdge(i, cur->adjvex);
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
        int count = processMouseInput();
        settextcolor(TEXT_COLOR);
        settextstyle(32, 0, _T("Arial"));
        outtextxy(50, 30, L"Graph Visualization");
        settextstyle(16, 0, _T("Arial"));
        wstring subtitle = isTransformed ?
            (transformMethod == 0 ? L"With Redundant Edges Transformation" : L"With Node Copy Transformation") :
            L"Adjacency List with Cut Vertex Detection";
        outtextxy(50, 80, subtitle.c_str());

        FlushBatchDraw();
        if (count == -1) {
            return -1; // 退出程序
        }
        if (count == 2) {
            return 2; // 回退一级界面
        }
        return 0;
    }

    // 改造：运行逻辑，支持返回一级界面
    int run() {
        BeginBatchDraw();

        // 初始计算割点
        graph->find_cut_vertex();
        cout << "Initial cut vertices: " << graph->get_cut_vertex_count() << endl;
        int c;
        while (true) {
            c = render();
            if (c == -1 || c == 2) { // 退出或返回一级界面
                break;
            }
        }
        EndBatchDraw(); // 补充原有缺失的EndBatchDraw
        return c; // 返回状态，供main函数判断
    }
};

int charToNum(char ch) 
{
    if (ch >= '0' && ch <= '9') 
    {
        return ch - '0';
    }
    return -1;
}

adj_graph* createGraphbyhand() 
{
    adj_graph* graph;
    int num;
	cout << "input the vertex number(>0):";
    while (true)
    {
        cin >> num;
        if (!num||num<0)
        {
            cout << "invalid input, please input again" << endl;
            cin.clear();
            cin.ignore(65535,'\n');
        }
        else if (num > 50)
        {
			cout << "the vertex number is excessive (max:50), please input again" << endl;
        }
        else if (num > 0)
            break;
    }
    graph = new adj_graph(num);
    while (true)
    {
        cout << "input the edge(-1 -1 is end):";
        int src, dest;
        if (!(cin >> src >> dest))
        {
            cout << "invalid input, please input again" << endl;
            cin.clear();
            cin.ignore(65535, '\n');
            continue;
        }
        if (src == -1 && dest == -1)
            break;
        if (src < 0 || dest < 0 || src >= num || dest >= num)
        {
            cout << "invalid input (out of range), please input again" << endl;
            continue;
        }
        graph->add_edge(src, dest);
    }
    return graph;
}

adj_graph* createRandomGraph()
{
    srand((unsigned int)time(0));
    int verNum = rand()%49+1;
    if (verNum == 0)
    {
        adj_graph* graph = new adj_graph(0);
        return graph;
    }
    int edgeNum = rand();
    adj_graph* graph=new adj_graph(verNum);
    for (int i = 0;i < verNum;i++)
    {
        int ver1 = rand() % verNum;
        int ver2 = rand() % verNum;
        graph->add_edge(ver1, ver2);
    }
    return graph;
}

int getNextValidNumber(ifstream& infile) {
    string numStr;
    char ch;
    //跳过所有非数字字符
    while (true) {
        if (!infile || infile.eof()) {
            return -1;
        }
        infile.get(ch);
        if (ch >= '0' && ch <= '9') {
            numStr += ch;
            break;
        }
    }
    while (true) {
        // 预读取
        if (!infile || infile.eof()) {
            break;
        }
        ch = infile.peek();
        if (ch >= '0' && ch <= '9') {
            infile.get(ch);
            numStr += ch;
        }
        else {
            // 非数字字符，停止读取
            break;
        }
    }
    return atoi(numStr.c_str());
}

adj_graph* createGraphfromfile(const string& filename)
{
    adj_graph* graph = nullptr;
    ifstream infile(filename);
    if (!infile.is_open())
    {
        cerr << "Failed to open file: " << filename << endl;
        return nullptr;
    }
    int vertexnum = getNextValidNumber(infile);
    if (vertexnum == -1) { 
        cerr << "File is empty or has no valid vertex number!" << endl;
        infile.close();
        return nullptr;
    }
    const int MAX_VERTEX_NUM = 50;
    if (vertexnum <= 0 || vertexnum > MAX_VERTEX_NUM) {
        if (vertexnum <= 0) {
            cerr << "Invalid vertex number: " << vertexnum << " (must be positive integer)" << endl;
        }
        else {
            cerr << "The vertex number (" << vertexnum << ") is excessive (max: " << MAX_VERTEX_NUM << ")" << endl;
        }
        infile.close();
        return nullptr;
    }
    graph = new adj_graph(vertexnum);
    if (!graph) {
        cout << "Failed to create adj_graph object!" << endl;
        infile.close();
        return nullptr;
    }
    int srcNum, destNum;
    while (true) {
        srcNum = getNextValidNumber(infile);
        if (srcNum == -1) {
            break;
        }
        destNum = getNextValidNumber(infile);
        if (destNum == -1) {
            cout<< "Warning: Only one valid number remains at the end of the file, cannot build edge!" << endl;
            break;
        }
        graph->add_edge(srcNum, destNum);
    }

    infile.close();
    return graph;
}

adj_graph* InitializeGraph(int method)
{
    adj_graph* graph = nullptr;
    switch (method)
    {
    case 1:
    {
        graph = createGraphbyhand();
        break;
    }
    case 2:
    {
        graph = createRandomGraph();
        break;
    }
    case 3:
    {
        graph = createGraphfromfile("text.txt");
        break;
    }
    default:
        cerr << "please input integer range from 1 to 3\n";
        break;
    }
    return graph;
}

class GraphMainUI {
private:
    int windowWidth;
    int windowHeight;
    struct ButtonInfo { int x, y, w, h; wstring text; bool hover; };
    vector<ButtonInfo> mainButtons;

public:
    GraphMainUI() {
        windowWidth = 1200;
        windowHeight = 800;
        initgraph(windowWidth, windowHeight);
        setbkcolor(BACKGROUND_COLOR);
        cleardevice();
        prepareMainButtons();
    }

    ~GraphMainUI() {
        EnsureGraphicsClose();
    }

    // 初始化一级界面按钮（三种创建方式+退出）
    void prepareMainButtons() {
        mainButtons.clear();
        int buttonW = 300;
        int buttonH = 60;
        int centerX = windowWidth / 2;
        int startY = 200;
        int spacing = 80;

        // 按钮1：手动创建
        mainButtons.push_back({ centerX - buttonW / 2, startY, buttonW, buttonH, L"1. Create Graph by Hand", false });
        // 按钮2：随机创建
        mainButtons.push_back({ centerX - buttonW / 2, startY + spacing, buttonW, buttonH, L"2. Create Random Graph", false });
        // 按钮3：文件创建
        mainButtons.push_back({ centerX - buttonW / 2, startY + 2 * spacing, buttonW, buttonH, L"3. Create Graph from File", false });
        // 按钮4：退出程序
        mainButtons.push_back({ centerX - buttonW / 2, startY + 3 * spacing, buttonW, buttonH, L"4. Exit Program", false });
    }

    // 绘制一级界面按钮（风格与二级界面统一）
    void drawMainButton(const ButtonInfo& b) {
        if (b.hover) {
            setfillcolor(BUTTON_HOVER_COLOR);
        }
        else {
            setfillcolor(BUTTON_COLOR);
        }
        setlinecolor(RGB(50, 50, 50));
        fillroundrect(b.x, b.y, b.x + b.w, b.y + b.h, 10, 10);

        settextcolor(WHITE);
        settextstyle(24, 0, _T("Arial"));
        setbkmode(TRANSPARENT);
        int textWidth = textwidth(b.text.c_str());
        int textHeight = textheight(b.text.c_str());
        outtextxy(b.x + (b.w - textWidth) / 2, b.y + (b.h - textHeight) / 2, b.text.c_str());
    }

    // 绘制一级界面整体布局
    void drawMainUI() {
        cleardevice();

        // 绘制标题
        settextcolor(TEXT_COLOR);
        settextstyle(48, 0, _T("Arial"));
        setbkmode(TRANSPARENT);
        wstring title = L"Graph Creation Main Interface";
        int titleWidth = textwidth(title.c_str());
        outtextxy((windowWidth - titleWidth) / 2, 80, title.c_str());

        // 绘制按钮
        prepareMainButtons();
        for (const auto& btn : mainButtons) {
            drawMainButton(btn);
        }

        FlushBatchDraw();
    }

    // 处理一级界面鼠标交互
    int processMainMouseInput() {
        bool anyMouse = false;
        MOUSEMSG m;
        MOUSEMSG lastMsg = { 0 };
        while (MouseHit()) {
            m = GetMouseMsg();
            lastMsg = m;
            anyMouse = true;
        }

        if (!anyMouse) return 0;

        int mx = lastMsg.x;
        int my = lastMsg.y;
        for (auto& btn : mainButtons) {
            btn.hover = (mx >= btn.x && mx <= btn.x + btn.w && my >= btn.y && my <= btn.y + btn.h);
        }

        // 处理按钮点击
        if (lastMsg.uMsg == WM_LBUTTONDOWN) {
            if (mx >= mainButtons[0].x && mx <= mainButtons[0].x + mainButtons[0].w &&
                my >= mainButtons[0].y && my <= mainButtons[0].y + mainButtons[0].h) {
                return 1; // 手动创建
            }
            else if (mx >= mainButtons[1].x && mx <= mainButtons[1].x + mainButtons[1].w &&
                my >= mainButtons[1].y && my <= mainButtons[1].y + mainButtons[1].h) {
                return 2; // 随机创建
            }
            else if (mx >= mainButtons[2].x && mx <= mainButtons[2].x + mainButtons[2].w &&
                my >= mainButtons[2].y && my <= mainButtons[2].y + mainButtons[2].h) {
                return 3; // 文件创建
            }
            else if (mx >= mainButtons[3].x && mx <= mainButtons[3].x + mainButtons[3].w &&
                my >= mainButtons[3].y && my <= mainButtons[3].y + mainButtons[3].h) {
                return -1; // 退出程序
            }
        }
        return 0;
    }

    // 运行一级界面
    int runMainUI() {
        BeginBatchDraw();
        int choice = 0;
        while (true) {
            drawMainUI();
            choice = processMainMouseInput();
            if (choice != 0) {
                break;
            }
        }
        EndBatchDraw();
        return choice;
    }
};