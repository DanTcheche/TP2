#include "hash.h"
#include "strutil.h"
#define TAM_PRIMO 105167

typedef struct count_min_sketch{
	size_t* array1;
	size_t* array2;
	size_t* array3;
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

unsigned int funcion_hash_2(unsigned char *message) { //crc32b
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

long funcion_hash_3(char *str, int len) { //sdbm
	unsigned long n = 0;

	while (len--)
		n = *str++ + (n << 6) + (n << 16) - n;

	return n;
}

//		PRIMITIVAS DEL SKETCH

count_min_sketch_t* crear_sketch(){
	count_min_sketch->array1[TAM_PRIMO];
	count_min_sketch->array2[TAM_PRIMO];
	count_min_sketch->array3[TAM_PRIMO];
	count_min_sketch->cant = 0;
}

void procesar_dato(count_min_sketch_t* sketch, char* clave){

}



ver_apariciones

destruir





