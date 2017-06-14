#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

	/***FUNCIONES AUXILIARES***/

/*
 Pre: La memoria no fue liberada previamente.
 Post: Libera la memoria de los componentes del arreglo de cadenas y luego el arreglo.
 */
void free_strv(char** strv){
	int indice=0;
	while(strv[indice]){
		free(strv[indice]);
		indice++;
	}
	free(strv);
}


char* strdup(const char* old){
	char* new;
	new = malloc(strlen(old)+1);
	if(!new) return NULL;
	
	strcpy(new,old);
	return new;
}
/*Obtiene la cantidad cadenas que va a haber en el arreglo + 1 por NULL*/
void obtener_cantidad(int* cant,const char* str, char sep){
	int indice =0;
	char c = str[indice];
	
	while(c != '\0'){
		if (c == sep){
			*(cant)+= 1;


		}
		indice++;	
		c= str[indice];
		
	}
	*(cant)+=1 ;

}	
void contar_caracteres(char** strv, int* cantidad){
	int indice = 0;
	while(strv[indice]){
		
		(*cantidad) += (int)strlen(strv[indice]);
		indice++;
	}
	*cantidad += indice;
}

/*** JOIN ***/

/*
 * Devuelve una cadena, allocada dinámicamente, resultado de unir todas las
 * cadenas del arreglo terminado en NULL 'strv'.
 *
 * Quien llama a la función toma responsabilidad de la memoria dinámica de la
 * cadena devuelta. La función devuelve NULL si no se pudo allocar memoria,
 * si 'sep' es '\0' o si recibe un strv vacio.
 */
char* join(char** strv, char sep){
	if(sep == '\0') return NULL;
	char new_sep[] = {sep,'\0'};
	int cantidad = 0;

	contar_caracteres(strv,&cantidad);
	if(!cantidad) cantidad = 1;

	char* res= malloc(cantidad*sizeof(char));
	if(!res) return NULL;
	int indice = 0;
	res[0] = '\0';
	char* posicion_mem = res;

	while(strv[indice]){
		
		posicion_mem = strcat( posicion_mem,strv[indice]);

		posicion_mem += strlen(strv[indice]);

		if(strv[indice+1]) posicion_mem = strcat(posicion_mem,&new_sep[0]);

		posicion_mem++;
		indice++;

	}
	return res;
}


/*** SPLIT ***/

/*Devuelve en un arreglo dinámico terminado en NULL con todos los subsegmentos
  de ‘str’ separados por el carácter ‘sep’. Tanto el arreglo devuelto como las
  cadenas que contiene son allocadas dinámicamente.
 
  Quien llama a la función toma responsabilidad de la memoria dinámica del
  arreglo devuelto. La función devuelve NULL si falló alguna llamada a
  malloc(), o si ‘sep’ es '\0'.
 */
char** split(const char* str, const char sep){
	if(sep == '\0') return NULL;
	int cantidad = 1;
	obtener_cantidad(&cantidad,str, sep);



	char** res = malloc((sizeof(char*)) * (cantidad));

	if(!res)return NULL;
	int indice = 0;
	int contador_max = 0;
	int contador_min = 0;
	char caracter = str[contador_max];
	int largo = (int)strlen(str)+1;


	for(int in = contador_min; in < largo; in++){
		if (caracter == sep || caracter == '\0'){
			char aux[contador_max - contador_min + 1];
			int posicion_aux = 0;
			for(int i = contador_min; i < contador_max; i++){
				aux[posicion_aux] = str[i];
				posicion_aux++;
			}
			aux[contador_max - contador_min] = '\0';
			contador_min = contador_max+1;
			res[indice] = strdup(aux);
			indice++;
		}
		contador_max++;
		caracter = str[contador_max];
	}
	res[indice] = NULL;
	return res;
}