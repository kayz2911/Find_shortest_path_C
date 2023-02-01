#include <stdio.h>
#include <stdlib.h>
#include "TSP.h"
#define INFINITIVE_VALUE 10000000



int main(void)
{

    double **matrix = (double **)malloc(4 * sizeof(double *));
    for (int i = 0; i < 4; i++)
    {
        matrix[i] = (double *)malloc(4 * sizeof(double));
    }

    matrix[0][0] = INFINITIVE_VALUE;
    matrix[0][1] = 5;
    matrix[0][2] = 9;
    matrix[0][3] = 20;
    matrix[1][0] = 7;
    matrix[1][1] = INFINITIVE_VALUE;
    matrix[1][2] = 10;
    matrix[1][3] = 12;
    matrix[2][0] = 15;
    matrix[2][1] = 8;
    matrix[2][2] = INFINITIVE_VALUE;
    matrix[2][3] = 21;
    matrix[3][0] = 5;
    matrix[3][1] = 10;
    matrix[3][2] = 27;
    matrix[3][3] = INFINITIVE_VALUE;

    int path_memory[5];
    double cost = TSP(matrix, 4, 4, path_memory);
    for(int i=0;i<5;i++){
        printf("%d ",path_memory[i]);
    }
    printf("\n Min cost: %lf\n", cost);
    return 0;
}