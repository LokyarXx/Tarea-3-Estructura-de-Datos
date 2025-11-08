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
        fclose(data);
        return NULL;
    }

    trie->height = height;
    trie->root = create_node();

    if(!trie->root)
    {
        printf("Error no se pudo crear la raiz\n");
        free(trie);
        fclose(data);
        return NULL;
    }

    FILE *adn = fopen("adn.txt", "r");
    if(!adn)
    {
        fclose(data);
        free(trie->root);
        free(trie);
        return NULL;
    }
    
    char buffer[100000]; 
    buffer[0] = '\0';

    char line[1024];
    while(fgets(line, sizeof(line), adn))
    {
        line[strcspn(line, "\n")] = '\0';
        strcat(buffer, line);
    }

    if(strlen(buffer) == 0)
    {
        printf("Archivo adn.txt vacio\n");
        fclose(adn);
        fclose(data);
        free(trie->root);
        free(trie);
        return NULL;
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

    int n = (int)strlen(pattern);
    
    if(n != trie->height){
        printf("-1\n");
        return;
    }
    
    for(int i = 0; i < n; i++){
        if(!check_adn_char(pattern[i])){
            printf("-1\n");
            return;
        }
    }

    Node_ *current = trie->root;

    for(int i = 0; i < n; i++){
        char base = pattern[i];
        
        switch(base){
            case 'A':
                if(!current->A){
                    printf("-1\n");
                    return;
                }
                current = current->A;
                break;
            case 'C':
                if(!current->C){
                    printf("-1\n");
                    return;
                }
                current = current->C;
                break;
            case 'G':
                if(!current->G){
                    printf("-1\n");
                    return;
                }
                current = current->G;
                break;
            case 'T':
                if(!current->T){
                    printf("-1\n");
                    return;
                }
                current = current->T;
                break;
        }
    }

    if(current->positions == NULL){
        printf("-1\n");
        return;
    }

    ListInt *reversed = NULL;
    ListInt *posList = current->positions;
    
    while(posList){
        ListInt *temp = (ListInt*)malloc(sizeof(ListInt));
        temp->pos = posList->pos;
        temp->next = reversed;
        reversed = temp;
        posList = posList->next;
    }

    posList = reversed;
    while(posList){
        printf("%d", posList->pos);
        if(posList->next != NULL)
            printf(" ");
        posList = posList->next;
    }
    printf("\n");
    
    while(reversed){
        ListInt *temp = reversed;
        reversed = reversed->next;
        free(temp);
    }
}