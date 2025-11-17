#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef STRUCTURE_H
#define STRUCTURE_H

typedef struct list_int {//lista enlazada para almacenar posiciones de secuencias
    int pos;
    struct list_int *next;
} ListInt;

typedef struct Node_ {//nodo del trie
    struct Node_ *A;
    struct Node_ *C;
    struct Node_ *G;
    struct Node_ *T;
    ListInt *positions;
} Node_;

typedef struct Trie_ {//trie para almacenar secuencias de ADN
    Node_ *root;
    int height;
} Trie_;

typedef struct GeneInfo {//estructura para almacenar una secuencia y sus posiciones
    char *gene;
    ListInt *positions;
    int count;
    struct GeneInfo *next;
} GeneInfo;

#endif