#ifndef COUNT_MIN_SKETCH_H
#define COUNT_MIN_SKETCH_H

#include <stdbool.h>
#include <stddef.h>

struct count_min_sketch;
typedef unsigned long (*funcion_hast_t) (const char* clave);

typedef struct count_min_sketch{
	size_t* array_1;
	size_t* array_2;
	size_t* array_3;
	size_t cant;
} count_min_sketch_t;


/* Crea el sketch.*/
count_min_sketch_t* crear_sketch();

/*Recibe clave por parametro y le suma uno a la posicion correspondiente.*/
void procesar_dato(count_min_sketch_t* sketch, char* clave);

/*Devuelve la minima cantidad de apariciones dada una clave.*/
size_t* cant_apariciones(count_min_sketch_t* sketch, char* clave);

/*Destruye el sketch.*/
void sketch_destruir(count_min_sketch_t* sketch);

#endif // HASH_H