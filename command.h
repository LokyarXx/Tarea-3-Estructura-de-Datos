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

int bio_read(const char *filename, const char *source)
{
    FILE *data = fopen(filename, "a+");
    if (!data)
    {
        return 1;
    }
    int height;
    if (fscanf(data, "altura:%d\n", &height) != 1)
    {
        fclose(data);
        return 1;
    }

    fclose(data);

    data = fopen(filename, "w");
    if (!data)
    {
        return 1;
    }
    fprintf(data, "altura:%d\n", height);
    fprintf(data, "fuente:%s\n", source);
    
    fclose(data);
    return 0;
}

Trie_ *load_trie(const char *filename)
{
    FILE *data = fopen(filename, "r");
    if (!data)
    {
        return NULL;
    }
    int height;
    char source[50];
    if (fscanf(data, "altura:%d\n", &height) != 1)
    {
        fclose(data);
        return NULL;
    }
    if(fscanf(data, "fuente:%49s\n", source) != 1)
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

    FILE *adn = fopen(source, "r");
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

void bio_max(Trie_ *trie)
{
    if(!trie || !trie->root)
    {
        printf("Arbol vacio\n");
        return;
    }

    GeneInfo *gene_list = NULL;
    char *current_gene = (char*)malloc((trie->height + 1) * sizeof(char));
    current_gene[0] = '\0';

    traverse_trie(trie->root, current_gene, 0, trie->height, &gene_list);

    if(gene_list == NULL)
    {
        printf("No hay genes en la secuencia\n");
        free(current_gene);
        return;
    }

    int max_count = 0;
    GeneInfo *current = gene_list;
    while(current != NULL)
    {
        if(current->count > max_count)
        {
            max_count = current->count;
        }
        current = current->next;
    }

    current = gene_list;
    while(current != NULL)
    {
        if(current->count == max_count)
        {
            printf("%s ", current->gene);
            
            ListInt *reversed = NULL;
            ListInt *pos = current->positions;
            while(pos != NULL)
            {
                ListInt *temp = (ListInt*)malloc(sizeof(ListInt));
                temp->pos = pos->pos;
                temp->next = reversed;
                reversed = temp;
                pos = pos->next;
            }
            
            pos = reversed;
            while(pos != NULL)
            {
                printf("%d", pos->pos);
                if(pos->next != NULL)
                    printf(" ");
                pos = pos->next;
            }
            printf("\n");
            
            while(reversed != NULL)
            {
                ListInt *temp = reversed;
                reversed = reversed->next;
                free(temp);
            }
        }
        current = current->next;
    }

    free(current_gene);
    free_gene_list(gene_list);
}

void bio_min(Trie_ *trie)
{
    if(!trie || !trie->root)
    {
        printf("Arbol vacio\n");
        return;
    }

    GeneInfo *gene_list = NULL;
    char *current_gene = (char*)malloc((trie->height + 1) * sizeof(char));
    current_gene[0] = '\0';

    traverse_trie(trie->root, current_gene, 0, trie->height, &gene_list);

    if(gene_list == NULL)
    {
        printf("No hay genes en la secuencia\n");
        free(current_gene);
        return;
    }

    int min_count = INT_MAX;
    GeneInfo *current = gene_list;
    while(current != NULL)
    {
        if(current->count < min_count)
        {
            min_count = current->count;
        }
        current = current->next;
    }

    current = gene_list;
    while(current != NULL)
    {
        if(current->count == min_count)
        {
            printf("%s ", current->gene);
            
            ListInt *reversed = NULL;
            ListInt *pos = current->positions;
            while(pos != NULL)
            {
                ListInt *temp = (ListInt*)malloc(sizeof(ListInt));
                temp->pos = pos->pos;
                temp->next = reversed;
                reversed = temp;
                pos = pos->next;
            }
            
            pos = reversed;
            while(pos != NULL)
            {
                printf("%d", pos->pos);
                if(pos->next != NULL)
                    printf(" ");
                pos = pos->next;
            }
            printf("\n");
            while(reversed != NULL)
            {
                ListInt *temp = reversed;
                reversed = reversed->next;
                free(temp);
            }
        }
        current = current->next;
    }

    free(current_gene);
    free_gene_list(gene_list);
}

void bio_all(Trie_ *trie)
{
    if(!trie || !trie->root)
    {
        printf("Arbol vacio\n");
        return;
    }

    GeneInfo *gene_list = NULL;
    char *current_gene = (char*)malloc((trie->height + 1) * sizeof(char));
    current_gene[0] = '\0';

    traverse_trie(trie->root, current_gene, 0, trie->height, &gene_list);

    if(gene_list == NULL)
    {
        printf("No hay genes en la secuencia\n");
        free(current_gene);
        return;
    }

    print_gene_list(gene_list);

    free(current_gene);
    free_gene_list(gene_list);
}