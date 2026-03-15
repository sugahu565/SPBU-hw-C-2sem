#include "graph.h"
#include <stdio.h>
#include <stdlib.h>

struct Graph {
    int sizeMatrix;
    int** matrix;
    int curVertex;
    int lastNeighbor;
};

int** initMatrix(int n)
{
    int** matrix = malloc(n * sizeof(int*));
    if (matrix == NULL)
        return NULL;

    int* data = calloc(n * n, sizeof(int));
    if (data == NULL) {
        free(matrix);
        return NULL;
    }

    for (int i = 0; i < n; ++i)
        matrix[i] = &data[i * n];

    return matrix;
}

void freeMatrix(int** matrix)
{

    free(matrix[0]);
    free(matrix);
}

Graph* initGraph(int n)
{
    Graph* g = malloc(sizeof(Graph));
    if (g == NULL)
        return NULL;

    g->matrix = initMatrix(n);
    if (g->matrix == NULL) {
        free(g);
        return NULL;
    }

    g->sizeMatrix = n;
    g->curVertex = -1;
    g->lastNeighbor = -1;

    return g;
}

void freeGraph(Graph* g)
{
    if (g == NULL)
        return;

    if (g->matrix != NULL)
        freeMatrix(g->matrix);

    free(g);
}

int connectVertices(Graph* g, int v1, int v2, int value)
{
    if (g == NULL || v1 < 0 || v1 >= g->sizeMatrix || v2 < 0 || v2 >= g->sizeMatrix)
        return -1;

    g->matrix[v1][v2] = value;
    g->matrix[v2][v1] = value;

    return 0;
}

void startGetNeighbors(Graph* g, int vertex)
{
    if (g == NULL || vertex < 0 || vertex >= g->sizeMatrix)
        return;

    g->curVertex = vertex;
    g->lastNeighbor = 0;
}

int getNeighbor(Graph* g, int* value)
{
    if (g == NULL || g->curVertex == -1 || g->lastNeighbor == -1)
        return -1;

    int i = g->lastNeighbor;
    while (i < g->sizeMatrix && g->matrix[g->curVertex][i] == 0)
        i++;

    if (i >= g->sizeMatrix) {
        g->lastNeighbor = -1;
        g->curVertex = -1;
        return -1;
    }

    *value = g->matrix[g->curVertex][i];
    g->lastNeighbor = i + 1;

    return i;
}
