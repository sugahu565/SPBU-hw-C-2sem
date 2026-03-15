#include "dijkstra.h"
#include "graph.h"
#include "readFile.h"
#include<stdio.h>
#include<stdlib.h>

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

