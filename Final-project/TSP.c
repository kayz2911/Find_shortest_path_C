#include <stdio.h>
#include <stdlib.h>

#include "TSP.h"

#define INFINITIVE_VALUE 10000000

/** find min value in arr
 * return index**/
int min(double *arr, int length, int *skip_index)
{
    int index;
    double min = arr[0];
    index = 0;
    for (int i = 0; i < length; i++)
    {
        if (arr[i] < min && skip_index[i] == 0)
        {
            min = arr[i];
            index = i;
        }
    }
    return index;
}

double reduce_value(double **matrix, int row, int col, int *row_full_infinti, int *col_full_infiniti)
{
    double cost = 0;
    for (int i = 0; i < row; i++)
    {
        if (row_full_infinti[i] == 0)
        {
            int index = min(matrix[i], col, col_full_infiniti);
            double min = matrix[i][index];
            cost += min;
            for (int j = 0; j < col; j++)
            {
                matrix[i][j] -= min;
            }
        }
    }

    for (int k = 0; k < col; k++)
    {
        if (col_full_infiniti[k] == 0)
        {
            double *tmpArr = (double *)malloc(row * sizeof(double));
            for (int i = 0; i < row; i++)
            {
                tmpArr[i] = matrix[i][k];
            }
            int index = min(tmpArr, row, row_full_infinti);
            double min = matrix[index][k];
            cost += min;
            for (int j = 0; j < row; j++)
            {
                matrix[j][k] -= min;
            }
        }
    }

    return cost;
}
/**
 * return 1-success, 0-found subpath**/
int reduce_matrix(double **matrix, int row, int col, int *path_memory, int *row_full_infinti, int *col_full_infiniti, int *visitedArr)
{
    int index_row, index_col;
    double max_0 = 0;
    //find best index
    for (int i = 0; i < row; i++)
    {
        for (int j = 0; j < col; j++)
        {
            double min_row = matrix[0][0], min_col = matrix[0][0];
            if (matrix[i][j] == 0)
            {
                for (int n = 0; n < col; n++)
                {
                    if (matrix[i][n] < min_row && n != j)
                    {
                        min_row = matrix[i][n];
                    }
                }

                for (int m = 0; m < row; m++)
                {
                    if (matrix[m][j] < min_col && m != i)
                    {
                        min_col = matrix[m][j];
                    }
                }

                if ((min_col + min_row) >= max_0)
                {
                    max_0 = min_col + min_row;
                    index_row = i;
                    index_col = j;
                }
            }
        }
    }
    // path_memory with index is index_row, value of index is index_col
    int flag = 0;
    int index = index_col;
    while (visitedArr[index] == 1)//check subpath
    {
        index = path_memory[index];
        if (index == index_row)
            flag++;
    }
    if (flag == 0) //not found
    {
        visitedArr[index_row] = 1;
        path_memory[index_row] = index_col;
        row_full_infinti[index_row] = 1;
        col_full_infiniti[index_col] = 1;
        for (int i = 0; i < col; i++)
        {
            matrix[index_row][i] = INFINITIVE_VALUE;
        }
        for (int i = 0; i < row; i++)
        {
            matrix[i][index_col] = INFINITIVE_VALUE;
        }
        matrix[index_col][index_row] = INFINITIVE_VALUE;
        return 1;
    }
    else{
        matrix[index_row][index_col] = INFINITIVE_VALUE;
        return 0;
    }
}
/** 
 * Traversal saleman problem **/
double TSP(double **Matrix, int row, int col, int *path_memory)
{
    double cost = 0;
    double **matrix = Matrix;
    int *row_full_infiniti = (int *)malloc(row * sizeof(int));
    for (int i = 0; i < row; i++)
    {
        row_full_infiniti[i] = 0;
    }
    int *col_full_infiniti = (int *)malloc(col * sizeof(int));
    for (int i = 0; i < col; i++)
    {
        col_full_infiniti[i] = 0;
    }
    int *visittedArr = (int *)malloc(col * sizeof(int));
    for (int i = 0; i < col; i++)
    {
        visittedArr[i] = 0;
    }
    int size_matrix = row;
    while (size_matrix != 2)
    {
        cost = cost + reduce_value(matrix, row, col, row_full_infiniti, col_full_infiniti);
        int check =  0;
        while (check == 0)
        {
            check = reduce_matrix(matrix, row, col, path_memory, row_full_infiniti, col_full_infiniti, visittedArr);
        }
        
        size_matrix--;
    }
    //cost = cost + reduce_value(matrix, row, col, row_full_infiniti, col_full_infiniti);

    int index_row_not_full[2], index_col_not_full[2], m = 0, n = 0;
    for (int i = 0; i < row; i++)
    {
        if (row_full_infiniti[i] == 0)
        {
            index_row_not_full[m] = i;
            m++;
        }
    }
    for (int j = 0; j < col; j++)
    {
        if (col_full_infiniti[j] == 0)
        {
            index_col_not_full[n] = j;
            n++;
        }
    }
    int flag = 0;
    int indexCheck = index_col_not_full[0];
    while (visittedArr[indexCheck] == 1)//check subpath
    {
        indexCheck = path_memory[indexCheck];
        if (indexCheck == index_row_not_full[0])
            flag++;
    }
    if (flag == 0){
        path_memory[index_row_not_full[0]] = index_col_not_full[0];
        path_memory[index_row_not_full[1]] = index_col_not_full[1];
    }
    else
    {
        path_memory[index_row_not_full[0]] = index_col_not_full[1];
        path_memory[index_row_not_full[1]] = index_col_not_full[0];
    }

    int *temp = (int *)malloc((row) * sizeof(int)); // length of arr = row
    for (int i = 0; i < row; i++)
    {
        temp[i] = path_memory[i]; // copy path_memory -> tmp
    }

    int index = 0; // start point is vertext 0
    int count = 0;
    path_memory[count] = index;
    while (temp[index] != 0)
    {
        index = temp[index];
        count++;
        path_memory[count] = index;
    }
    path_memory[count + 1] = 0; // after juorney, return start poin 0
    return cost;
}