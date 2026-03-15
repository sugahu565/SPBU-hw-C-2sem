#include "dijkstra.h"
#include "graph.h"
#include<stdio.h>
#include<stdlib.h>

Graph* inputGraph(FILE* f, int sizeGraph, int numEdges) {
    
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



int readInput(const char* filename, Graph** g, int* n, int* m, int* k, int** capitals, int** typeOfCity) {
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

int main(void)
{
    Graph* g = NULL;
    int n, m, k;
    int* capitals = NULL;
    int* typeOfCity = NULL;

    if (readInput("input.txt", &g, &n, &m, &k, &capitals, &typeOfCity) != 0) {
        printf("input is wrong\n");
        return -1;
    }

    // capitals - список столиц (для сохранения очередности)
    // typeOfCity - каждому городу по индексу присваивается принадлежность какому-то гос-ву (иначе -1)
    // allQueues - все очереди (обращаться по индексу, в котором столица лежит в capitals)
    // g - исходный граф (можно смотреть всех соседей по имитации итератора)

    int err = solve(g, n, k, capitals, typeOfCity);

    if (err == -1)
        return -1;

    for (int i = 0; i < k; ++i) {
        printf("%d: ", capitals[i] + 1);
        for (int j = 0; j < n; ++j) {
            if (typeOfCity[j] == capitals[i])
                printf("%d ", j + 1);
        }
        printf("\n");
    }
    freeAll(capitals, typeOfCity, g, NULL, 0);
    return 0;
}

