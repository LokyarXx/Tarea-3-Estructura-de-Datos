#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "command.h"

#define help 168620
#define start 1810284
#define read 178498
#define search 17952822
#define min 17388
#define max 17318
#define all 16216
#define exit 167494

int CharToNum(char *str);
int CharToInt(char *str);

int main(int argc, char *argv[])
{
    if(argc<2)
    {
        printf("Not enough arguments\n");
        return 1;
    }

    Trie_ *trie=NULL;
    int command=CharToInt(argv[1]);

    switch (command)
    {
        case help:
            printf("Help command selected\n");
            break;
        case start:
            trie = trie_create(CharToNum(argv[2]));
            printf("Trie creado con altura %d\n", trie->height);
            break;
        case read:
            trie = load_trie("biodata");
            if(trie)
            {
                printf("Trie cargado con altura %d\n", trie->height);
            }
            else
            {
                printf("Error loading trie\n");
            }
            break;
        case search:
            break;
        default:
            break;
    }

    //printf("%d",CharToInt(argv[1]));
    return 0;
}

//transforma un string en un numero
int CharToNum(char *str)
{
    return atoi(str);
}

//transforma un string en un entero
int CharToInt(char *str)
{
    int result=0;
    
    while(*str)
    {
        result = result * 10 + (*str + '0');
        str++;
    }
    return result;
}
