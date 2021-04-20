#include <iostream>
#include <fstream>
#include "defns.h"

using namespace std;

struct adj_list_node* new_adj_list_node(int dest, float weight)
{
	struct adj_list_node* newNode = (struct adj_list_node*) malloc(sizeof(struct adj_list_node));
	newNode->dest = dest;
	newNode->weight = weight;
	newNode->next = NULL;
	return newNode;
}

// creates graph
struct Graph* create_graph(int V)
{
	struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));
	graph->V = V;
	graph->array = (struct adjacency_list*) malloc(V * sizeof(struct adjacency_list));

	for (int i = 0; i < V; ++i)
		graph->array[i].head = NULL;

	return graph;
}

// Inserts and edge int othe graph
void insert_graph_edge(struct Graph* graph, int src, int dest, float weight, string directed)
{
	struct adj_list_node* newNode = new_adj_list_node(dest, weight);
	newNode->next = graph->array[src].head;
	graph->array[src].head = newNode;

	// check for direction
    if(directed == "undirected")
    {
        newNode = new_adj_list_node(src, weight);
        newNode->next = graph->array[dest].head;
        graph->array[dest].head = newNode;
    }
}
