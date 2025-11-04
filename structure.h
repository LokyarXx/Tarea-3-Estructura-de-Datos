#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef STRUCTURE_H
#define STRUCTURE_H

typedef struct list_int {
    int pos;
    struct list_int *next;
} ListInt;

typedef struct Node_ {
    struct Node_ *A;
    struct Node_ *C;
    struct Node_ *G;
    struct Node_ *T;
    ListInt *positions;
} Node_;

typedef struct Trie_ {
    Node_ *root;
    int height;
} Trie_;


#endif