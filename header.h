#pragma once
#include<iostream>
using namespace std;
class EdgeNode
{
public:
	int adjvex;
	int weight;
	EdgeNode* next;
	EdgeNode(int adjvex, int weight) :adjvex(adjvex), weight(weight), next(nullptr) {}
};
class vertex
{
public:
	int data;
	EdgeNode* firstnode;
};
class adj_graph
{
public:
	int vexnum, edgenum;
	vertex* adjlist;
};
