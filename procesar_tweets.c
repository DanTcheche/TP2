#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include "strutil.h"
#include "hash.h"
#include "heap.h"
#include "count_min_sketch.h"
#define ERROR_BUFFER -1
#define ERROR_PARAMETROS -4
#define TAM_PRIMO 105167

typedef struct campo_heap{
   char* tag;
   size_t apariciones;
}campo_heap_t;

campo_heap_t* campo_heap_crear(char* tag, size_t apariciones){
   campo_heap_t* campo = malloc(sizeof(campo_heap_t));
   if(!campo)return NULL;
   campo->tag = tag;
   campo->apariciones = apariciones;
   return campo;
}

/*Funcion de comparacion del heap*/
int cmp(const void* a,const void* b){
   campo_heap_t* item_1 = (campo_heap_t*)a;
   campo_heap_t* item_2 = (campo_heap_t*)b;
   if((item_1->apariciones) < (item_2->apariciones)) return 1;
   if((item_1->apariciones) > (item_2->apariciones)) return -1;
   return 0;
}

/*Funcion que recibe un hash, un sketch y crea un heap de menores con los elementos de este.*/
heap_t* heap_menores(hash_t* hash, count_min_sketch_t* sketch){
   heap_t* heap = heap_crear(cmp);
   hash_iter_t* iter_hash = hash_iter_crear(hash);
   while(!hash_iter_al_final(iter_hash)){
      char* tag = hash_iter_ver_actual(iter_hash);
      size_t apariciones = cant_apariciones(sketch, tag);
      campo_heap_t* campo = campo_heap_crear(tag, apariciones);
      heap_encolar(heap, campo);
      hash_iter_avanzar(iter_hash);
   }
   hash_iter_destruir(iter_hash);
   return heap;
}

/*Imprime por consola los k TT.*/
void imprimir_TT(heap_t* heap, int k){
   printf("Historicos %d trending topics\n", k);
   for(int i = 0; i < k; i++){
      if(!heap_esta_vacio(heap)){
         campo_heap_t* campo = heap_desencolar(heap);
         printf("%s", campo->tag);
      }
   }
}

/*Funcion que procesa los tweets. Devuelve -1 si no se pudo crear el buffer.*/
int procesar_tweets(FILE* archivo, int n, int k){
   char* buffer = malloc(n+1);
   if(!buffer) return ERROR_BUFFER;
   count_min_sketch_t* sketch = crear_sketch(TAM_PRIMO);
   hash_t* hash = hash_crear(NULL);
   size_t num;
   int cont = 0;
   while(cont<n){
      if(!getline(&buffer, &num, archivo)) break;
      char** tags = split(buffer, ',');
      int i = 1;
      while(tags[i]){
         procesar_dato(sketch, tags[i]);
         hash_guardar(hash, tags[i], NULL);
         i++;
      }
      cont++;
   }

   heap_t* heap = heap_menores(hash, sketch);
   imprimir_TT(heap, k);
   hash_destruir(hash);
   sketch_destruir(sketch);
   heap_destruir(heap, NULL);
   return 0;
}

/*Dado un char, se fija si esta compuesto unicamente por digitos.*/
bool chequear_digitos(char* parametro){
   for(int i = 0; parametro[i] != '\0'; i++){
      if(!isdigit(parametro[i])){
         return false;
      }
   }
   return true;
}

/*Función que verifica que la cantidad de parametros sea correcta y que ambos
parametro sean dígitos.*/
bool chequear_parametros(int argc, char* argv[]){
   if(argc != 3){
      printf("Ingresó un número incorrecto de parametros.\n");
      return false;
   }
   char* cantidad = argv[1];
   char* cantidad2 = argv[2];
   if(!chequear_digitos(cantidad) && !chequear_digitos(cantidad2)){
      printf("No ingresó un numero en ambos parametros.\n");
      return false;      
   }
   return true;
}

/*Main de la función. Si hubo un error en los parametros devuelve -4, de lo contrario devuelve 0.*/
int main(int argc, char* argv[]){
   if(!(chequear_parametros(argc, argv))) return ERROR_PARAMETROS;
   int lineas = atoi(argv[1]);
   int TT = atoi(argv[2]);
   procesar_tweets(stdin, lineas, TT);
   return 0;
}