#pragma once
#include<iostream>
using namespace std;
typedef struct EdgeNode
{
	int adjvex;
	int weight;
	struct EdgeNode* next;
};
typedef struct vertex
{
	int data;
	EdgeNode* firstnode;
};
class adj_graph
{
public:
	int vexnum, edgenum;
	vertex* adjlist;
};
