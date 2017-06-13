#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include "hash.h"
#define CAP_INICIAL 10
#define CARGA_MAX 0.7
#define	CARGA_MIN 0.25

typedef enum {BORRADO,VACIO,OCUPADO} estado_t;

typedef struct campo_hash{
	char* clave;
	void* dato;
	estado_t estado;
}campo_hash_t;

typedef struct hash{
	size_t capacidad;
	size_t cantidad;
	hash_destruir_dato_t destructor;
	campo_hash_t* tabla;
} hash_t;


// **FUNCIONES AUXILIARES**

bool misma_clave(const hash_t* hash, const char* clave, size_t indice){
	int aux = strcmp(clave, hash->tabla[indice].clave);
	return aux == 0;
}

unsigned long funcion_hash(unsigned char *str){
    unsigned long hash = 5381;
    int c;
    while ((c = (*str++))){
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */
    }
    return hash;
}

float factor_de_carga(hash_t* hash){
	return (float)(hash->cantidad)/(float)(hash->capacidad);
}

void inicializar_tabla(campo_hash_t* tabla,size_t capacidad){
	for(int i=0; i < capacidad; i++){
		tabla[i].estado = VACIO;
	}
}

bool hash_redimensionar(hash_t* hash, size_t nueva_cap){
	campo_hash_t* tabla_2 = malloc(sizeof(campo_hash_t)*nueva_cap);
	if(!tabla_2) return false;
	inicializar_tabla(tabla_2,nueva_cap);
	campo_hash_t* tabla_anterior = hash->tabla;
	size_t cap_anterior = hash->capacidad;
	hash->tabla = tabla_2;
	hash->capacidad = nueva_cap;
	hash->cantidad = 0;
	for(int i = 0; i < cap_anterior; i++){
		if (tabla_anterior[i].estado == OCUPADO) {
			hash_guardar(hash, tabla_anterior[i].clave, tabla_anterior[i].dato);
			free(tabla_anterior[i].clave);
		}
	}
	free(tabla_anterior);
	return true;
}

unsigned long obtener_indice(const hash_t* hash, bool* condicion, const char* clave){
	unsigned long ind = funcion_hash((unsigned char*)clave);
	ind = ind % hash->capacidad;
	while(hash->tabla[ind].estado != VACIO){
		if(hash->tabla[ind].estado == OCUPADO && (misma_clave(hash, clave, ind))){
			*(condicion) = true;
			return ind;
		}
		ind++;
		if(ind == hash->capacidad) ind = 0;
	}
	return ind;
}

//		**PRIMITIVAS DEL HASH**

hash_t* hash_crear(hash_destruir_dato_t destruir_dato){
	hash_t* hash = malloc(sizeof(hash_t));
	if(!hash) return NULL;
	hash->cantidad = 0;
	hash->capacidad = CAP_INICIAL;
	hash->tabla = malloc(sizeof(campo_hash_t)*CAP_INICIAL);
	if(!hash->tabla){
		free(hash);
		return NULL;
	}
	inicializar_tabla(hash->tabla,hash->capacidad);
	hash->destructor = destruir_dato;
	return hash;
}

bool hash_guardar(hash_t* hash, const char *clave, void *dato){
	char* clave_2 = malloc(strlen(clave)+1);
	if(!clave_2) return false;
	strcpy(clave_2,clave);
	bool condicion = false;
	unsigned long indice = obtener_indice(hash, &condicion, clave_2);
		if(condicion){
			if(hash->destructor){
				hash->destructor(hash->tabla[indice].dato);
			}
			hash->tabla[indice].dato = dato;
			free(clave_2);
			return true;
		}
	hash->tabla[indice].clave = clave_2;
	hash->tabla[indice].dato = dato;
	hash->tabla[indice].estado = OCUPADO;
	hash->cantidad++;
	if(factor_de_carga(hash) >= CARGA_MAX){
		bool res = hash_redimensionar(hash, hash->capacidad*2);
		if(!res){
			free(clave_2);
		}
		return res;
	}
	return true;
}

void* hash_borrar(hash_t* hash, const char *clave){
	bool condicion = false;
	unsigned long indice = obtener_indice(hash, &condicion, clave);
		if(condicion){
			hash->tabla[indice].estado = BORRADO;
			hash->cantidad--;
			free(hash->tabla[indice].clave);
			void* aux = hash->tabla[indice].dato;
			float carga = factor_de_carga(hash);
			if(carga < CARGA_MIN && hash->capacidad > CAP_INICIAL){
				bool res = hash_redimensionar(hash, hash->capacidad/2);
				if(!res) return NULL;
			}
			return aux;
		}
	return NULL;
}

void *hash_obtener(const hash_t *hash, const char *clave){
	bool condicion = false;
	unsigned long indice = obtener_indice(hash, &condicion, clave);
	if(condicion) return hash->tabla[indice].dato;
	return NULL;
}	

bool hash_pertenece(const hash_t *hash, const char *clave){
	bool condicion = false;
	obtener_indice(hash, &condicion, clave);
	return condicion;
}

size_t hash_cantidad(const hash_t *hash){
	return hash->cantidad;
}

void hash_destruir(hash_t *hash){
	for(int i = 0; i< hash->capacidad; i++){
		if(hash->tabla[i].estado == OCUPADO){
			free(hash->tabla[i].clave);
			if(hash->destructor){
				hash->destructor(hash->tabla[i].dato);
			}
		}
	}
	free(hash->tabla);
	free(hash);
}

/* Iterador del hash */

typedef struct hash_iter{
	size_t iterados;
	size_t indice;
	const hash_t* hash;
}hash_iter_t;

hash_iter_t* hash_iter_crear(const hash_t* hash){
	hash_iter_t* iter = malloc(sizeof(hash_iter_t));
	if(!iter) return NULL;
	iter->iterados = 0;
	iter->indice = 0;
	iter->hash = hash;
	if(iter->hash->cantidad != 0){
		while (iter->hash->tabla[iter->indice].estado != OCUPADO){
			iter->indice++;
		}
	}
	return iter;
}

bool hash_iter_al_final(const hash_iter_t* iter){
	return iter->iterados == iter->hash->cantidad;
}

bool hash_iter_avanzar(hash_iter_t* iter){
	if(hash_iter_al_final(iter)) return false;
	iter->indice++;
	if((iter->iterados)+1 != iter->hash->cantidad){
		while(iter->hash->tabla[iter->indice].estado != OCUPADO ){
			iter->indice++;
			if(iter->indice == iter->hash->capacidad) return false;
		}
	}
	iter->iterados++;
	return true;
}

const char* hash_iter_ver_actual(const hash_iter_t *iter){
	if(hash_iter_al_final(iter)) return NULL;
	const char* aux = iter->hash->tabla[iter->indice].clave;
	return aux;
}

void hash_iter_destruir(hash_iter_t* iter){
	free(iter);
}