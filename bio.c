#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
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
void free_trie(Node_ *node);

int main(int argc, char *argv[])
{
    if(argc<2)
    {
        printf("Argumentos insuficientes, escriba help \n");
        return 1;
    }

    Trie_ *trie=NULL;
    int command=CharToInt(argv[1]);

    switch (command)
    {
        case help:
            printf ("Comandos disponibles\n");
            printf ("<start> <numero> para crear un arbol de altura <numero>\n");
            printf ("<read> <archivo.txt> para leer el arbol de ADN desde un archivo\n");
            printf ("<search> <secuencia> para buscar una secuencia de ADN \n");
            printf ("<max> para buscar el/los genes con maxima frecuencia\n");
            printf ("<min> para buscar el/los genes con minima frecuencia\n");
            printf ("<all> para mostrar todos los genes encontrados\n");
            printf ("<exit> para liberar memoria y salir\n");     
            break;
            
        case start:
            if(argc < 3)
            {
                printf("Debe especificar la altura del arbol\n");
                return 1;
            }
            trie = trie_create(CharToNum(argv[2]));
            if(trie)
                printf("Arbol creado con tamaño %d\n", trie->height);
            else
                printf("Error creando arbol\n");
            break;
            
        case read:
            if(argc < 3)
            {
                printf("Debe especificar el archivo a leer\n");
                return 1;
            }
            trie = load_trie("biodata");
            
            if(trie)
                printf("Secuencia S leida del archivo\n");
            else
                printf("Error cargando arbol\n");
            
            break;
            
        case search:
            if(argc < 3)
            {
                printf("Debe especificar la secuencia a buscar\n");
                return 1;
            }
            trie = load_trie("biodata");
            if(trie){
                if((int)strlen(argv[2]) != trie->height)
                {
                    printf("La secuencia debe tener longitud %d\n", trie->height);
                }
                else
                {
                    bio_search(trie, argv[2]);
                }
            } else {
                printf("Error cargando arbol\n");
            }
            break;
            
        case max:
            trie = load_trie("biodata");
            if(trie){
                bio_max(trie);
            } else {
                printf("Error cargando arbol\n");
            }
            break;
            
        case min:
            trie = load_trie("biodata");
            if(trie){
                bio_min(trie);
            } else {
                printf("Error cargando arbol\n");
            }
            break;
            
        case all:
            trie = load_trie("biodata");
            if(trie){
                bio_all(trie);
            } else {
                printf("Error cargando arbol\n");
            }
            break;
            
        case exit:
            trie = load_trie("biodata");
            if(trie){
                if(trie->root)
                    free_trie(trie->root);
                free(trie);
                printf("Limpiando cache y saliendo...\n");
            }
            remove("biodata");
            break;
            
        default:
            printf("Comando no reconocido. Use 'help' para ver comandos disponibles\n");
            break;
    }

    return 0;
}

int CharToNum(char *str)
{
    int num = atoi(str);
    if(num < 0){
        num*= -1;
    }   
    return num;
}

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

void free_trie(Node_ *node)
{
    if(node == NULL)
        return;

    free_trie(node->A);
    free_trie(node->C);
    free_trie(node->G);
    free_trie(node->T);
    
    ListInt *current = node->positions;
    while(current != NULL)
    {
        ListInt *temp = current;
        current = current->next;
        free(temp);
    }

    free(node);
}