#pragma once

typedef struct Graph Graph;

Graph* initGraph(int n);
void freeGraph(Graph* g);

int connectVertices(Graph* g, int v1, int v2, int value);

void startGetNeighbors(Graph* g, int vertex);
int getNeighbor(Graph* g, int* value);

