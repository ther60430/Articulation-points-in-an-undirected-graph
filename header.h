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
	}
};
class adj_graph
{
public:
	int vertexnum;
	int edgenum;
	Vertex* adjlist;
	int* dfn,*low,*parent;
	bool* is_cut_vertex;
	adj_graph(int num) 
	{
		if (num < 0)
		{
			cout<< "Vertex number must be non-negative.";
			return;
		}
		else
		{
			vertexnum = num;
			edgenum = 0;
			adjlist = new Vertex[vertexnum];  
			dfn = new int[vertexnum];
			low = new int[vertexnum];
			parent = new int[vertexnum];
			is_cut_vertex = new bool[vertexnum];
			for (int i = 0; i < vertexnum; i++)
			{
				dfn[i] = 0;
				low[i] = 0;
				parent[i] = -1;
				is_cut_vertex[i] = false;
			}
		}
	}
	~adj_graph()
	{
		delete[] adjlist;
	}
	void add_edge(int src, int dest);
	bool is_connected();
	Vertex* DFS_cut_vertex(int u);
	adj_graph* transform_cut_vertex(Vertex* list);
	void delete_edge(int src, int dest);
};
