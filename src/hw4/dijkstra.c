#include "dijkstra.h"
#include "graph.h"
#include "priorityQueue.h"
#include <stdio.h>
#include <stdlib.h>


void freeAllQueues(priorQueue** q, int size) {
    int i = 0;
    while (i < size && q[i] != NULL) {
        freeQueue(q[i]);
        i++;
    }
    free(q);
}

void freeAll(int* capitals, int* typeOfCity, Graph* g, priorQueue** allQueues, int numAllocQueues) {
    if (capitals != NULL)
        free(capitals);
    if (typeOfCity != NULL)
        free(typeOfCity);
    if (g != NULL)
        freeGraph(g);
    if (allQueues != NULL)
        freeAllQueues(allQueues, numAllocQueues);
    
}

int solve(Graph* g, int n, int k, int* capitals, int* typeOfCity) {
    
    priorQueue** allQueues = malloc(sizeof(priorQueue*) * k); 
    if (allQueues == NULL) {
        freeAll(capitals, typeOfCity, g, NULL, 0);
        return -1;
    }

    for (int i = 0; i < k; ++i) { // инициализирую очереди для каждой столицы
        allQueues[i] = initQueue();
        if (allQueues[i] == NULL) {
            freeAll(capitals, typeOfCity, g, allQueues, i + 1);
            return -1;
        }

    }

    for (int i = 0; i < k; ++i) { // добавляю в каждую очередь соответствующую столицу
        int err = insertMin(allQueues[i], 0, capitals[i]);
        if (err) {
            freeAll(capitals, typeOfCity, g, allQueues, k);
            return -1;
        }
    }

    int notNullQueue = k;
    int curIndexCapital = 0;
    int curCapital = capitals[curIndexCapital];

    while (notNullQueue > 0) {
       // теперь Дейкстра для каждой столицы - по одной итерации
       // 1. Извлекаю из очереди город с мин стоимостью достижения
       // 2. Добавляю все возможные следующие города в очередь (если ещё не заняты), но их вес - вес ребра до них из текущей вершины
       // 3. Перехожу к след городу
       // Во всех очередях максимум e * k = n * n * k городов, т.к. я не обновляю веса, а просто пихаю всё в очередь почти без разбора 
        
        if (sizeQueue(allQueues[curIndexCapital]) == 0) {
            curIndexCapital = (curIndexCapital + 1) % k;
            curCapital = capitals[curIndexCapital];
            continue;
        }

        int nextVertex = -1; // поиск подходящей следующей вершины на добавление от неё следующих
        while (nextVertex == -1 && sizeQueue(allQueues[curIndexCapital]) != 0) {
            int applicant = getMin(allQueues[curIndexCapital]);
            if (typeOfCity[applicant] == -1)
                nextVertex = applicant;
            else
                deleteMin(allQueues[curIndexCapital]);
        }
    
        if (nextVertex == -1) {
            notNullQueue--;
            curIndexCapital = (curIndexCapital + 1) % k;
            curCapital = capitals[curIndexCapital];
            continue;
        }

        typeOfCity[nextVertex] = curCapital;
        startGetNeighbors(g, nextVertex);

        int val;
        int neigbor = getNeighbor(g, &val);
        while (neigbor != -1) { // добавляем смежные вершины
            if (typeOfCity[neigbor] == -1) {
                int err = insertMin(allQueues[curIndexCapital], val, neigbor);
                if (err != 0) {
                    freeAll(capitals, typeOfCity, g, allQueues, k);
                    return -1;
                }
            }
            neigbor = getNeighbor(g, &val);
        }

        curIndexCapital = (curIndexCapital + 1) % k;
        curCapital = capitals[curIndexCapital];
    }
    freeAllQueues(allQueues, k);
    return 0;
}


