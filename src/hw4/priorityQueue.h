#pragma once

typedef struct priorQueue priorQueue;

priorQueue* initQueue(void);

int insertMin(priorQueue* q, int key, int value);

void deleteMin(priorQueue* q);

int getMin(priorQueue* q);

int sizeQueue(priorQueue* q);

void freeQueue(priorQueue* q);
