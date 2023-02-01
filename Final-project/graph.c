#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

#define INFINITIVE_VALUE 10000000

Graph createGraph()
{
    Graph graph;
    graph.edges = make_jrb();
    graph.vertices = make_jrb();
    return graph;
}
/**
 * check key has maximum value in the graph
**/
int maxKey(Graph graph)
{
    JRB currentNode;
    int max = 0;
    jrb_traverse(currentNode, graph.vertices)
    {
        if (jval_i(currentNode->key) >= max)
        {
            max = jval_i(currentNode->key);
        }
    }
    return max;
}

void addVertex(Graph graph, int id, char* name){
    JRB node = jrb_find_int(graph.vertices, id);
    if(node == NULL)
        jrb_insert_int(graph.vertices, id, new_jval_s(strdup(name)));
    else
        strcpy(jval_s(node->val), name);
}

char *getVertex(Graph graph, int id){
    JRB node = jrb_find_int(graph.vertices, id);
    if(node == NULL)
        return NULL;
    else
        return (jval_s(node->val));
}

void addEdge(Graph graph, int v1, int v2, double weight)
{
    JRB tree1 = make_jrb();
    JRB treeV2 = make_jrb();
    JRB node1 = jrb_find_int(graph.edges, v1);
    JRB nodeV2 = jrb_find_int(graph.edges, v2);
    if (node1 == NULL)
    {
        jrb_insert_int(graph.edges, v1, new_jval_v(tree1));
        jrb_insert_int(tree1, v2, new_jval_d(weight));
    }
    else
    {
        JRB tree = (JRB)jval_v(node1->val);
        JRB nodeCheckDupV2 = jrb_find_int(tree, v2);
        if(nodeCheckDupV2 == NULL){
            jrb_insert_int(tree, v2, new_jval_d(weight));
        }
        else{
            nodeCheckDupV2->val = new_jval_d(weight);
        }
        
    }
    if (nodeV2 == NULL)
    {
        jrb_insert_int(graph.edges, v2, new_jval_v(treeV2));
    }
}
/**
 * return INFINITIVE_VALUE if no edge between v1 and v2
**/
double getEdgeValue(Graph graph, int v1, int v2)
{
    JRB node = jrb_find_int(graph.edges, v1);
    JRB tree = (JRB)jval_v((node->val));
    JRB result = jrb_find_int(tree, v2);
    return jval_d(result->val);
}

int hasEdge(Graph graph, int v1, int v2)
{
    JRB node1 = jrb_find_int(graph.edges, v1);
    if (node1 == NULL)
    {
        return 0;
    }
    else
    {
        JRB tree = (JRB)jval_v(node1->val);
        JRB result = jrb_find_int(tree, v2);
        if (result == NULL)
        {
            return 0;
        }
        return 1;
    }
}

int indegree(Graph graph, int v, int *output)
{
    int total = 0;
    JRB node;
    jrb_traverse(node, graph.edges)
    {
        JRB tree = (JRB)jval_v(node->val);
        if (jrb_find_int(tree, v) != NULL)
        {
            output[total++] = jval_i(node->key);
        }
    }
    return total;
}

int outdegree(Graph graph, int v, int *output)
{
    int total = 0;
    JRB node = jrb_find_int(graph.edges, v);
    if (node != NULL)
    {
        JRB tree = (JRB)jval_v(node->val);
        if (tree == NULL)
        {
            return 0;
        }
        JRB tmp;
        jrb_traverse(tmp, tree)
        {
            output[total++] = jval_i(tmp->key);
        }
    }
    return total;
}

/**
 * find in the ddlis, return 1 if found, return 0 if not
 **/
int findDll(Dllist list, int key)
{
    Dllist ptr;
    dll_traverse(ptr, list)
    {
        if (jval_i(ptr->val) == key)
        {
            return 1;
        }
    }
    return 0;
}
/**
 * check graph contain cycle. if contain return 0, not return 1
 * */
int DAG(Graph graph)
{
    int result = 1;
    int sizeVisittedArray = maxKey(graph) + 1;
    int *visittedArray = (int *)malloc((sizeVisittedArray) * sizeof(int));
    for (int i = 0; i < sizeVisittedArray; i++)
    {
        visittedArray[i] = 0;
    }

    Dllist stack = new_dllist();
    int currentKey = jval_i(jrb_first(graph.vertices)->key);
    dll_append(stack, new_jval_i(currentKey));
    visittedArray[currentKey] = 1;
    while (!dll_empty(stack))
    {
        int check = 0;
        JRB temp;
        JRB node = jrb_find_int(graph.edges, jval_i(dll_last(stack)->val));
        JRB tree = (JRB)jval_v(node->val);
        if (tree == NULL)
        {
            //                                                                                                                                                                                                                          dll_delete_node(dll_last(stack));
        }
        else
        {
            jrb_traverse(temp, tree)
            {
                if (visittedArray[jval_i(temp->key)] == 0)
                {
                    dll_append(stack, temp->key);
                    visittedArray[jval_i(temp->key)] = 1;
                    check++;
                }
                else
                {
                    if (findDll(stack, jval_i(temp->key)) == 1)
                        result = 0; // mean that the vertice visitted tiwce
                }
            }
        }
        if (check == 0)
        {
            dll_delete_node(dll_last(stack));
        }
    }
    return result;
}

void dropGraph(Graph graph)
{
    free(graph.edges);
    free(graph.vertices);
}

void topologicalSort(Graph g)
{
    int ind[100], output[100], outd[100], n, index;
    int output_tmp[100];
    JRB tmp;
    Dllist queue = new_dllist();
    Dllist node;
    jrb_traverse(tmp, g.vertices)
    {
        ind[jval_i(tmp->key)] = indegree(g, jval_i(tmp->key), output_tmp);
        if (indegree(g, jval_i(tmp->key), output_tmp) == 0)
        {
            dll_append(queue, tmp->key);
        }
    }
    int count = 0;
    while (!dll_empty(queue))
    {
        node = dll_first(queue);
        index = jval_i(node->val);
        output[count++] = index;
        printf("%s ", getVertex(g, index));
        n = outdegree(g, index, outd);
        dll_delete_node(node);
        for (int i = 0; i < n; i++)
        {
            int j = outd[i];
            ind[j]--;
            if (ind[j] == 0)
            {
                dll_append(queue, new_jval_i(j));
            }
        }
    }
}

void swap(double *a, double *b)
{
    double temp = *a;
    *a = *b;
    *b = temp;
}

int partition_2(double arr[], int low, int high)
{
    int pivot = arr[high];
    int left = low;
    int right = high - 1;
    while (1)
    {
        while (left <= right && arr[left] < pivot)
            left++;
        while (right >= left && arr[right] > pivot)
            right--;
        if (left >= right)
            break;
        swap(&arr[left], &arr[right]);
        left++;
        right--;
    }
    swap(&arr[left], &arr[high]);
    return left;
}

void quickSort_2(double arr[], int low, int high)
{
    if (low < high)
    {
        int pivot = partition_2(arr, low, high);
        quickSort_2(arr, low, pivot - 1);
        quickSort_2(arr, pivot + 1, high);
    }
}

/**
 * return the total weight of the path and the path is given via path and its length.
 * Return cost, INFINITIVE_VALUE if no path is found 
 * return path and length of path
 **/
double shortestPath(Graph graph, int s, int t, Dllist path, int *length)
{

    int sizeArray = maxKey(graph);
    int *visitted = (int *)malloc(sizeArray * sizeof(int)); // array remember which node was passed
    for (int i = 0; i <= sizeArray; i++)
    {
        visitted[i] = 0;
    }
    double *cost = (double *)malloc(sizeArray * sizeof(double)); //array remember cost of each node
    for (int i = 0; i <= sizeArray; i++)
    {
        cost[i] = INFINITIVE_VALUE;
    }
    int *memory_path = (int *)malloc(sizeArray * sizeof(int)); //array remember next node which closest from current node
    for (int i = 0; i <= sizeArray; i++)
    {
        memory_path[i] = -1;
    }
    Dllist queue = new_dllist();
    dll_append(queue, new_jval_i(s));
    cost[s] = 0;
    while (!dll_empty(queue))
    {
        int current_vertex = jval_i(dll_first(queue)->val);
        if (visitted[current_vertex] == 0) // maybe in the queue has same vertext, but after the vertext passed, next vertext(the same vertext) can skip
        {
            int count_adjacent_node = 0;
            //double *arr_cost_node = (double *)malloc(1 * sizeof(double));
            double arr_cost_node[100];
            JRB temp;
            JRB node = jrb_find_int(graph.edges, current_vertex);
            if(node == NULL){
                printf("no\n");
            }
            JRB tree = (JRB)jval_v(node->val);

            // compute cost of each node adjacent with current node
            jrb_traverse(temp, tree)
            {
                double p = jval_d(temp->val) + cost[current_vertex];
                int key = jval_i(temp->key);
                if (p < cost[key])
                {
                    cost[key] = p;                     // update min cost
                    memory_path[key] = current_vertex; // update closet node
                }
                arr_cost_node[count_adjacent_node] = cost[key];
                count_adjacent_node++;
                //arr_cost_node = (double *)realloc(arr_cost_node, (count_adjacent_node + 1) * sizeof(double));
            }

            //mark current node passed
            visitted[current_vertex] = 1;
            // put node in queue increase order of cost at adjecent node with current node
            quickSort_2(arr_cost_node, 0, count_adjacent_node - 1);
            for (int i = 0; i < count_adjacent_node; i++)
            {
                jrb_traverse(temp, tree)
                {
                    int key = jval_i(temp->key);
                    if ((visitted[key] == 0) && (cost[key] == arr_cost_node[i]))
                    {
                        dll_append(queue, temp->key);
                    }
                }
            }
        }
        dll_delete_node(dll_first(queue));
    }
    // find path & length of the path
    int index = t;
    dll_prepend(path, new_jval_i(t));
    *length = *length + 1;
    while (index != s)
    {
        index = memory_path[index];
        dll_prepend(path, new_jval_i(index));
        *length = *length + 1;
    }
    return cost[t];
}

/**
 * return the total weight of the path and the path is given via path and its length.
 * Return cost, INFINITIVE_VALUE if no path is found 
 * Dont return path
 **/
double shortestPath_2(Graph graph, int s, int t){
    int sizeArray = maxKey(graph);
    int *visitted = (int *)malloc(sizeArray * sizeof(int)); // array remember which node was passed
    for (int i = 0; i <= sizeArray; i++)
    {
        visitted[i] = 0;
    }
    double *cost = (double *)malloc(sizeArray * sizeof(double)); //array remember cost of each node
    for (int i = 0; i <= sizeArray; i++)
    {
        cost[i] = INFINITIVE_VALUE;
    }
    int *memory_path = (int *)malloc(sizeArray * sizeof(int)); //array remember next node which closest from current node
    for (int i = 0; i <= sizeArray; i++)
    {
        memory_path[i] = -1;
    }
    Dllist queue = new_dllist();
    dll_append(queue, new_jval_i(s));
    cost[s] = 0;
    while (!dll_empty(queue))
    {
        int current_vertex = jval_i(dll_first(queue)->val);
        if (visitted[current_vertex] == 0) // maybe in the queue has same vertext, but after the vertext passed, next vertext(the same vertext) can skip
        {
            int count_adjacent_node = 0;
            double *arr_cost_node = (double *)malloc(1 * sizeof(double));

            JRB temp;
            JRB node = jrb_find_int(graph.edges, current_vertex);
            JRB tree = (JRB)jval_v(node->val);

            // compute cost of each node adjacent with current node
            jrb_traverse(temp, tree)
            {
                double p = jval_d(temp->val) + cost[current_vertex];
                int key = jval_i(temp->key);
                if (p < cost[key])
                {
                    cost[key] = p;                     // update min cost
                    memory_path[key] = current_vertex; // update closet node
                }
                arr_cost_node[count_adjacent_node] = cost[key];
                count_adjacent_node++;
                arr_cost_node = (double *)realloc(arr_cost_node, (count_adjacent_node + 1) * sizeof(double));
            }

            //mark current node passed
            visitted[current_vertex] = 1;
            // put node in queue increase order of cost at adjecent node with current node
            quickSort_2(arr_cost_node, 0, count_adjacent_node - 1);
            for (int i = 0; i < count_adjacent_node; i++)
            {
                jrb_traverse(temp, tree)
                {
                    int key = jval_i(temp->key);
                    if ((visitted[key] == 0) && (cost[key] == arr_cost_node[i]))
                    {
                        dll_append(queue, temp->key);
                    }
                }
            }
        }
        dll_delete_node(dll_first(queue));
    }
    return cost[t];
}

double getEdgeWeight(Graph g, int v1, int v2){
    JRB nodeV1, treeV1, ans;
    nodeV1 = jrb_find_int(g.edges, v1);
    if(nodeV1){
        treeV1 = (JRB) jval_v(nodeV1->val);
        ans = jrb_find_int(treeV1, v2);
        if(ans)
            return jval_d(ans->val);
    }
    return INFINITIVE_VALUE;
}
/* if negative weighted edges can be allowed, the algorithm may find the longer path
, try to gather as much as possible the negative numbers(VD let all weighted edges be
negative.), so you can add a fixed number that make sure all edges that have positive 
weight then subtract later.*/
double shortestPath_3(Graph g, int start, int end, int *path, int *length){
    JRB travel, ans, S = make_jrb();
    int min=INFINITIVE_VALUE, u, v,j, stt=0, i=0, max = INFINITIVE_VALUE;
    double L[100]={};
    jrb_traverse(travel, g.vertices){
        j = jval_i(travel->key);
        L[j] = INFINITIVE_VALUE; // is length of shortest path from start to vertex i
    }
    L[start] = 0;
    while(!jrb_find_int(S, end)){
        // search u : L[u] min and dont belong to S
        min = INFINITIVE_VALUE;   
        jrb_traverse(travel, g.vertices){
            j = jval_i(travel->key);
            if(jrb_find_int(S, j) == NULL && L[j] < min){
                min = L[j];
                u = j;
            }
        }
        jrb_insert_int(S, u, new_jval_i(0));
        jrb_traverse(travel, g.vertices){
            v = jval_i(travel->key);
            if(jrb_find_int(S, v)==NULL && L[u] + getEdgeWeight(g, u, v) < L[v]){
                L[v] = L[u] + getEdgeWeight(g, u, v);
            }
        }
    }
    max = end;
    path[i++] = end;
    //create shortestPath
    while(max!=start){
        jrb_rtraverse(travel, S){
            v = jval_i(travel->key);
            if(L[max] == L[v] + getEdgeWeight(g, v, max)){
                path[i++] = v;
                max = v;
            }
        }
    }
    *length=i;
    return L[end];
}