#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structure.h"

int check_adn_char(char c);
Node_ *create_node();
void add_position(Node_ *node, int pos);
void insert_gen(Trie_ *trie, char *gene, int position);
void insert_adn(Trie_ *trie, char *c);

int check_adn_char(char c)
{
    if(c != 'A' && c != 'C' && c != 'G' && c != 'T')
    {
        return 0;
    }
    return 1;
}

int check_adn_txt(char *sequence)
{
    int n = strlen(sequence);
    for(int i = 0; i < n; i++){
        if(!check_adn_char(sequence[i])){
            printf("Caracter invalido: %c\n", sequence[i]);
        }
    }
    for(int i = 0; i < n; i++){
        if(!check_adn_char(sequence[i])){
            return 0;
        }
    }
    return 1;
}

void insert_adn(Trie_ *trie, char *c)
{
    int n = strlen(c);
    printf("tamano cadena: %d\n", n);
    int m = trie->height;
    for(int i = 0; i <= n - m; i++)
    { 
        printf("pos %d: ", i);
        putchar(c[i]);
        for(int j = 1; j < m; j++)
        {
            if(j+i >= n)
            {
                break;
            }
            putchar(c[i+j]);
        }
        printf("\n");
        
        if(i + m <= n)
        {
            char sub[m+1];
            strncpy(sub, &c[i], m);
            sub[m] = '\0';
            insert_gen(trie, sub, i);
        }
    }
    printf("Inserting base %s into trie\n", c);
}

Node_* create_node()
{
    Node_ *newNode = (Node_*)malloc(sizeof(Node_));

    if(!newNode)
    {
        return NULL;
    }

    newNode->A = NULL;
    newNode->C = NULL;
    newNode->G = NULL;
    newNode->T = NULL;
    newNode->positions = NULL;

    return newNode;
}

void add_position(Node_ *node, int pos)
{
    ListInt *new_pos = (ListInt*)malloc(sizeof(ListInt));
    if (!new_pos)
    {
        return;
    }
    new_pos->pos = pos;
    new_pos->next = node->positions;
    node->positions = new_pos;
}

void insert_gen(Trie_ *trie, char *gene, int position)
{
    //Si el trie no tiene root, esta se crea
    if (!trie->root)
    {
        trie->root = create_node();
        return;
    }

    Node_ *current = trie->root;

    //Se recorre el gen caracter por caracter
    for(int i = 0; i < trie->height; i++)
    {
        //Dependiendo del caracter se elije el hijo
        Node_ **next = NULL;
        if(gene[i] == 'A')
        {
            next = &current->A;
        }
        else if(gene[i] == 'C')
        {
            next = &current->C;
        }
        else if(gene[i] == 'G')
        {
            next = &current->G;
        }
        else if(gene[i] == 'T')
        {
            next = &current->T;
        }

        if (*next == NULL)
        {
            *next = create_node();
        }
        current = *next;
    }

    //Si se llega a la hoja, se agrega la posicion
    add_position(current, position);
}
