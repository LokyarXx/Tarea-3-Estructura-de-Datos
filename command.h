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
    Trie_ *trie = trie_create(height);
    if (!trie)
    {
        fclose(data);
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

    insert_adn(trie, buffer);
    
    fclose(adn);
    fclose(data);
    return trie;
}


