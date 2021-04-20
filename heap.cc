#include <iostream>
#include <fstream>
#include "defns.h"

using namespace std;

// creates minheap
struct minheap* create_minheap(int capacity)
{
	struct minheap* minHeap = (struct minheap*) malloc(sizeof(struct minheap));
	minHeap->pos = (int *)malloc(capacity * sizeof(int));
	minHeap->size = 0;
	minHeap->capacity = capacity;
	minHeap->array = (struct min_heap_node**)malloc(capacity *sizeof(struct min_heap_node*));
	return minHeap;
}

struct min_heap_node* new_node_minheap(int v,float dist)
{
	struct min_heap_node* minHeapNode = (struct min_heap_node*) malloc(sizeof(struct min_heap_node));
	minHeapNode->v = v;
	minHeapNode->dist = dist;
	return minHeapNode;
}

// swap nodes of minheap
void swap_nodes_minheap(struct min_heap_node** a, struct min_heap_node** b)
{
	struct min_heap_node* t = *a;
	*a = *b;
	*b = t;
}

// maintains the minheap
void minHeapify(struct minheap* minHeap, int ind)
{
	int smallest, left, right;
	smallest = ind;
	left = 2 * ind + 1;
	right = 2 * ind + 2;

	if (left < minHeap->size && minHeap->array[left]->dist < minHeap->array[smallest]->dist )
	{
		smallest = left;
	}

	if (right < minHeap->size && minHeap->array[right]->dist < minHeap->array[smallest]->dist )
	{
		smallest = right;
	}
	if (smallest != ind)
	{
		// The nodes to be swapped in min heap
		min_heap_node *smallestNode = minHeap->array[smallest];
		min_heap_node *indNode = minHeap->array[ind];

		// Swap positions
		minHeap->pos[smallestNode->v] = ind;
		minHeap->pos[indNode->v] = smallest;

		// Swap nodes
		swap_nodes_minheap(&minHeap->array[smallest], &minHeap->array[ind]);

		minHeapify(minHeap, smallest);
	}
}
void decreaseKey(struct minheap* minHeap, int v, float dist)
{
	int i = minHeap->pos[v];
	minHeap->array[i]->dist = dist;

	while (i && minHeap->array[i]->dist < minHeap->array[(i - 1) / 2]->dist)
	{
		// Swap with parent
		minHeap->pos[minHeap->array[i]->v] = (i-1)/2;
		minHeap->pos[minHeap->array[(i-1)/2]->v] = i;
		swap_nodes_minheap(&minHeap->array[i], &minHeap->array[(i - 1) / 2]);

		// move up to parent
		i = (i - 1) / 2;
	}
}

bool isInMinHeap(struct minheap *minHeap, int v)
{
	if (minHeap->pos[v] < minHeap->size)
		return true;
	return false;
}
// check if minHeap is empty
int is_minheap_empty(struct minheap* minHeap)
{
	return minHeap->size == 0;
}

// Extract the min of the heap
struct min_heap_node* extractMin(struct minheap* minHeap)
{
	if (is_minheap_empty(minHeap))
		return NULL;

	// Store the root node
	struct min_heap_node* root = minHeap->array[0];

	// Replace root node with last node
	struct min_heap_node* lastNode = minHeap->array[minHeap->size - 1];
	minHeap->array[0] = lastNode;

	minHeap->pos[root->v] = minHeap->size-1;
	minHeap->pos[lastNode->v] = 0;

	--minHeap->size;
	minHeapify(minHeap, 0);

	return root;
}