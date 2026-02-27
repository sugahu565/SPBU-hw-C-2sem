#include "fileFunctions.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define max(a, b) ((a) > (b) ? (a) : (b))

char* fscanfTable(FILE* f, int** maxLen, int* numHeaderOut, int* lenTableOut)
{
    int c = fgetc(f);
    int numHeader = 1;

    // считаю количество стобцов, чтобы в массиве maxLen держать макс длину каждого столбца
    while (c != '\n' && c != EOF) {
        if (c == ',')
            numHeader++;
        c = fgetc(f);
    }

    *maxLen = calloc(numHeader, sizeof(int));
    *numHeaderOut = numHeader;

    if (*maxLen == NULL)
        return NULL;

    rewind(f);

    int lenTable = 0, capacity = 32;
    char* buffer = malloc(capacity * sizeof(char));

    if (buffer == NULL) {
        free(*maxLen);
        return NULL;
    }

    c = fgetc(f);
    int curHeader = 0, curLen = 0;

    while (c != EOF) {
        // пытаюсь найти макс. ширину каждого столбца
        if (c != ',' && c != '\n')
            curLen++;

        if (c == ',') {
            (*maxLen)[curHeader] = max((*maxLen)[curHeader], curLen);
            curHeader++;
            curLen = 0;
        } else if (c == '\n') {
            (*maxLen)[curHeader] = max((*maxLen)[curHeader], curLen);
            curHeader = 0;
            curLen = 0;
        }

        // далее записываю символ в buffer

        lenTable++;

        if (lenTable == capacity) {
            capacity *= 2;
            char* temp = realloc(buffer, capacity);

            if (temp == NULL) {
                free(buffer);
                free(*maxLen);
                return NULL;
            }

            buffer = temp;
        }

        if (curHeader == numHeader && c != '\n') {
            while (c != EOF && c != '\n')
                c = fgetc(f);
            if (c == EOF) {
                lenTable--;
                break;
            }
            curHeader = 0;
            curLen = 0;
        }

        buffer[lenTable - 1] = (char)c;
        c = fgetc(f);
    }
    if (curHeader != numHeader)
        (*maxLen)[curHeader] = max((*maxLen)[curHeader], curLen);

    char* temp = realloc(buffer, lenTable + 1);
    if (temp == NULL) {
        free(buffer);
        free(*maxLen);
        return NULL;
    }
    buffer = temp;
    buffer[lenTable] = '\0';
    *lenTableOut = lenTable;
    return buffer;
}

void fprintfTopTable(FILE* f, const int* maxLen, int numHeader)
{
    // вывод в файл верхушки таблицы
    fputc('+', f);
    for (int i = 0; i < numHeader; ++i) {
        for (int j = 0; j < maxLen[i] + 2; ++j)
            fputc('=', f);
        fputc('+', f);
    }
}

char* createLine(const int* maxLen, int numHeader, int width)
{
    // создаю линию между строками таблицы, чтобы выводить сразу же через fprintf

    char* line = malloc((width + 1) * sizeof(char));

    if (line == NULL)
        return NULL;

    int curSymb = 0;
    for (int curHeader = 0; curHeader < numHeader; ++curHeader) {
        line[curSymb] = '+';
        curSymb++;
        for (int j = 0; j < maxLen[curHeader] + 2; ++j) {
            line[curSymb] = '-';
            curSymb++;
        }
    }
    line[curSymb] = '+';
    line[width] = '\0';
    return line;
}

int fprintfCell(FILE* f, char* table, int* startSymb, const int* maxLen, int curHeader)
{
    /*
    фукнция считывает ячейку до знака переноса или разделителя, вписывает
    её в файл с нужным выравниванием, а потом возвращает 1, если ячейка кончилась разделителем, или 0, если переносом.
    Также startSymb, после исп функции, стоит на знаке разделителя/переноса
    */

    int start = *startSymb;
    int end = start;
    int numDigits = 0;

    while (table[end] != '\n' && table[end] != ',' && table[end] != '\0') {
        numDigits += (('0' <= table[end] && table[end] <= '9') || table[end] == '.');
        end++;
    }

    int len = end - start;

    if (numDigits == len)
        fprintf(f, " %*.*s |", maxLen[curHeader], len, table + start);
    else
        fprintf(f, " %-*.*s |", maxLen[curHeader], len, table + start);

    *startSymb = end;
    return table[end] == ',' ? 1 : 0;
}

void emptyCell(FILE* f, int len)
{
    fprintf(f, " %*s |", len, "");
}

int fprintfTable(FILE* f, char* table, int lenTable, const int* maxLen, int numHeader)
{
    int width = 2; // "| " символы в начале
    for (int i = 0; i < numHeader; ++i)
        width += maxLen[i] + 3; // + символы " | " между столбцами;
    width--;

    // линия между строками таблицы
    char* line = createLine(maxLen, numHeader, width);
    if (line == NULL)
        return -1;

    // тут начинаю вывод таблицы
    int curHeader = 0;

    // каждая итерация большого цикла - строка таблицы
    fprintfTopTable(f, maxLen, numHeader);
    int i = 0;
    int flag = 1;
    while (i < lenTable) {
        fprintf(f, "\n|");
        while (i < lenTable && table[i] != '\n') {
            int nextCell = fprintfCell(f, table, &i, maxLen, curHeader); // вывод ячейки, i на ',' или '\n'
            curHeader++; // следующий заголовок

            while (curHeader == numHeader && i < lenTable && table[i] != '\n') // лишние ячейки в строке
                i++;

            if (nextCell) // если есть след ячейка
                i++; // ставлю i на начало ячейки
        }
        for (int j = curHeader; j < numHeader; ++j)
            emptyCell(f, maxLen[j]);
        curHeader = 0;
        if (flag) {
            fputc('\n', f);
            fprintfTopTable(f, maxLen, numHeader);
            flag = 0;
        } else
            fprintf(f, "\n%s", line);
        i++;
    }
    free(line);
    return 0;
}

int fwriteTable(FILE* input, FILE* output)
{
    if (input == NULL || output == NULL)
        return -1;

    int* maxLen = NULL;
    int numHeader = 0;
    int lenTable = 0;

    char* table = fscanfTable(input, &maxLen, &numHeader, &lenTable);

    fclose(input);

    if (table == NULL) {
        fclose(output);
        return -1;
    }

    fprintfTable(output, table, lenTable, maxLen, numHeader);

    fclose(output);

    free(maxLen);
    free(table);

    return 0;
}
