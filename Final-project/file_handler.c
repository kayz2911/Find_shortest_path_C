#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "file_handler.h"
#include "graph.h"

#define MAX 100
#define MAXDATA 500000

Graph read_db(char *filename, Graph graph)
{
    FILE *fptr;
    if ((fptr = fopen(filename, "r")) == NULL)
    {
        printf("Cannot open file named %s\n", filename);
        return graph;
    }
    else
    {
        char str[MAX];
        int countData = 0;
        int data[MAXDATA];
        for (int i = 0; i < MAXDATA; i++)
        {
            data[i] = 0;
        }
        while (fgets(str, MAX, fptr) != NULL)
        {
            char from[MAX];
            char to[MAX];
            double weight;
            fscanf(fptr, "%s %s %lf", from, to, &weight);

            char from2[MAX];
            char to2[MAX];

            strcpy(from2, from);
            strcpy(to2, to);

            int id1, id2;

            char delim1[] = "-";
            char delim2[] = "-";
            char *ptr1 = strtok(from, delim1);
            int count = 0;
            while (ptr1 != NULL)
            {
                if (count > 0)
                {
                    count++;
                }
                id1 = atoi(ptr1);
                ptr1 = strtok(NULL, delim1);
            }
            count = 0;
            char *ptr2 = strtok(to, delim2);
            while (ptr2 != NULL)
            {
                if (count > 0)
                {
                    id2 = atoi(ptr2);
                }
                ptr2 = strtok(NULL, delim2);
                count++;
            }

            if (data[id1] != 1)
            {
                addVertex(graph, countData, from2);
                data[id1] = 1;
                countData++;
            }

            if (data[id2] != 1)
            {
                addVertex(graph, countData, to2);
                data[id2] = 1;
                countData++;
            }

            addEdge(graph, id1 - 1, id2 - 1, weight);
        }
        return graph;
    }
}