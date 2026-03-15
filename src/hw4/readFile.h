#pragma once

#include "graph.h"
#include <stdio.h>

Graph* inputGraph(FILE* f, int sizeGraph, int numEdges);

int readInput(const char* filename, Graph** g, int* n, int* m, int* k, int** capitals, int** typeOfCity);

