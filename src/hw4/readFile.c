#include "readFile.h"
#include "dijkstra.h"
#include "graph.h"
#include <stdio.h>
#include <stdlib.h>

Graph* inputGraph(FILE* f, int sizeGraph, int numEdges)
{

    Graph* g = initGraph(sizeGraph);
    if (g == NULL)
        return NULL;

    for (int i = 0; i < numEdges; ++i) {

        int v1, v2, value;

        if (fscanf(f, "%d %d %d", &v1, &v2, &value) != 3) {
            printf("input is wrong\n");
            freeGraph(g);
            return NULL;
        }

        connectVertices(g, v1 - 1, v2 - 1, value);
    }

    return g;
}

int readInput(const char* filename, Graph** g, int* n, int* m, int* k, int** capitals, int** typeOfCity)
{
    FILE* f = fopen(filename, "r");
    if (f == NULL) {
        printf("...something went wrong\n");
        return -1;
    }

    if (fscanf(f, "%d %d", n, m) != 2) {
        fclose(f);
        return -1;
    }

    *g = inputGraph(f, *n, *m);
    if (*g == NULL) {
        fclose(f);
        return -1;
    }

    if (fscanf(f, "%d", k) != 1) {
        freeAll(NULL, NULL, *g, NULL, 0);
        fclose(f);
        return -1;
    }

    *capitals = calloc(*k, sizeof(int));
    *typeOfCity = malloc(*n * sizeof(int));

    if (*capitals == NULL || *typeOfCity == NULL) {
        freeAll(*capitals, *typeOfCity, *g, NULL, 0);
        fclose(f);
        return -1;
    }

    for (int i = 0; i < *n; ++i)
        (*typeOfCity)[i] = -1;

    for (int i = 0; i < *k; ++i) {
        int cap;
        if (fscanf(f, "%d", &cap) != 1) {
            freeAll(*capitals, *typeOfCity, *g, NULL, 0);
            fclose(f);
            return -1;
        }
        (*capitals)[i] = cap - 1;
    }

    fclose(f);
    return 0;
}
