#pragma once
#include<iostream>
using namespace std;
class EdgeNode
{
public:
	int adjvex,ad;
	int degree;
};
class vertex
{
public:
	int data;
	EdgeNode* firstnode;
	vertex(int data)
	{
			this->data = data;
			this->firstnode = nullptr;
	}
};
class adj_graph
{
public:
	vertex* adjlist;
	adj_graph();
};
