#pragma once
using namespace std;

struct parsed_query
{
    string key_term;
    int source;
    int destination;
    int flag;
};

struct adj_list_node
{
	int dest;
	float weight;
	struct adj_list_node* next;
};

struct adjacency_list
{	
	struct adj_list_node *head;
};

struct Graph
{
	int V;
	struct adjacency_list* array;
};

struct min_heap_node
{
    float dist;
	int v;
};

struct minheap
{
    int *pos;
	int size;	
	int capacity;
	struct min_heap_node **array;
};