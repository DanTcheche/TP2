#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "count_min_sketch.h"
#define TAM_PRIMO 105167

typedef unsigned long (*funcion_de_hash_t);

typedef struct count_min_sketch{
	size_t* array_1;
	size_t* array_2;
	size_t* array_3;
	size_t cant;
}count_min_sketch_t;

//		FUNCIONES DE HASHING

unsigned long funcion_hash_1(unsigned char *str){ //djb2
    unsigned long hash = 5381;
    int c;
    while ((c = (*str++))){
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash;
}

unsigned long funcion_hash_2(unsigned char *message) { //crc32b
   int i, j;
   unsigned int byte, crc, mask;

   i = 0;
   crc = 0xFFFFFFFF;
   while (message[i] != 0) {
      byte = message[i];            // Get next byte.
      crc = crc ^ byte;
      for (j = 7; j >= 0; j--) {    // Do eight times.
         mask = -(crc & 1);
         crc = (crc >> 1) ^ (0xEDB88320 & mask);
      }
      i = i + 1;
   }
   return ~crc;
}

unsigned long funcion_hash_3(char *str) { //sdbm
        unsigned long hash = 0;
        int c;

        while (c = (*(str++)))
            hash = c + (hash << 6) + (hash << 16) - hash;

        return hash;
    }


unsigned long obtener_indice(char* clave, funcion_de_hash_t* funcion, size_t cant){
	unsigned long pos = funcion(clave);
	pos = pos%cant;
	return pos;
}

size_t obtener_max(count_min_sketch_t* sketch, unsigned long pos_1, unsigned long pos_2, unsigned long pos_3){
	if((sketch->array_1[pos_1]<sketch->array_2[pos_2])&&(sketch->array_1[pos_1]<sketch->array_3[pos_3])) return sketch->array_1[pos_1];
	if((sketch->array_1[pos_2]<sketch->array_2[pos_1])&&(sketch->array_2[pos_2]<sketch->array_3[pos_3])) return sketch->array_2[pos_2];
	return sketch->array_3[pos_3];
}

//		PRIMITIVAS DEL SKETCH

count_min_sketch_t* crear_sketch(){
	count_min_sketch_t* sketch = malloc(sizeof(count_min_sketch_t));
	sketch->array_1 = malloc(sizeof(size_t)*TAM_PRIMO);
	sketch->array_2 = malloc(sizeof(size_t)*TAM_PRIMO);
	sketch->array_3 = malloc(sizeof(size_t)*TAM_PRIMO);
	sketch->cant = 0;
	return sketch;
}

void procesar_dato(count_min_sketch_t* sketch, char* clave){
	unsigned long pos_1 = obtener_indice(clave, funcion_hash_1, sketch->cant);
	unsigned long pos_2 = obtener_indice(clave, funcion_hash_2, sketch->cant);
	unsigned long pos_3 = obtener_indice(clave, funcion_hash_3, sketch->cant);
	sketch->array_1[pos_1]++;
	sketch->array_2[pos_2]++;
	sketch->array_3[pos_3]++;
}

size_t cant_apariciones(count_min_sketch_t* sketch, char* clave){
	unsigned long pos_1 = obtener_indice(clave, funcion_hash_1, sketch->cant);
	unsigned long pos_2 = obtener_indice(clave, funcion_hash_2, sketch->cant);
	unsigned long pos_3 = obtener_indice(clave, funcion_hash_3, sketch->cant);
	return obtener_max(sketch, pos_1, pos_2, pos_3);
}

void sketch_destruir(count_min_sketch_t* sketch){
	free(sketch->array_1);
	free(sketch->array_2);
	free(sketch->array_3);
	free(sketch);
}






