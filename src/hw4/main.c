#include "priorityQueue.h"
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
            free(g);
            return NULL;
        }
        
        connectVertices(g, v1 - 1, v2 - 1, value);
    }
    
    return g;
}

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

// int inputAll(FILE* f, int* n, int* m, Graph* g, )

int main(void)
{
    FILE* f = fopen("input.txt", "r");

    if (f == NULL) {
        printf("...something went wrong\n");
        return -1;
    }

    int n, m;
    if (fscanf(f, "%d %d", &n, &m) != 2) {
        printf("input is wrong\n");
        return -1;
    }
    
    Graph* g = inputGraph(f, n, m);
    if (g == NULL) {
        // сюда попадает и случай, когда в графе нет вершин. Рассматривать его я не буду, тк не считаю его корректным
        fclose(f);
        return -1;
    }

    
    int k;

    if (fscanf(f, "%d", &k) != 1) {
        printf("input is wrong\n");
        freeGraph(g);
        fclose(f);
        return -1;
    }

    int* capitals = calloc(k, sizeof(int));
    int* typeOfCity = calloc(n, sizeof(int));

    if (capitals == NULL || typeOfCity == NULL) {
        if (!capitals)
            free(capitals);
        if (!typeOfCity)
            free(typeOfCity);
        fclose(f);
        freeGraph(g);
        return -1;
    }

    for (int i = 0; i < n; ++i)
        typeOfCity[i] = -1;

    for (int i = 0; i < k; ++i) {
        int cap;
        if (fscanf(f, "%d", &cap) != 1) {
            free(capitals);
            free(typeOfCity);
            freeGraph(g);
            fclose(f);
            return -1;
        }
        capitals[i] = cap - 1;
    }

    fclose(f);

    // ввод закончен, считаю его корректным
    // теперь нужно создать массив приоритетных очередей, в каждый поместить столицу с нулевым весом
    // когда буду делать обходы, нужно следить, чтобы не было добавления уже посещенных городов +
    // когда достаю из очереди новое значение, его тоже проверяю - город мог быть посещён другим городом

    // capitals - список столиц (для сохранения очередности)
    // typeOfCity - каждому городу по индексу присваивается принадлежность какому-то гос-ву (иначе -1)
    // allQueues - все очереди (обращаться по индексу, в котором столица лежит в capitals)
    // g - исходный граф (можно смотреть всех соседей по имитации итератора)

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

    for (int i = 0; i < k; ++i) {
        printf("%d: ", capitals[i] + 1);
        for (int j = 0; j < n; ++j) {
            if (typeOfCity[j] == capitals[i])
                printf("%d ", j + 1);
        }
        printf("\n");
    }
    for (int i = 0; i < n; ++i)
        printf("%d ", typeOfCity[i]);
    printf("\n");

    freeAll(capitals, typeOfCity, g, allQueues, k);

    // n - число городов, m - число дорог
    // k - число столиц
    // каждой столице по очереди даём ближайший город
    // т.е. Дейкстра? для каждого
    // => нада массив принадлежности каждой столице
    // каждой вершинке нужна очередь с приоритетами, НО верхняя вершина может быть уже занята, хуня
    // т.е. дохера памяти занято фигней. В худшем случае, затраты k * n. С другой стороны, оч много операций без этого
    // Общая ситуация: компонент связности, постоянно держать в памяти доступные вершины с их "весами" для каждого k
    // При добавлении новой вершины, добавляю новые вершинки, идущие от неё
    // После ввода инфы должна быть заполненная структура графа 
    return 0;
}

