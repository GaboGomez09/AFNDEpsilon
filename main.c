#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "file_reader.h"
#include "automata.h"
int main(int argc, char const *argv[]){
  char *alfabeto = NULL, *transition = NULL;
  int *estados,  estadoInicial, *estadosFinales,  *estadosOrigen, *estadosDestino;
  int estadoLength, estadosFinalesLength, origenLength, destinoLength;
  LeerArchivo(argv[1], &alfabeto, &transition, &estados, &estadoInicial, &estadosFinales, &estadosOrigen, &estadosDestino, &estadoLength, &estadosFinalesLength, &origenLength, &destinoLength);

  for (size_t i = 0; i < 3; i++) {
    printf("\n");
  }
  printf("Estados: ");
  for (size_t i = 0; i < estadoLength; i++) {
    printf("[%d] ", estados[i]);
  }
  printf("\n");
  printf("Alfabeto: ");
  for (size_t i = 0; i < strlen(alfabeto); i++) {
    printf("[%c] ", alfabeto[i]);
  }
  printf("\n");

  printf("Estado inicial: [%d]\n", estadoInicial);

  printf("Estados finales: ");
  for (size_t i = 0; i < estadosFinalesLength; i++) {
    printf("[%d] ", estadosFinales[i]);
  }
  printf("\n\n");


  printf("Delta   |     E      ");
  for (size_t i = 0; i < strlen(alfabeto); i++) {
    printf("|     %c     ", alfabeto[i]);
  }

  printf("\n\n");

  for (size_t i = 0; i < estadoLength; i++) {
    printf("  %d ", estados[i]);
    printf("    {");
    for (size_t j = 0; j < strlen(transition); j++) {
      if (transition[j] == 'E' && (estadosOrigen[j] == estados[i])) {
        printf(" %d ", estadosDestino[j]);
      }
    }
    printf("}");

    for (size_t k = 0; k < strlen(alfabeto); k++) {
      printf("      {");
      for (size_t j = 0; j < strlen(transition); j++) {
        if (alfabeto[k] == transition[j] && (estadosOrigen[j] == estados[i])) {
          printf(" %d ", estadosDestino[j]);
        }
      }
      printf("}    ");

    }

    printf("\n\n");

  }



  char cadena[100] = "a";
  printf("Por favor ingrese la cadena que desea validar.\n");
  scanf("%s", cadena);
  Nodo* nodo = NULL;
  nodo = crearNodo(cadena);



  char *newalfabeto = (char*)malloc(strlen(alfabeto)+1);
  strcpy(newalfabeto,alfabeto);

  Estado* initialState;
  Estado* zombie = crearEstado(100);
  initialState = crearEstados(&zombie, alfabeto, estadoInicial, estados, estadoLength, transition, estadosOrigen, estadosDestino, origenLength, destinoLength);

  recorrerEstados(zombie, nodo, initialState, newalfabeto, estadosFinales, estadosFinalesLength); //solo queda verificar la ultima funcion

  free(zombie);
  free(initialState);
  free(newalfabeto);
  free(alfabeto);
  free(transition);
  free(estados);
  free(estadosFinales);
  free(estadosDestino);
  free(estadosOrigen);
  free(nodo);

  return 0;
}
