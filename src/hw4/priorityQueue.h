#pragma once

typedef struct PriorQueue PriorQueue;

PriorQueue* initQueue(void);

int insertMin(PriorQueue* q, int key, int value);

void deleteMin(PriorQueue* q);

int getMin(PriorQueue* q);

int sizeQueue(PriorQueue* q);

void freeQueue(PriorQueue* q);
