#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "strutil.h"
#include "hash.h"
#include "heap.h"

typedef struct campo_heap{
   char* users;
   int* tags;
}campo_heap_t;

/*Funcion de comparacion del heap*/
int cmp(const void* a,const void* b){
   campo_heap_t* item_1 = (campo_heap_t*)a;
   campo_heap_t* item_2 = (campo_heap_t*)b;
   if(*(item_1->tags) < *(item_2->tags)) return 1;
   if(*(item_1->tags) > *(item_2->tags)) return -1;
   return 0;
}

campo_heap_t* campo_heap_crear(int* tags,char* user){
	campo_heap_t* campo = malloc(sizeof(campo_heap_t));
	if(!campo)return NULL;
	campo->tags = tags;
	campo->users = user;
	return campo;
}

/*Funcion que recibe un hash, crea un heap de menores con los elementos de este.*/
heap_t* heap_menores(hash_t* hash){
   heap_t* heap = heap_crear(cmp);
   hash_iter_t* iter = hash_iter_crear(hash);
   while(!hash_iter_al_final(iter)){
      char* user = hash_iter_ver_actual(iter);
      int* tags = hash_obtener(hash,user);
      campo_heap_t* campo = campo_heap_crear(tags,user);
      if(!campo)return NULL;
      heap_encolar(heap, campo);
      hash_iter_avanzar(iter);
   }
   hash_iter_destruir(iter);
   return heap;
}

void imprimir_heap(heap_t* heap){
	while(!heap_esta_vacio(heap)){
		campo_heap_t* campo = heap_desencolar(heap);
		int* tag = campo->tags;
		printf("%d: %s", *tag, campo->users);
		while(!heap_esta_vacio(heap)){
			campo_heap_t* campo_2 = heap_ver_max(heap);
			if(*(campo_2->tags) == *(tag)){
				printf(", %s", campo_2->users);
				free(heap_desencolar(heap));
			}
			else{
				printf("\n");
				break;
			}
		}
		free(campo);
	}
	printf("\n");

}


int procesar_usuarios(char* file){
	FILE* archivo = fopen(file, "r");

	if(!archivo) return -1;
	hash_t* hash = hash_crear(free);
	size_t n;
	char* str = NULL;

	while(getline(&str,&n,archivo) > 0){

		char** process = split(str,',');
		free(str);
		char* user = process[0];		
	
		if (!hash_pertenece(hash,user)){
			int* value = calloc(1,sizeof(int));
     		
			hash_guardar(hash,user,value);
			}

		int* cant_tags = hash_obtener(hash,user);

		int indice =1;

		while(process[indice]){
			*(cant_tags) += 1;

			indice++;
		}
		n = 0;
		//free(str);
		free_strv(process);

	}
	free(str);
	fclose(archivo);
	heap_t* heap = heap_menores(hash);
	imprimir_heap(heap);
	heap_destruir(heap,NULL);
	hash_destruir(hash);


	
	return 0;
}

int main(int argc,char* argv[]) {
	if(argc != 2){
		printf("Error");
		return 1;
	}
	char* str = argv[1];
		
	procesar_usuarios(str);
	return 0;

}