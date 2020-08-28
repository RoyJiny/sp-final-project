#ifndef UTILS_H
#define UTILS_H


#include "spmat.h"
#include <math.h>

char handle_errors(Error error, char *name);

double dot_product(double *row1, double *row2, int size);

/*input file -----> adjacency matrix
 *save the degree of each vertex in degree (by increasing order)*/
Error read_input(FILE *input, spmat *A, int *degree, int nof_vertex);

/**/
Error compute_modularity_matrix_row(spmat *A, int row, group *g, int *degrees, double M, double *B_g_row);

double compute_modularity_value(spmat *B_g, double *s);

Error power_iteration(spmat *mat, double *vector );

int calculate_eigen_value(spmat *mat, double *eigen_vector);/*TODO: substrat C_1norm*/

/*determine the partition into 2 groups, and calc the s vector*/
void eigen2s(double *eigen, int *g1 ,int *g2 , double* s, int size);

#endif