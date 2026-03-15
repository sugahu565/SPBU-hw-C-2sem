#include "priorityQueue.h"
#include<stdio.h>
#include<stdlib.h>

typedef struct Node {
    int key;
    int value;
} Node;

struct priorQueue {
    int sizeAlloc;
    int nextNode;
    Node* allNodes;
};

priorQueue* initQueue(void) {
    priorQueue* q = malloc(sizeof(priorQueue));
    q->sizeAlloc = 0;
    q->nextNode = 0;
    q->allNodes = NULL;
    return q;
}

void siftUp(priorQueue* q, int indexNode) {
    // q->allNodes + curNode * sizeof(Node) = указатель на ноду в списке нодов
    if (indexNode == 0)
        return;

    Node* curNode = &q->allNodes[indexNode];
    Node* parent = &q->allNodes[(indexNode - 1) / 2];
    
    if (curNode->key < parent->key) {

        Node temp = *curNode;
        *curNode = *parent;
        *parent = temp;

        siftUp(q, (indexNode - 1) / 2);
    }
}

void siftDown(priorQueue* q, int indexNode) {

    int lChild = indexNode * 2 + 1;
    int rChild = indexNode * 2 + 2;

    if (lChild >= q->nextNode)
        return;

    Node* curNode = &q->allNodes[indexNode];
    Node* minChild = &q->allNodes[lChild];
    int minChildIndex = lChild;

    if (rChild < q->nextNode && q->allNodes[rChild].key < minChild->key) {
        minChild = &q->allNodes[rChild];
        minChildIndex = rChild;
    }

    if (curNode->key > minChild->key) {
        Node temp = *curNode;
        *curNode = *minChild;
        *minChild = temp;
        siftDown(q, minChildIndex);
    }
}

int insertMin(priorQueue* q, int key, int value) {

    if (q->sizeAlloc == 0) {
        q->allNodes = malloc(sizeof(Node));
        if (q->allNodes == NULL)
            return -1;
        q->sizeAlloc = 1;
        q->nextNode = 0; // where put new node
    }

    if (q->nextNode >= q->sizeAlloc) {
        Node* temp = realloc(q->allNodes, sizeof(Node) * q->sizeAlloc * 2);
        if (temp == NULL)
            return -1;
        q->allNodes = temp;
        q->sizeAlloc *= 2;
    }

    // теперь q->nextNode указывает туда, куда можно запихать новую ноду
    
    q->allNodes[q->nextNode].key = key;
    q->allNodes[q->nextNode].value = value;

    siftUp(q, q->nextNode);
    q->nextNode++;
    return 0;
}

void deleteMin(priorQueue* q) {
    if (q->nextNode == 0)
        return;
    q->allNodes[0].key = q->allNodes[q->nextNode - 1].key;
    q->allNodes[0].value = q->allNodes[q->nextNode - 1].value;
    q->nextNode--;
    
    siftDown(q, 0);
}

int getMin(priorQueue* q) {
    if (q->nextNode == 0)
        return -1;
    return q->allNodes[0].value;
}

int sizeQueue(priorQueue* q) {
    return q->nextNode;
}

void freeQueue(priorQueue* q) {
    if (q->allNodes != NULL)
        free(q->allNodes);
    free(q);
}

