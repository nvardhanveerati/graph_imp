#include <iostream>
#include <fstream>
#include <cstring>
#include "defns.h"
#include "graph.h"
#include "heap.h"

using namespace std;

static int most_recent_find_source = -1;
static int most_recent_find_dest = -1;
int MAX_INT = 2147483645;

void print_predecessors(int *predecessor_array, int source, int dest, int *inserted_into_heap, int *deleted_from_heap)
{
	int temp_s = source;
	int temp_d = dest;
	int temp_iter;

	string ans="";
	ans = to_string(temp_d)+">";
	if(inserted_into_heap[temp_d] == 0 && deleted_from_heap[temp_d] == 0)
	{
		cout << "No "<<temp_s<<"-"<<temp_d<<" path has been computed."<<endl;
		return;
	}
	while(temp_d!=temp_s)
	{
		temp_iter = predecessor_array[temp_d];
		ans = to_string(temp_iter)+ ", "+ans;
		temp_d = temp_iter;
	}
	if(inserted_into_heap[temp_d] == 1 && deleted_from_heap[temp_d] == 0)
	{
		ans = "Path not known to be shortest: "+ans;
		cout <<ans<<endl;
		return;
	}
	ans = "Shortest path: <"+ans;
	cout << ans <<endl;
}

void dijkstra(struct Graph* graph, int src, int destination, int flag, int *predecessor_array, float *weights_array, int *inserted_into_heap, int *deleted_from_heap)
{
	
	// Number of nodes in graph
	int V = graph->V;

	// dist values
	float* dist = new float[V];

	// minHeap contains picked values
	struct minheap* minHeap = create_minheap(V);

	// Initialize 
	for (int v = 0; v < V; ++v)
	{
		dist[v] = MAX_INT;
	}

	int heap_position = minHeap->size;
	minHeap->array[heap_position] = new_node_minheap(src, dist[src]);
	
	//cout insertion
	if(flag == 1)
	{
		printf("Insert vertex %d, key=%12.4f\n",src, 0.0);
	}
	inserted_into_heap[src]= 1;
    minHeap->pos[src] = heap_position;
	dist[src] = 0;
	decreaseKey(minHeap, src, dist[src]);

	minHeap->size++;

	while (!is_minheap_empty(minHeap))
	{
		// Etract minimun from heap
		struct min_heap_node* minHeapNode = extractMin(minHeap);
		
		deleted_from_heap[minHeapNode->v] = 1;
		if(flag == 1)
		{
			printf("Delete vertex %d, key=%12.4f\n",minHeapNode->v, minHeapNode->dist);
		}
		
		if (minHeapNode->v == destination)
        {
            return;
        }
		int u = minHeapNode->v;

		struct adj_list_node* pCrawl = graph->array[u].head;
		while (pCrawl != NULL)
		{
			int v = pCrawl->dest;
            if (dist[v] == MAX_INT && pCrawl->weight + dist[u] < dist[v])
            {
                int heap_position = minHeap->size;
                minHeap->array[heap_position] = new_node_minheap(v, dist[v]);
				inserted_into_heap[v] = 1;
                minHeap->pos[v] = heap_position;
				predecessor_array[v] = u;
                dist[v] = pCrawl->weight + dist[u];
				//cout insertion
				if(flag == 1)
				{
					printf("Insert vertex %d, key=%12.4f\n",v, dist[v]);
				}
                decreaseKey(minHeap, v, dist[v]);
                minHeap->size++;
            }

            if (dist[u] != MAX_INT && pCrawl->weight + dist[u] < dist[v])
			{
				float old_dist = dist[v];
				dist[v] = dist[u] + pCrawl->weight;
				predecessor_array[v] = u;
				// update distance
				// value in min heap also
				decreaseKey(minHeap, v, dist[v]);
				if(flag == 1)
				{
					printf("Decrease key of vertex %d, from %12.4f to %12.4f\n",v,old_dist,dist[v]);
				}
			}
			pCrawl = pCrawl->next;
		}
	}

	for(int i=0;i<V;i++)
	{
		weights_array[i] = dist[i];
	}
}

void tokenize(string line, string sep, string *str_ptr)
{
    const char* sepa = sep.c_str();

    char* token;
	char str[line.length()];
    strcpy(str, line.c_str());
    char *rest = str;

    int i=0;
    while ((token = strtok_r(rest, sepa, &rest))) 
        str_ptr[i++] = token;
}

void execute_find_query(string query, struct Graph* graph, int num_nodes, int *predecessor_array, float *weights_array, int *inserted_into_heap, int *deleted_from_heap)
{
	string *find_query_parts = new string[5];
	tokenize(query," ",find_query_parts);

	int q_source = stoi(find_query_parts[1]);
	int q_dest = stoi(find_query_parts[2]);
	int q_flag = stoi(find_query_parts[3]);

	bool flag_1=false;
	bool flag_2=false;
	bool flag_3=false;
	if(q_source >= 1 && q_source <= num_nodes)
		flag_1=true;
	if(q_dest!=q_source)
		flag_2=true;
	if(q_flag==0 || q_flag==1)
		flag_3=true;
	if(!(flag_1 && flag_2 && flag_3))
	{
		cout << "Error: invalid find query"<<endl;
	}
	else
	{
		most_recent_find_source = q_source;
		most_recent_find_dest = q_dest;
		for(int i=0;i<num_nodes+1;i++)
		{
			predecessor_array[i] = -100;
			weights_array[i] = 0.0;
			inserted_into_heap[i] = 0;
			deleted_from_heap[i] = 0;
		}
		dijkstra(graph, q_source, q_dest, q_flag, predecessor_array, weights_array, inserted_into_heap, deleted_from_heap);
	}

	delete[] find_query_parts;
}

void execute_write_query(string query, struct Graph* graph, int num_nodes, int* predecessor_array, float* weights_array, int* inserted_into_heap, int *deleted_from_heap)
{
	string *write_query_parts = new string[5];
	tokenize(query," ",write_query_parts);

	int write_source = stoi(write_query_parts[2]);
	int write_dest = stoi(write_query_parts[3]);

	if(most_recent_find_dest == -1 && most_recent_find_source == -1)
	{
		cout << "Error: no path computation done" << endl;
		return;
	}
	if(most_recent_find_source!= write_source || write_dest<1 || write_dest>num_nodes || write_dest==write_source)
	{
		cout << "Error: invalid source destination pair" << endl;
		return;
	}

	if(weights_array[write_dest] == MAX_INT)
	{
		cout << "No "<<write_source<<"-"<<write_dest<<" path exists." << endl;
	}
	else
	{
		print_predecessors(predecessor_array, write_source, write_dest,inserted_into_heap, deleted_from_heap);
		if(!(inserted_into_heap[write_dest] == 0 && deleted_from_heap[write_dest] == 0))
		{
			printf("The path weight is: %12.4f\n", weights_array[write_dest]);
		}
	}
	delete[] write_query_parts;
}

void parse_query(string query, struct Graph* graph, int num_nodes, int *predecessor_array, float *weights_array, int *inserted_into_heap, int *deleted_from_heap)
{
	string subs = query.substr(0,4);
	if(subs == "find")
	{
		execute_find_query(query, graph, num_nodes, predecessor_array, weights_array, inserted_into_heap, deleted_from_heap);
	}
	else if(subs == "writ")
	{
		execute_write_query(query, graph, num_nodes, predecessor_array, weights_array, inserted_into_heap, deleted_from_heap);
	}
}

int main(int argc, char** argv)
{
    string graph_filename = argv[1];
    string directed = argv[2];

    int num_nodes, num_edges;
    
    ifstream input_file;
    input_file.open(graph_filename);
    input_file >> num_nodes;
    input_file >> num_edges;
        
    struct Graph* graph = create_graph(num_nodes+1);
    for(int i=0;i<num_edges;i++)
    {
        int t, src, dest;
        float weight;
        input_file >> t; 
        input_file >> src;
        input_file >> dest;
        input_file >> weight;

        insert_graph_edge(graph, src, dest, weight, directed);
    }

	int *predecessor_array = new int[num_nodes+1];
	for(int i=0;i<num_nodes+1;i++)
	{
		predecessor_array[i] = -100;
	}
	float *weights_array = new float[num_nodes+1];
	int *inserted_into_heap = new int[num_nodes+1];
	int *deleted_from_heap = new int[num_nodes+1];

	string conc_queries = "";
	string query;
	int num_queries = 0;
	getline(cin, query);
	while(query!="stop")
	{
		if(conc_queries!="")
			conc_queries = conc_queries +"|"+query;
		else
			conc_queries = query;
		num_queries++;
		getline(cin, query);
	}
	num_queries++;
	conc_queries+="|stop";

	string *queries = new string[num_queries];
	tokenize(conc_queries, "|",queries);
	for(int i=0;i<num_queries;i++)
	{
		cout << "Query: "<<queries[i]<<endl;
		parse_query(queries[i], graph, num_nodes, predecessor_array, weights_array,inserted_into_heap,deleted_from_heap);
	}

	delete[] weights_array;
	delete[] inserted_into_heap;
	delete[] deleted_from_heap;
	delete[] predecessor_array;
	delete[] queries;
    return 0;
}

