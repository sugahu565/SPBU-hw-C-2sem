#include "dijkstra.h"
#include "graph.h"
#include "readFile.h"
#include <stdio.h>
#include <stdlib.h>

int expected1[] = { 0, 0, 2 };

int expected2[] = { 0, 0, 0, 4, 4 };

int expected3[] = { 0, 0, 0, 3, 3, 3 };

int expected4[] = { 1, 1, 1, 3, 1, 5 };

int solveFunction(char* nameTestFile, const int* expected)
{
    Graph* g = NULL;
    int n, m, k;
    int* capitals = NULL;
    int* typeOfCity = NULL;

    if (readInput(nameTestFile, &g, &n, &m, &k, &capitals, &typeOfCity) != 0) {
        printf("input is wrong\n");
        return -1;
    }

    int err = solve(g, k, capitals, typeOfCity);

    if (err == -1)
        return -1;

    int correctAnswer = 1;
    for (int i = 0; i < n; ++i) {
        if (expected[i] != typeOfCity[i]) {
            correctAnswer = 0;
            break;
        }
    }

    freeAll(capitals, typeOfCity, g, NULL, 0);
    return correctAnswer;
}

int main(void)
{
    int passedTests = 0;

    int err = solveFunction("input_files/input1.txt", expected1);
    if (err == -1)
        printf("...something went wrong\n");
    else if (err == 0)
        printf("Test 1 failed\n");
    else {
        printf("Test 1 passed\n");
        passedTests++;
    }

    err = solveFunction("input_files/input2.txt", expected2);
    if (err == -1)
        printf("...something went wrong\n");
    else if (err == 0)
        printf("Test 2 failed\n");
    else {
        printf("Test 2 passed\n");
        passedTests++;
    }

    err = solveFunction("input_files/input3.txt", expected3);
    if (err == -1)
        printf("...something went wrong\n");
    else if (err == 0)
        printf("Test 3 failed\n");
    else {
        printf("Test 3 passed\n");
        passedTests++;
    }

    err = solveFunction("input_files/input4.txt", expected4);
    if (err == -1)
        printf("...something went wrong\n");
    else if (err == 0)
        printf("Test 4 failed\n");
    else {
        printf("Test 4 passed\n");
        passedTests++;
    }

    printf("Passed %d tests!\n", passedTests);
    return 0;
}
