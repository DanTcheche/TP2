#include <stdlib.h>
#include <stdbool.h>
#include "heap.h"
#define CARGA_MAX 0.7
#define	CARGA_MIN 0.25
#define FACT_REDIM 2
#define TAM_INI 30


typedef struct heap{
	void** elementos;
	size_t cantidad;
	size_t tam;
	cmp_func_t cmp;
}heap_t;

//			FUNCIONES AUXILIARES

void swap(void* elementos[], int pos1, int pos2){
	void* aux = elementos[pos1];
	elementos[pos1] = elementos[pos2];
	elementos[pos2] = aux;
}

void upheap(void* elementos[], size_t posicion, cmp_func_t cmp){
	if(posicion == 0) return;
	int padre = ((int)posicion-1)/2;
	if(cmp(elementos[padre], elementos[posicion]) < 0){
		swap(elementos, padre, (int)posicion);
		upheap(elementos, padre, cmp);
	}
}

void downheap(void* elementos[], size_t tam, size_t posicion, cmp_func_t cmp){
	if(posicion>tam) return;
	size_t max = posicion;
	size_t hijo_izq = posicion*2+1;
	size_t hijo_der = posicion*2+2;
	if(hijo_izq < tam && cmp(elementos[hijo_izq], elementos[max]) > 0) max = hijo_izq;
	if(hijo_der < tam && cmp(elementos[hijo_der], elementos[max]) > 0) max = hijo_der;
	if(max != posicion){
		swap(elementos, (int)max, (int)posicion);
		downheap(elementos, tam, max, cmp);
	}
}

void heapify(void* elementos[], size_t cant, cmp_func_t cmp){
	for(int i = ((int)cant-1); i >= 0; i--){
		downheap(elementos, cant, i, cmp);
	}
}

float factor_de_carga(heap_t* heap){
	return (float)(heap->cantidad)/(float)(heap->tam);
}

bool heap_redimensionar(heap_t* heap, size_t tam_nuevo){
	void** elementos_aux = heap->elementos;
	void** elementos_nuevo = realloc(elementos_aux, sizeof(void*)*tam_nuevo);
	if(!elementos_nuevo) return false;
	heap->elementos = elementos_nuevo;
	heap->tam = tam_nuevo;
	return true;
}

//		PRIMITIVAS DEL HEAP

heap_t* heap_crear(cmp_func_t cmp){
	heap_t* heap = malloc(sizeof(heap_t));
	if(!heap) return NULL;
	void** elementos = malloc(sizeof(void*)*TAM_INI);
	if(!elementos) return NULL;
	heap->elementos = elementos;
	heap->cmp = cmp;
	heap->cantidad = 0;
	heap->tam = TAM_INI;
	return heap;
}

void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp){
	heapify(elementos, cant, cmp);
	for(int i = ((int)cant-1); i >= 0; i--){
		swap(elementos, 0, i); 
		downheap(elementos, i, 0, cmp);
	}
}

heap_t* heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp){
	heapify(arreglo, n, cmp);
	heap_t* heap = heap_crear(cmp);
	heap->elementos = arreglo;
	heap->cantidad = n;
	heap->cmp = cmp;
	heap->tam = n;
	return heap;
}

void heap_destruir(heap_t *heap, void destruir_elemento(void *e)){
	if(destruir_elemento){
		for(int i = 0; i<heap->cantidad; i++){
			destruir_elemento(heap->elementos[i]);
		}
	}
	free(heap->elementos);
	free(heap);
}

size_t heap_cantidad(const heap_t *heap){
	return heap->cantidad;
}

bool heap_esta_vacio(const heap_t *heap){
	return (heap->cantidad == 0);
}

bool heap_encolar(heap_t *heap, void* elem){
	if(elem == NULL) return false;
	if (factor_de_carga(heap)>CARGA_MAX){
		if(!heap_redimensionar(heap, (heap->tam)*FACT_REDIM)) return false;
	}
	heap->elementos[heap->cantidad] = elem;
	upheap(heap->elementos, heap->cantidad, heap->cmp);
	heap->cantidad++;
	return true;
}

void* heap_ver_max(const heap_t *heap){
	if(heap->cantidad == 0) return NULL;
	return heap->elementos[0];
}

void* heap_desencolar(heap_t *heap){
	if(heap_esta_vacio(heap)) return NULL;
	void* aux = heap->elementos[0];
	swap(heap->elementos, 0, (int)heap->cantidad-1);
	heap->cantidad--;
	downheap(heap->elementos, heap->cantidad, 0, heap->cmp);
	if (factor_de_carga(heap)<CARGA_MIN && heap->tam>TAM_INI){
		if(!heap_redimensionar(heap, (heap->tam)/FACT_REDIM)) return NULL;
	} 
	return aux;
}