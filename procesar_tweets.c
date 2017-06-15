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
#define ERROR_SKETCH -2
#define ERROR_HASH -3
#define ERROR_HEAP -4
#define ERROR_PARAMETROS -5
#define TAM_PRIMO 50023

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
heap_t* heap_menores(hash_t* hash, count_min_sketch_t* sketch, int k){
   heap_t* heap = heap_crear(cmp);
   if(!heap) return NULL;
   hash_iter_t* iter_hash = hash_iter_crear(hash);
   if(!iter_hash) return NULL;
   while(!hash_iter_al_final(iter_hash)){
      char* tag = hash_iter_ver_actual(iter_hash);
      size_t apariciones = cant_apariciones(sketch, tag);
      campo_heap_t* campo = campo_heap_crear(tag, apariciones);
      if(!campo) return NULL;
      if(heap_cantidad(heap)>k){
         campo_heap_t* campo_2 = heap_ver_max(heap);
         if(campo->apariciones > campo_2->apariciones){
            heap_desencolar(heap);
         }
      }
      heap_encolar(heap, campo);
      hash_iter_avanzar(iter_hash);
   }
   hash_iter_destruir(iter_hash);
   return heap;
}

/*Imprime por consola los k TT.*/
void imprimir_TT(heap_t* heap, int k){
   printf("Historicos %d trending topics\n", k);
   while(!heap_esta_vacio(heap)){
         campo_heap_t* campo = heap_desencolar(heap);
         printf("%s", campo->tag);
      }
   }

/*Procesa una linea del archivo. Se han creado el sketch y el hash correctamente.*/
void procesar_linea(count_min_sketch_t* sketch, hash_t* hash, char* buffer){
   int i = 1;
   char** tags = split(buffer, ',');
   while(tags[i]){
      procesar_dato(sketch, tags[i]);
      hash_guardar(hash, tags[i], NULL);
      i++;
   }
   free_strv(tags);
}

/*Funcion que procesa los tweets. Devuelve -1 si no se pudo crear el buffer, -2 si 
no se pudo crear el sketch, -3 si no se pudo crear el hash y -4 si no se pudo crear el heap.*/
int procesar_tweets(FILE* archivo, int n, int k){
   char* buffer = NULL;
   count_min_sketch_t* sketch = crear_sketch(TAM_PRIMO);
   if(!sketch) return ERROR_SKETCH;
   hash_t* hash = hash_crear(free);
   if(!hash) return ERROR_HASH;
   size_t num = 0;
   int cont = 0;
   while(cont<n){
      if(getline(&buffer, &num, archivo)<0) break;
      procesar_linea(sketch, hash, buffer);
      cont++;
      free(buffer);
   }
   heap_t* heap = heap_menores(hash, sketch, k);
   if(!heap) return ERROR_HEAP;
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

/*Main de la función. Si hubo un error en los parametros devuelve -5, de lo contrario devuelve 0.*/
int main(int argc, char* argv[]){
   if(!(chequear_parametros(argc, argv))) return ERROR_PARAMETROS;
   int lineas = atoi(argv[1]);
   int TT = atoi(argv[2]);
   int ok = procesar_tweets(stdin, lineas, TT);
   printf("%d\n", ok);
   return 0;
}