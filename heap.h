#pragma once
#include <iostream>
#include <fstream>
#include "defns.h"

using namespace std;

struct minheap* create_minheap(int capacity);
struct min_heap_node* new_node_minheap(int v,float dist);
void swap_nodes_minheap(struct min_heap_node** a, struct min_heap_node** b);
void minHeapify(struct minheap* minHeap, int ind);
void decreaseKey(struct minheap* minHeap, int v, float dist);
bool isInMinHeap(struct minheap *minHeap, int v);
int is_minheap_empty(struct minheap* minHeap);
struct min_heap_node* extractMin(struct minheap* minHeap);
