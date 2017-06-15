#ifndef COUNT_MIN_SKETCH_H
#define COUNT_MIN_SKETCH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct count_min_sketch count_min_sketch_t;

typedef unsigned long (*funcion_de_hash_t)(char*);

/* Crea el sketch.*/
count_min_sketch_t* crear_sketch(size_t cant);

/*Recibe clave por parametro y le suma uno a la posicion correspondiente.*/
void procesar_dato(count_min_sketch_t* sketch, char* clave);

/*Devuelve la minima cantidad de apariciones dada una clave.*/
size_t cant_apariciones(count_min_sketch_t* sketch, char* clave);

/*Destruye el sketch.*/
void sketch_destruir(count_min_sketch_t* sketch);

#endif //