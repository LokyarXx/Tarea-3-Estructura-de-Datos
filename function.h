#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <limits.h>
#include <stdbool.h>
#include "structure.h"
#define archivos 20 // numero maximos de archivos de secuencias ADN
extern int three_height;

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

void NumToChar(int aleatorio, FILE *archivo, char secuencia[], int pos){

    char letra;
    
    switch (aleatorio){
        case 0: letra = 'A'; break; // Si el numero es 0, la letra es A
        case 1: letra = 'T'; break; // Si el numero es 1, la letra es T
        case 2: letra = 'C'; break; // Si el numero es 2, la letra es C
        case 3: letra = 'G'; break; // Si el numero es 3, la letra es G
    }

    secuencia[pos] = letra;
    printf ("%c", letra);
    fprintf (archivo, "%c", letra); // Esribe la letra en el archivo
}

void create_sequence(int long_adn){

    int rnd; // aleatorio
    int num = 1; 
    int prev_rnd = -1; // variable para evitar repeticion de letras consecutivas
    char secuencia[long_adn]; 
    srand(time(NULL));
    char nombre_archivo[20];

    if (long_adn < 15){
        printf ("Longitud invalida. Ingrese una longitud de al menos 15 caracteres.\n");
        return;
    }

    // ciclo que verifica el numero de archivos existentes, y crea uno nuevo con el numero siguiente 
    do {
        sprintf(nombre_archivo, "secuencia%d.txt", num); // se crea el nombre del archivo
        FILE *test = fopen(nombre_archivo, "r");
        if (test == NULL) 
            break; 
        fclose(test);
        num++;
    } while(num <= archivos); // Limitar el numero maximo de archivos

    if (num > archivos){
        printf("Se ha alcanzado el numero maximo de archivos de secuencias de ADN, elimine archivos de secuencia para generar nuevos. \n");
        return; 
    }

    FILE *archivo = fopen(nombre_archivo, "w"); 
    if (archivo == NULL){
        printf ("error al crear el archivo");
    }

    for (int i = 1; i <= long_adn; i++){
        
        do {
            rnd = rand()%4; //numeros aleatorios entre 0 y 3
        } while (rnd == prev_rnd); // evita que se repitan letras consecutivas

        prev_rnd = rnd;
        NumToChar(rnd, archivo, secuencia, i); //funcion que transforma el numero aleatorio a letra 
    }

    printf ("\nsecuencia nueva creada con exito en secuencia%d.txt\n", num);
    fclose(archivo);
}

int FileExists(const char *filename)
{
    struct stat buffer;
    return (stat (filename, &buffer) == 0);
}

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
    int n = (int)strlen(sequence);
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

int delete_secuence_file(const char *filename){

    if (!FileExists(filename)){
        printf("El archivo %s no existe o ya fue eliminado\n", filename);
        return -1;
    }
    if (remove(filename) == 0){
        printf ("archivo %s eliminado\n", filename); 
        return 0;
    }
    else {
        printf ("Error al eliminar el archivo %s\n", filename);
        return -1;
    }
}

int delete_all_secuence_files(){

    char filename[30];
    int deleted=0;

    for (int i = 1; i <= archivos; i++){
        
        sprintf (filename, "secuencia%d.txt", i);

        if (FileExists(filename)){
            
            if (remove (filename) == 0){
                deleted ++;
            }
        }
    }
    
    if (deleted == 0){
        printf ("No hubo archivos de secuencia de ADN para eliminar\n");
        return -1;
    }

    else {
        printf ("Se han eliminado TODOS los archivos de secuencias ADN\n");
    }

    return 0;
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
    if (!trie->root)
    {
        trie->root = create_node();
    }

    Node_ *current = trie->root;
    for(int i = 0; i < trie->height; i++)
    {
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
    add_position(current, position);
}

void insert_adn(Trie_ *trie, char *c)//aqui se insertan todas las subcadenas de longitud m en el trie
{
    int n = (int)strlen(c);
    int m = trie->height;
    for(int i = 0; i <= n - m; i++)
    { 
        if(i + m <= n)
        {
            char sub[m+1];
            strncpy(sub, &c[i], m);
            sub[m] = '\0';
            insert_gen(trie, sub, i);
        }
    }
}

int count_positions(ListInt *positions)
{
    int count = 0;
    ListInt *current = positions;
    while(current != NULL)
    {
        count++;
        current = current->next;
    }
    return count;
}

void traverse_trie(Node_ *node, char *current_gene, int depth, int max_depth, GeneInfo **gene_list)
{
    if(node == NULL)
    {
        return;
    }
    if(depth == max_depth)
    {
        if(node->positions != NULL)
        {
            GeneInfo *new_gene = (GeneInfo*)malloc(sizeof(GeneInfo));
            new_gene->gene = (char*)malloc((max_depth + 1) * sizeof(char));
            strcpy(new_gene->gene, current_gene);
            new_gene->positions = node->positions;
            new_gene->count = count_positions(node->positions);
            new_gene->next = *gene_list;
            *gene_list = new_gene;
        }
        return;
    }

    if(node->A != NULL)
    {
        current_gene[depth] = 'A';
        current_gene[depth + 1] = '\0';
        traverse_trie(node->A, current_gene, depth + 1, max_depth, gene_list);
    }
    if(node->C != NULL)
    {
        current_gene[depth] = 'C';
        current_gene[depth + 1] = '\0';
        traverse_trie(node->C, current_gene, depth + 1, max_depth, gene_list);
    }
    if(node->G != NULL)
    {
        current_gene[depth] = 'G';
        current_gene[depth + 1] = '\0';
        traverse_trie(node->G, current_gene, depth + 1, max_depth, gene_list);
    }
    if(node->T != NULL)
    {
        current_gene[depth] = 'T';
        current_gene[depth + 1] = '\0';
        traverse_trie(node->T, current_gene, depth + 1, max_depth, gene_list);
    }
}

void print_gene_list(GeneInfo *list)
{
    while(list != NULL)
    {
        printf("%s ", list->gene);
        
        ListInt *reversed = NULL;
        ListInt *current = list->positions;
        while(current != NULL)
        {
            ListInt *temp = (ListInt*)malloc(sizeof(ListInt));
            temp->pos = current->pos;
            temp->next = reversed;
            reversed = temp;
            current = current->next;
        }
        
        current = reversed;
        while(current != NULL)
        {
            printf("%d", current->pos);
            if(current->next != NULL)
                printf(" ");
            current = current->next;
        }
        printf("\n");
        
        while(reversed != NULL)
        {
            ListInt *temp = reversed;
            reversed = reversed->next;
            free(temp);
        }
        
        list = list->next;
    }
}

void free_gene_list(GeneInfo *list)
{
    while(list != NULL)
    {
        GeneInfo *temp = list;
        list = list->next;
        free(temp->gene);
        free(temp);
    }
}