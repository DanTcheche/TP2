#include <stdbool.h>
#include <stddef.h>

void free_strv(char** strv);
char* strdup(const char* old);
int obtener_cantidad(const char* str, char sep);
char** split(const char* str, const char sep);
void contar_caracteres(char** strv, int* cantidad);
char* join(char** strv, char sep);