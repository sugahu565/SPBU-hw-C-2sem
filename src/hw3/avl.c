#include "avl.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Node {
    struct Node* leftChild;
    struct Node* rightChild;
    char* value;
    char* name;
    int balance;
} Node;


typedef struct AVL {
    Node* root;
} AVl;


AVL* avlInit(void) {
    AVl tree = malloc(sizeof(AVL));
    tree->root = NULL;
    return tree;
}

Node* rotateLeft(Node* vertex) {
    Node* a = vertex;
    Node* b = vertex->rightChild;
    
}


void rotateRight() {

}


void bigRotateLeft() {

}


void bigRotateRight() {

}

Node* insertRecursion(Node* curVertex, Node* vertex) {
    if (strcmp(vertex->value, curVertex->value)) {      
        if (curVertex->leftChild == NULL) {
            
        }
    } else {

    }
}

int avlInsert(AVL* tree, char* name, char* val) {
    
    Node* vertex = malloc(sizeof(Node));
    if (vertex == NULL)
            return -1;
    vertex->balance = 0;
    vertex->value = val;
    vertex->name = name;
    vertex->leftChild = NULL;
    vertex->rightChild = NULL;
    

    if (tree->root == NULL) {
        tree->root = vertex;
        return 0;
    }


    tree->root = insertRecursion(tree->root, vertex);
    return 0;
}

void avlDelete(AVL* tree, int val) {
    return;
}

int avlFind(AVL* tree, int val) {
    return 0;
}

void avlFree(AVL* tree) {
    return;
}

