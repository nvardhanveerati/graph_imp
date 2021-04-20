#pragma once
#include <iostream>
#include <fstream>
#include "defns.h"

using namespace std;

struct adj_list_node* new_adj_list_node(int dest, float weight);
struct Graph* create_graph(int V);
void insert_graph_edge(struct Graph* graph, int src, int dest, float weight, string directed);
