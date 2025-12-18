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
	void add_edge(int src, int dest,int weight);
	void delete_edge(int src, int dest);
	bool is_connected();
	bool* DFS_cut_vertex(int u);
	void addvertex();
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
	adj_graph& TCV_ReconnectEdge(adj_graph& original, int* cut_vertext, int cut_vertex_number);
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
