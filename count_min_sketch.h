#ifndef COUNT_MIN_SKETCH_H
#define COUNT_MIN_SKETCH_H

#include <stdbool.h>
#include <stddef.h>

struct count_min_sketch;
typedef unsigned long (*funcion_hash_t) (const char* clave);

typedef struct sketch count_min_sketch_t;


/* Crea el count_min_sketch.*/
count_min_sketch_t* crear_sketch(size_t cant);

/*Recibe clave por parametro y le suma uno a la posicion correspondiente.
Pre: se creo el count_min_sketch.
Post: aumento en una el valor en al posicion correspondiente de todas las tablas*/

void procesar_dato(count_min_sketch_t* sketch, char* clave);

/*Devuelve la minima cantidad de apariciones en las 3 tablas dada una clave.
Pre: se creo el count_min_sketch.
Post: se devuelve la cantidad minima de aparciones*/
size_t cant_apariciones(count_min_sketch_t* sketch, char* clave);

/*Destruye el count_min_sketch.
Pre: se creo el count_min_sketch.*/
void sketch_destruir(count_min_sketch_t* sketch);

#endif // SKETCH_H
