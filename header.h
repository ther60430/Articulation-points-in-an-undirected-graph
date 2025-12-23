#pragma once
#include<iostream>
using namespace std;
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
	int* dfn,*low,*parent;
	bool* is_cut_vertex,*visited;
	adj_graph(int num) 
	{
		if (num < 0)
		{
			cout<< "Vertex number must be non-negative.";
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
	void add_edge(int src, int dest,int weight)
	{
		if (src < 0 || dest < 0||weight<0||src>vertexnum||dest>vertexnum)
		{
			return;
		}
		else if(!is_exist(src,dest))
		{
			EdgeNode* newnode = new EdgeNode(dest, weight);
			newnode->next = adjlist[src].firstnode;
			adjlist[src].firstnode = newnode;
			newnode = new EdgeNode(src, weight);
			newnode->next = adjlist[dest].firstnode;
		}
		else
		{
			EdgeNode* cur = adjlist[src].firstnode;
			EdgeNode* cur2 = adjlist[dest].firstnode;
			while (cur != nullptr)
			{
				if(cur->adjvex==dest)
				{
					if (weight < cur->weight)
					{
						cur->weight = weight;
					}
					break;
				}
			}
			while (cur2 != nullptr)
			{
				if (cur2->adjvex == src)
				{
					if (weight < cur2->weight)
					{
						cur2->weight = weight;
					}
					break;
				}
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
		// 边界检查
		if (u < 0 || u >= vertexnum || adjlist == nullptr) 
		{
			return;
		}
		int children=0;
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
				if(low[v] >= dfn[u]&&parent[u]!=-1)                 //非根节点且满足割点条件
				{
					is_cut_vertex[u] = true;
				}
				if(children >1&&parent[u]==-1)                              //根节点且满足割点条件
				{
					is_cut_vertex[u] = true;
				}
			}
			else if ( v != parent[u])                   //后向边
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
		// 初始化失败，直接返回
		if (vertexnum < 0 || (vertexnum == 0 && !adjlist)) return;
		int newvertexnum = vertexnum + 1;
		// 分配新的动态数组
		Vertex* newadjlist = new (nothrow) Vertex[newvertexnum];
		int* newdfn = new (nothrow) int[newvertexnum];
		int* newlow = new (nothrow) int[newvertexnum];
		int* newparent = new (nothrow) int[newvertexnum];
		bool* newis_cut_vertex = new (nothrow) bool[newvertexnum];
		bool* newvisited = new (nothrow) bool[newvertexnum];
		// 检查内存分配是否成功
		if (!newadjlist || !newdfn || !newlow || !newparent || !newis_cut_vertex || !newvisited) {
			delete[] newadjlist;
			delete[] newdfn;
			delete[] newlow;
			delete[] newparent;
			delete[] newis_cut_vertex;
			delete[] newvisited;
			return;
		}
		for (int i = 0; i < vertexnum; i++) {
			// 深拷贝Vertex的邻接边
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

	adj_graph& TCV_AddRedundantEdges(adj_graph& original, int* cut_vertex,int cut_vertex_number)
	{
		for (int c=0;c<cut_vertex_number;c++)
		{
			int count = 0;
			EdgeNode* edge = original.adjlist[cut_vertex[c]].firstnode;
			while (edge != nullptr)
			{
				count++;
				edge = edge->next;
			}
			int* adj_vertex = new int[count];
			int k = 0;
			edge = original.adjlist[cut_vertex[c]].firstnode;
			while (edge != nullptr)
			{
				adj_vertex[k] = edge->adjvex;
				k++;
				edge = edge->next;
			}
			for (int i = 0;i < count-1;i++)
			{
				original.add_edge(adj_vertex[i], adj_vertex[i+1], 0);
			}
			original.add_edge(adj_vertex[count-1], adj_vertex[0], 0);
			delete[] adj_vertex;
		}
		return original;
	}
	adj_graph& TCV_CopyNode(adj_graph& original, int* cut_vertex, int cut_vertex_number)
	{
		for (int c = 0;c < cut_vertex_number;c++)
		{
			int count = 0;
			EdgeNode* edge = original.adjlist[cut_vertex[c]].firstnode;
			while (edge != nullptr)
			{
				count++;
				edge = edge->next;
			}
			int* adj_vertex = new int[count];
			int k = 0;
			edge = original.adjlist[cut_vertex[c]].firstnode;
			while (edge != nullptr)
			{
				adj_vertex[k] = edge->adjvex;
				k++;
				edge = edge->next;
			}
			original.addvertex();
			int new_vertex = original.vertexnum - 1;
			original.add_edge(cut_vertex[c], new_vertex, 0);
			for (int i = 0;i < count;i++)
			{
				original.add_edge(adj_vertex[i], new_vertex, 0);
			}
			delete[] adj_vertex;
		}
		return original;
	}
	adj_graph& transform_cut_vertex(adj_graph& original, bool* list, int method)
	{
		int count = 0;
		for (int i=0;i<original.vertexnum;i++)
		{
			if (list[i] == true)
				count++;
		}
		int* cut_vertex = new int[count];
		int j = 0;
		for (int i = 0;i < original.vertexnum;i++)
		{
			if (list[i] == true)
			{
				cut_vertex[j] = i;
				j++;
			}
		}
		if (list == nullptr)return original;
		switch (method)
		{
		case 0:
		{
			original= TCV_AddRedundantEdges(original, cut_vertex, count);
			break;
		}
		case 1:
		{
			original=TCV_CopyNode(original,cut_vertex,count);
			break;
		}
		default:
			break;
		}
		delete[] cut_vertex;
		return original;
	}

};
