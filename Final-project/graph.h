#ifndef _GRAPH_H
#define _GRAPH_H

#include "dllist.h"
#include "jrb.h"

typedef struct
{
    JRB edges;
    JRB vertices;
} Graph;

extern Graph createGraph();
extern int maxKey(Graph graph);
extern void addVertex(Graph graph, int id, char *name);
extern char *getVertex(Graph graph, int id);
extern void addEdge(Graph graph, int v1, int v2, double weight);
extern double getEdgeValue(Graph graph, int v1, int v2);
extern int hasEdge(Graph graph, int v1, int v2);
extern int indegree(Graph graph, int v, int *output);
extern int outdegree(Graph graph, int v, int *output);
extern int findDll(Dllist list, int key);
extern int DAG(Graph graph);
extern void dropGraph(Graph graph);
extern void topologicalSort(Graph g);
/**
 * return the total weight of the path and the path is given via path and its length.
 * Return cost, INFINITIVE_VALUE if no path is found.
 * return path and length of path.
 **/
extern double shortestPath(Graph graph, int s, int t, Dllist path, int *length);
/**
 * return the total weight of the path and the path is given via path and its length.
 * Return cost, INFINITIVE_VALUE if no path is found.
 * Dont return path.
 **/
extern double shortestPath_2(Graph graph, int s, int t);
extern double shortestPath_3(Graph graph, int start, int end, int *path, int *length);
#endif