#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structure.h"
#include "function.h"

Trie_ *trie_create(int height)
{
    Trie_ *newTrie = (Trie_*)malloc(sizeof(Trie_));
    if(!newTrie)
    {
        return NULL;
    }

    FILE *data=fopen("biodata","w");
    if (!data)
    {
        free(newTrie);
        return NULL;
    }
    fprintf(data,"altura:%d\n",height);

    if(!newTrie)
    {
        free(newTrie);
        return NULL;
    }
    newTrie->height = height;
    newTrie->root = NULL;
    
    fclose(data);
    return newTrie;
}

Trie_ *load_trie(const char *filename)
{
    FILE *data = fopen(filename, "r");
    if (!data)
    {
        return NULL;
    }
    int height;
    if (fscanf(data, "altura:%d\n", &height) != 1)
    {
        fclose(data);
        return NULL;
    }
    
    Trie_ *trie = (Trie_*)malloc(sizeof(Trie_));
    if(!trie)
    {
        return NULL;
    }

    trie ->height = height;
    trie ->root = create_node();

    if(!trie->root)
    {
        printf("Error no se pudo crear la raiz\n");
        free(trie);
        return NULL;
    }


    FILE *adn = fopen("adn.txt", "r");
    char buffer[1024];
    if(!adn)
    {
        fclose(data);
        free(trie);
        return NULL;
    }
    while(fgets(buffer, sizeof(buffer), adn))
    {
        buffer[strcspn(buffer, "\n")] = '\0';
        printf("Read sequence: %s\n", buffer);
    }

    if(!check_adn_txt(buffer)){
        printf("Secuencia de ADN invalida\n");
        fclose(adn);
        fclose(data);
        free(trie->root);
        free(trie);
        return NULL;
    }
    insert_adn(trie, buffer);
    
    fclose(adn);
    fclose(data);
    return trie;
}


void bio_search(Trie_ *trie, char *pattern){
    if(!trie || !trie->root){
        printf("Arbol vacio\n");
        return;
    }

    Node_ *current = trie->root;
    int n= strlen(pattern);

    for(int i = 0; i < n; i++){
        char base = pattern[i];
        
        switch(base){
            case 'A':
                if(!current->A){
                    printf("Patron no encontrado\n");
                    return;
                }
                current = current->A;
                break;
            case 'C':
                if(!current->C){
                    printf("Patron no encontrado\n");
                    return;
                }
                current = current->C;
                break;
            case 'G':
                if(!current->G){
                    printf("Patron no encontrado\n");
                    return;
                }
                current = current->G;
                break;
            case 'T':
                if(!current->T){
                    printf("Patron no encontrado\n");
                    return;
                }
                current = current->T;
                break;
        }
    }

    if(current->positions == NULL){
        printf("Patron no encontrado\n");
        return;
    }

    ListInt *posList = current->positions;
    while(posList){
        printf("%d\t", posList->pos);
        posList = posList->next;
    }
    printf("\n");
}