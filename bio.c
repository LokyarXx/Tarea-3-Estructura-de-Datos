#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "command.h"

#define help 168620
#define create 16485289
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
    Trie_ *trie=NULL;
    int command=CharToInt(argv[1]);

    switch (command)
    {
        case help:// muestra los comandos disponibles
            printf ("Comandos disponibles\n");
            printf("<create> <longitud> para crear una nueva secuencia de ADN aleatoria de longitud <longitud>\n");
            printf ("<start> <numero> para crear un arbol de altura <numero>\n");
            printf ("<read> <archivo.txt> para leer el arbol de ADN desde un archivo\n");
            printf ("<search> <secuencia> para buscar una secuencia de ADN \n");
            printf ("<max> para buscar el/los genes con maxima frecuencia\n");
            printf ("<min> para buscar el/los genes con minima frecuencia\n");
            printf ("<all> para mostrar todos los genes encontrados\n");
            printf ("<exit> para liberar memoria y salir\n");     
            break;
        case create:// generar nueva secuencia de ADN
            if(argc < 3)
            {
                printf("Debe especificar la longitud de la secuencia de ADN a generar\n");
                return 1;
            }
            create_sequence(CharToNum(argv[2]));
            break;
        case start:// crear arbol con altura especificada
            if(argc < 3)
            {
                printf("Debe especificar la altura del arbol\n");
                return 1;
            }
            if(trie_create(CharToNum(argv[2])) == 0)
            {
                printf("Arbol de altura %s creado correctamente\n", argv[2]);
            }
            else
            {
                printf("Error creando el arbol\n");
            }
            break;
            
        case read:// leer arbol desde archivo txt
            if(argc < 3)
            {
                printf("Debe especificar el nombre del archivo\n");
                return 1;
            }
            if(!FileExists(argv[2]))
            {
                printf("El archivo %s no existe\n", argv[2]);
                return 1;
            }
            if(bio_read("biodata", argv[2]) == 0)
            {
                printf("Archivo %s leido correctamente\n", argv[2]);
            }
            else
            {
                printf("Error leyendo el archivo %s\n", argv[2]);
            }
            
            break;
            
        case search:// buscar secuencia en el arbol
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
                printf("Error: arbol no creado\n");
            }
            break;
            
        case max:// buscar secuencia con maxima frecuencia con sus posiciones
            trie = load_trie("biodata");
            if(trie){
                bio_max(trie);
            } else {
                printf("Error: arbol no creado\n");
            }
            break;
            
        case min:// buscar secuencia con minima frecuencia con sus posiciones
            trie = load_trie("biodata");
            if(trie){
                bio_min(trie);
            } else {
                printf("Error: arbol no creado\n");
            }
            break;
            
        case all:// mostrar todas las secuencias en el arbol y sus posiciones
            trie = load_trie("biodata");
            if(trie){
                bio_all(trie);
            } else {
                printf("Error: arbol no creado\n");
            }
            break;
            
        case exit:// libera la memoria y sale
            trie = load_trie("biodata");
            if(trie){
                if(trie->root)
                    free_trie(trie->root);
                free(trie);
                printf("Limpiando cache y saliendo...\n");
            }
            if(remove("biodata") != 0)
            {
                printf("Error al eliminar el archivo biodata\n");
            }
            break;
            
        default:// caso en que el comando no sea reconocido
            printf("Comando no reconocido. Use './bio help' para ver comandos disponibles\n");
            break;
    }

    return 0;
}

int CharToNum(char *str)// convierte cadena a numero entero positivo
{
    int num = atoi(str);
    if(num < 0){
        num*= -1;
    }   
    return num;
}

int CharToInt(char *str)// convierte cadena a numero entero
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