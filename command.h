#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "structure.h"
#include "function.h"

int bio_create(int height){//crea el archivo biodata con la altura del trie
    FILE *data=fopen("biodata","w");
    if (!data)
    {
        return 1;
    }
    fprintf(data,"altura:%d\n",height);
    fclose(data);
    return 0;
}

int bio_read(const char *filename, const char *source){//lee el archivo biodata y actualiza la fuente de datos
    FILE *data = fopen(filename, "a+");
    if (!data){
        return 1;
    }
    int height;
    if (fscanf(data, "altura:%d\n", &height) != 1){//guarda la altura del trie
        fclose(data);
        return 1;
    }

    fclose(data);

    data = fopen(filename, "w");//reescribe el archivo con la nueva fuente
    if (!data){
        return 1;
    }
    //guarda la altura y la fuente de datos
    fprintf(data, "altura:%d\n", height);
    fprintf(data, "fuente:%s\n", source);
    
    fclose(data);
    return 0;
}

Trie_ *load_trie(const char *filename){//carga el trie desde el archivo biodata
    FILE *data = fopen(filename, "r");
    if (!data){
        return NULL;
    }
    int height;
    char source[50];
    if (fscanf(data, "altura:%d\n", &height) != 1){//lee la altura del trie
        fclose(data);
        return NULL;
    }
    if(fscanf(data, "fuente:%49s\n", source) != 1){//lee la fuente de datos
        fclose(data);
        return NULL;
    }
    
    Trie_ *trie = (Trie_*)malloc(sizeof(Trie_));//crea el trie
    if(!trie){
        fclose(data);
        return NULL;
    }

    trie->height = height;//asigna la altura
    trie->root = create_node();//crea el nodo raiz

    if(!trie->root){//verifica si se creo la raiz
        printf("Error no se pudo crear la raiz\n");
        free(trie);
        fclose(data);
        return NULL;
    }

    FILE *adn = fopen(source, "r");
    if(!adn){//verifica si se pudo abrir el archivo de secuencia
        fclose(data);
        free(trie->root);
        free(trie);
        return NULL;
    }
    
    char buffer[100000]; 
    buffer[0] = '\0';

    char line[1024];
    while(fgets(line, sizeof(line), adn)){//lee cada linea del archivo
        line[strcspn(line, "\n")] = '\0';
        strcat(buffer, line);
    }

    if(strlen(buffer) == 0){//verifica si el archivo esta vacio
        printf("Archivo adn.txt vacio\n");
        fclose(adn);
        fclose(data);
        free(trie->root);
        free(trie);
        return NULL;
    }

    if(!check_adn_txt(buffer)){//verifica si la secuencia del archivo es valida
        printf("Secuencia de ADN invalida\n");
        fclose(adn);
        fclose(data);
        free(trie->root);
        free(trie);
        return NULL;
    }
    
    insert_adn(trie, buffer);//inserta la cadenas en el trie
    
    fclose(adn);
    fclose(data);
    return trie;
}

void bio_search(Trie_ *trie, char *pattern){//busca una secuencia en el trie e imprime sus posiciones
    if(!trie || !trie->root){
        printf("Arbol vacio\n");
        return;
    }

    int n = (int)strlen(pattern);//longitud de la secuencia a buscar
    
    if(n != trie->height){//verifica si la longitud es correcta
        printf("-1\n");
        return;
    }
    
    for(int i = 0; i < n; i++){//verifica si los caracteres son validos
        if(!check_adn_char(pattern[i])){
            printf("-1\n");
            return;
        }
    }

    Node_ *current = trie->root;//nodo de busqueda que empieza en la raiz

    for(int i = 0; i < n; i++){//recorre cada caracter de la secuencia
        char base = pattern[i];
        
        switch(base){
            case 'A':
                current = current->A;
                break;
            case 'C':
                current = current->C;
                break;
            case 'G':
                current = current->G;
                break;
            case 'T':
                current = current->T;
                break;
            default://caracter invalido
                printf("-1\n");
                return;
        }
    }

    if(current->positions == NULL){//si no se encontraron posiciones
        printf("-1\n");
        return;
    }

    ListInt *reversed = NULL;
    ListInt *posList = current->positions;
    
    while(posList){//aqui se invierte la lista para imprimir en orden
        ListInt *temp = (ListInt*)malloc(sizeof(ListInt));
        temp->pos = posList->pos;
        temp->next = reversed;
        reversed = temp;
        posList = posList->next;
    }

    posList = reversed;
    while(posList){//imprime las posiciones
        printf("%d", posList->pos);
        if(posList->next != NULL)
            printf(" ");
        posList = posList->next;
    }
    printf("\n");
    
    while(reversed){//libera la lista invertida
        ListInt *temp = reversed;
        reversed = reversed->next;
        free(temp);
    }
}

void bio_max(Trie_ *trie){//buscar secuencia con maxima frecuencia con sus posiciones
    if(!trie || !trie->root){
        printf("Arbol vacio\n");
        return;
    }

    GeneInfo *gene_list = NULL;//lista para almacenar las secuencias y sus posiciones
    char *current_gene = (char*)malloc((trie->height + 1) * sizeof(char));//cadena temporal para construir las secuencias
    current_gene[0] = '\0';//inicializa la cadena vacia

    traverse_trie(trie->root, current_gene, 0, trie->height, &gene_list);//recorre el trie y llena la lista
    if(gene_list == NULL){
        printf("No hay genes en la secuencia\n");
        free(current_gene);
        return;
    }

    int max_count = 0;//variable para la maxima frecuencia
    GeneInfo *current = gene_list;//puntero para recorrer la lista
    while(current != NULL){//encuentra la maxima frecuencia
        if(current->count > max_count){
            max_count = current->count;
        }
        current = current->next;
    }

    current = gene_list;//reinicia el puntero al inicio de la lista
    while(current != NULL){//imprime las secuencias con la maxima frecuencia
        if(current->count == max_count){
            printf("%s ", current->gene);
            
            ListInt *reversed = NULL;
            ListInt *pos = current->positions;
            while(pos != NULL){//invierte la lista de posiciones para imprimir en orden
                ListInt *temp = (ListInt*)malloc(sizeof(ListInt));
                temp->pos = pos->pos;
                temp->next = reversed;
                reversed = temp;
                pos = pos->next;
            }
            
            pos = reversed;
            while(pos != NULL){//imprime las posiciones
                printf("%d", pos->pos);
                if(pos->next != NULL)
                    printf(" ");
                pos = pos->next;
            }
            printf("\n");
            
            while(reversed != NULL){//libera la lista invertida
                ListInt *temp = reversed;
                reversed = reversed->next;
                free(temp);
            }
        }
        current = current->next;
    }

    free(current_gene);//libera la cadena temporal
    free_gene_list(gene_list);//libera la lista de secuencias
}

void bio_min(Trie_ *trie){//buscar secuencia con minima frecuencia con sus posiciones
    if(!trie || !trie->root){
        printf("Arbol vacio\n");
        return;
    }

    GeneInfo *gene_list = NULL;//lista para almacenar las secuencias y sus posiciones
    char *current_gene = (char*)malloc((trie->height + 1) * sizeof(char));//cadena temporal para construir las secuencias
    current_gene[0] = '\0';//inicializa la cadena vacia

    traverse_trie(trie->root, current_gene, 0, trie->height, &gene_list);//recorre el trie y llena la lista
    if(gene_list == NULL){
        printf("No hay genes en la secuencia\n");
        free(current_gene);
        return;
    }

    int min_count = INT_MAX;//variable para la minima frecuencia
    GeneInfo *current = gene_list;//puntero para recorrer la lista
    while(current != NULL){//encuentra la frecuencia minima
        if(current->count < min_count){
            min_count = current->count;
        }
        current = current->next;
    }

    current = gene_list;//reinicia el puntero al inicio de la lista
    while(current != NULL){
        if(current->count == min_count){//imprime las secuencias con frecuencias minimas
            printf("%s ", current->gene);
            
            ListInt *reversed = NULL;
            ListInt *pos = current->positions;
            while(pos != NULL){
                ListInt *temp = (ListInt*)malloc(sizeof(ListInt));
                temp->pos = pos->pos;
                temp->next = reversed;
                reversed = temp;
                pos = pos->next;
            }
            
            pos = reversed;
            while(pos != NULL){//imprime las posiciones
                printf("%d", pos->pos);
                if(pos->next != NULL)
                    printf(" ");
                pos = pos->next;
            }
            printf("\n");
            while(reversed != NULL){//libera la lista invertida
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

void bio_all(Trie_ *trie){//imprime cada secuencia con sus respectivas posiciones
    if(!trie || !trie->root){
        printf("Arbol vacio\n");
        return;
    }

    GeneInfo *gene_list = NULL;//lista para almacenar las secuencias y sus posiciones
    char *current_gene = (char*)malloc((trie->height + 1) * sizeof(char));//cadena temporal para construir las secuencias
    current_gene[0] = '\0';//inicializa la cadena vacia

    traverse_trie(trie->root, current_gene, 0, trie->height, &gene_list);//recorre el trie y llena la lista

    if(gene_list == NULL){
        printf("No hay genes en la secuencia\n");
        free(current_gene);
        return;
    }

    print_gene_list(gene_list);//imprime las secuencias con sus posiciones
    //libera las listas
    free(current_gene);
    free_gene_list(gene_list);
}

void bio_exit(Node_ *node){//elimina el arbol
    if(node == NULL){
        return;
    }
    //elimina recursivamente cada hijo del arbol
    bio_exit(node->A);
    bio_exit(node->C);
    bio_exit(node->G);
    bio_exit(node->T);
    
    ListInt *current = node->positions;
    while(current != NULL){
        ListInt *temp = current;
        current = current->next;
        free(temp);
    }

    free(node);
}