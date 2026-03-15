#pragma once
#include "graph.h"
#include "priorityQueue.h"

void freeAll(int* capitals, int* typeOfCity, Graph* g, priorQueue** allQueues, int numAllocQueues);
int solve(Graph* g, int k, int* capitals, int* typeOfCity);
