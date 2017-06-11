#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#define ERROR_PARAMETROS -4

/*Función que verifica que la cantidad de parametros sea correcta y que ambos
parametro sean dígitos.*/
bool chequear_parametros(int argc, char* argv[]){
   if(argc != 3){
      printf("Ingresó un número incorrecto de parametros.\n");
      return false;
   }
   char* cantidad = argv[1];
   char* cantidad2 = argv[2];
   if(!chequear_parametros(cantidad) && !chequear_parametros(cantidad2)){
      printf("No ingresó un numero en ambos parametros.\n");
      return false;      
   }
   return true;
}

bool chequear_digitos(char* parametro){
   for(int i = 0; parametro[i] != '\0'; i++){
      if(!isdigit(cantidad[i])){
         return false;
      }
   }
   return true;
}

/*Main de la función. Si hubo un error en los parametros devuelve -4, de lo contrario devuelve 0.*/
int main(int argc, char* argv[]){
   if(!(chequear_parametros(argc, argv))) return ERROR_PARAMETROS;
   filesplit(argv[1], atoi(argv[2]));
   return 0;
   
}
