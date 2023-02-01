#ifndef _TSP_H
#define _TSP_H

extern int min(double *arr, int length, int *skip_index);
extern double reduce_value(double **matrix, int row, int col, int *row_full_infinti, int *col_full_infiniti);
extern int reduce_matrix(double **matrix, int row, int col, int *path_memory, int *row_full_infinti, int *col_full_infiniti, int *visittedArr);
/** 
 * Traversal saleman problem 
 * return path_memory 
 * return total cost */
extern double TSP(double **Matrix, int row, int col, int *path_memory);

#endif