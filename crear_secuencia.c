#include<stdio.h>
#include<stdlib.h>
#include<time.h>

#define long_adn 16 //longitud maxima de la secuencia ADN
#define archivos 5 // numero maximos de archivos de secuencias ADN

// Esto es una funcion dentro del programa. Nombre sugerido: generar nueva secuencia de ADN. tipo: la que estimen conveniente
void generar_secuencia(){

    int aleatorio; 
    int num = 1;
    char secuencia[long_adn];
    srand(time(NULL));
    char nombre_archivo[20];
    
    // ciclo que verifica el numero de archivos existentes, y crea uno nuevo con el numero siguiente 
    do {
        sprintf(nombre_archivo, "secuencia_adn_%d.txt", num); // se crea el nombre del archivo
        FILE *test = fopen(nombre_archivo, "r");
        if (test == NULL) 
            break; 
        fclose(test);
        num++;
    } while(num <= archivos); // Limitar el numero maximo de archivos

    if (num > archivos){
        printf("Se ha alcanzado el numero maximo de archivos de secuencias de ADN, elimine archivos de secuencia para generar nuevos. \n");
        return 1;
    }

    FILE *archivo = fopen(nombre_archivo, "w"); 
    if (archivo == NULL){
        printf ("error al crear el archivo");
        return 1;
    }

    for (int i = 1; i <= long_adn; i++){

        aleatorio = rand()%4; //numeros aleatorios entre 0 y 3
        transformar_a_letra(aleatorio, archivo, secuencia, i); //funcion que transforma el numero aleatorio a letra 
    }

    printf ("\nsecuencia nueva creada con exito en secuencia_adn.txt\n");
    fclose(archivo);
    return 0;
}

void transformar_a_letra(int aleatorio, FILE *archivo, char secuencia[], int pos){

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

