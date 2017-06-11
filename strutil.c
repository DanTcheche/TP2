#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*Recibe una cadena como parametro y devuelve un puntero a una copia de esa cadena*/
char* strdup(const char* cadena){
	char* cadena_nueva;
	cadena_nueva = malloc(strlen(cadena)+1);
	if(!cadena_nueva) return NULL;
	strcpy(cadena_nueva, cadena);
	cadena_nueva[strlen(cadena)] = '\0';
	return cadena_nueva;
}

/*Libera la memoria del strv y todos sus elementos.*/
void free_strv(char* strv[]){
	int pos = 0;
	while(strv[pos]){
		free(strv[pos]);
		pos++;
	}
	free(strv);
}

/*Cuenta la cantidad de apariciones de un caracter en una cadena.*/
int contar_sep(const char* cadena, char sep){
	int cant_de_caracteres = 0;
	int cant_seps = 0;
	while(cadena[cant_de_caracteres] != '\0'){
		if(cadena[cant_de_caracteres] == sep){
			cant_seps++;
		}
		cant_de_caracteres++;
	}
	return cant_seps;
}

int contar_carac_pos(const char* cadena, char sep){
	int cant_de_caracteres = 0;
	while(cadena[cant_de_caracteres] != '\0'){
		if(cadena[cant_de_caracteres] == sep) break;
		cant_de_caracteres++;
	}
	return cant_de_caracteres;
}

/* Devuelve en un arreglo dinámico terminado en NULL con todos los subsegmentos
 * de 'str' separados por el carácter 'sep'. Tanto el arreglo devuelto como las
 * cadenas que contiene son allocadas dinámicamente.
 *
 * Quien llama a la función toma responsabilidad de la memoria dinámica del
 * arreglo devuelto. La función devuelve NULL si falló alguna llamada a
 * malloc(), o si 'sep' es '\0'.
 */
char** split(const char* cadena, char sep){
	if(sep == '\0') return NULL;
	int cant_seps = contar_sep(cadena, sep);
	char** strv = malloc(sizeof(char**)*(cant_seps+2));
	if(!strv) return NULL;
	if(cant_seps == 0){
		strv[0] = strdup(cadena);
		strv[1] = NULL;
		return strv;
	}
	int pos = 0;
	int cont_aux = 0;
	while(pos != cant_seps+1){
		size_t caracteres = contar_carac_pos(&cadena[cont_aux], sep);
		char cadena_aux[caracteres+1];
		if(caracteres == 0){
			strv[pos] = strdup("");
		}
		else{
			for (int i = 0; i < caracteres; ++i){
				cadena_aux[i] = cadena[cont_aux+i];
			}
		cadena_aux[caracteres] = '\0';
		strv[pos] = strdup(cadena_aux);
		}
		pos++;
		cont_aux += (int)caracteres+1;
	}
	strv[cant_seps+1] = NULL;
	return strv;
}

/*Recibe un strv por parametro y devuelve la suma de los caracteres de todos los elementos.*/
size_t contar_caracteres(char** strv){
	int pos = 0;
	size_t largo_cadena = 0;
	while(strv[pos] != NULL){
		largo_cadena += strlen(strv[pos]);
		pos++;
	}
	largo_cadena += (pos);
	return largo_cadena;
}

/* Devuelve una cadena, allocada dinámicamente, resultado de unir todas las
 * cadenas del arreglo terminado en NULL 'strv'.
 *
 * Quien llama a la función toma responsabilidad de la memoria dinámica de la
 * cadena devuelta. La función devuelve NULL si no se pudo allocar memoria o
 * o si 'sep' es '\0'.
 */
char* join(char** strv, char sep){
	if(sep == '\0') return NULL;
	size_t caracteres = contar_caracteres(strv);
	char* cadena = malloc(((int)caracteres)+2);
	if(!cadena) return NULL;
	int indice = 0;
	int pos = 0;
	while(strv[indice] != NULL){
		int contador = 0;
		char caracter = strv[indice][contador];
		while(caracter != '\0'){
			cadena[pos] = caracter;
			contador++;
			pos++;
			caracter = strv[indice][contador];
		}
		cadena[pos] = sep;
		pos++;
		indice++;
	}
	if (caracteres == 0){
		cadena[caracteres] = '\0';
	}
	else{
		cadena[caracteres-1] = '\0';
	}
	return cadena;
}