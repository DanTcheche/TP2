#define _POSIX_C_SOURCE 200809L

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "strutil.h"
#include "lista.h"
#include "hash.h"
/*Funcion que devuelve el mayor valor en una tabla de hash.*/
int obtener_max_tag(hash_t* hashe){
	int tag_max = 0;
	if(hash_cantidad(hashe) == 0) return max_tag;
	hash_iter_t* iter = hash_iter_crear(hashe);

	while(!hash_iter_al_final(iter)){
		int* tags = hash_obtener(hashe,hash_iter_ver_actual(iter));
		if(*tags > tag_max) tag_max = *tags;
		hash_iter_avanzar(iter);
	}
	hash_iter_destruir(iter);
	return tag_max;
}
/*Funcion que imprime los elementos de una lista*/
void imprimir_lista(lista_t* lista, int tag){
	if(lista_esta_vacia(lista))return;
	printf("%d: ", tag );
	while(!lista_esta_vacia(lista)){
		char* user = lista_borrar_primero(lista);
		if(lista_largo(lista) == 0){
			printf("%s\n",user);
			return;
		}
		printf("%s, ",user );
	}
}
/*Funcion que crea una lista enlazada para cada elemento de una lista_t**. Memoria a cargo del Usuario */
bool inicilaizar_listas(lista_t** lista, int rango){
	for(int i = 0; i < rango; i++ ){
		if(!(lista[i] = lista_crear()))return false;
	}
	return true;
}
/*Recorre y destruye las listas de una array lista_t**. No destruye los datos de las listas*/
void destruir_listas(lista_t** listas, int rango){
	for(int i = 0; i < rango; i++){
		lista_destruir(listas[i],NULL);
	}
}
 /*Realiza bucket sort con los usuarios de un hash, segun su valor en una lista previamente inicializada */
void completar_listas_con_usuarios(hash_t* hash, lista_t** listas){
	hash_iter_t* iter_h = hash_iter_crear(hash);
	while(!hash_iter_al_final(iter_h)){
		char* user = hash_iter_ver_actual(iter_h);
		int* aux = hash_obtener(hash,user);
		lista_insertar_ultimo(listas[*aux], user);
		hash_iter_avanzar(iter_h);
	}
	hash_iter_destruir(iter_h);
}
/*Funcion que recorre un archivo de la forma Usuario, tags, tags, etc.
 Muestra por pantalla la cantidad de tags de cada usuario*/
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
		free_strv(process);
	}
	free(str);
	fclose(archivo);

	int max_tag = obtener_max_tag(hash);
	lista_t* listas[max_tag+1];
	if(!inicilaizar_listas(listas, max_tag+1))return -2;
	completar_listas_con_usuarios(hash,listas);

	for(int ind = 0; ind < max_tag+1; ind++){
		if(listas[ind] == 0) continue;
		imprimir_lista(listas[ind],ind);
	}

	destruir_listas(listas, max_tag);
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