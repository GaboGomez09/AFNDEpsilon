#ifndef file_reader
#define file_reader

void LeerArchivo(char **alfabeto, char **transition, int **estados, int *estadoInicial, int **estadosFinales, int **estadosOrigen, int **estadosDestino, int *estadoLength, int *estadosFinalesLength, int *origenLength, int *destinoLength);
void LeerLinea(char **buffer, FILE *filePointer);
void ConvertirNumeros(int **arreglo,char *buffer, int *arregloLength);
void FiltrarCaracteres(char **alfabeto, char *buffer);
void guardarFuncionesDelta(int **estadosOrigen, int *origenLength,char **transition, int **estadosDestino, int *destinoLength, FILE *apuntadorArchivo);

void LeerArchivo(char **alfabeto, char **transition, int **estados, int *estadoInicial, int **estadosFinales, int **estadosOrigen, int **estadosDestino, int *estadoLength, int *estadosFinalesLength, int *origenLength, int *destinoLength){
  FILE* apuntadorArchivo = fopen("epsilon.txt", "r");
  char *buffer;
  if (apuntadorArchivo == NULL) {
    printf("No se pudo abrir el archivo.\n");
  } else {
      LeerLinea(&buffer, apuntadorArchivo);
      ConvertirNumeros(estados,buffer, estadoLength);
      free(buffer);


      LeerLinea(&buffer, apuntadorArchivo);
      FiltrarCaracteres(alfabeto, buffer);
      free(buffer);


      LeerLinea(&buffer, apuntadorArchivo);
      (*estadoInicial) = atoi(buffer);
      free(buffer);

      LeerLinea(&buffer, apuntadorArchivo);
      ConvertirNumeros(estadosFinales, buffer, estadosFinalesLength);
      free(buffer);

      guardarFuncionesDelta(estadosOrigen, origenLength, transition, estadosDestino, destinoLength, apuntadorArchivo);

      fclose(apuntadorArchivo);
  }

}

void guardarFuncionesDelta(int **estadosOrigen, int *origenLength,char **transition, int **estadosDestino, int *destinoLength, FILE *apuntadorArchivo){
  char buffer[100];
  char *tempBuf = (char*)malloc(10);
  int sizeOrigen = sizeof(int), sizeDestino = sizeof(int), sizeTransition = sizeof(char) + 1;
  *estadosOrigen = (int*)malloc(sizeOrigen*200);
  *transition = (char*)malloc(sizeTransition*200);
  *estadosDestino = (int*)malloc(sizeDestino*200);
  int i = 0;
  int k, longitud = 0;
  while(fscanf(apuntadorArchivo, "%s", buffer) != EOF){
       i = 0;
       k = 0;
       while ((buffer[k]) != ',') {
         tempBuf[i++] = buffer[k++];
       }
       k++;
       tempBuf[i] = '\0';
       (*estadosOrigen)[longitud] = atoi(tempBuf);
       (*transition)[longitud] = buffer[k];
       k+=2;
       i = 0;
       while ((buffer[k]) != '\0') {
         tempBuf[i++] = buffer[k++];
       }
       tempBuf[i] = '\0';
       (*estadosDestino)[longitud] = atoi(tempBuf);
       longitud++;
       *origenLength = longitud;
       *destinoLength = longitud;
    }
  free(tempBuf);
}

void LeerLinea(char **buffer, FILE *filePointer){
  char buf;
  int bufferSize = 1;
  *buffer = (char*)malloc(bufferSize);
  while ((buf = fgetc((filePointer))) != '\n') {
    *buffer = (char*)realloc(*buffer, bufferSize+= 1);
    (*buffer)[bufferSize - 2] = buf;
    (*buffer)[bufferSize - 1] = '\0';
  }
}

void ConvertirNumeros(int **arreglo,char *buffer, int *arregloLength){
  char numero[100];
  for (size_t l = 0; l < 100; l++) {
    numero[l] = 0;
  }
  int k = 0, arregloSize = sizeof(int);
  *arreglo = (int*)malloc(arregloSize);
  for (size_t i = 0; i < strlen(buffer); i++) {
    int j = 0;
    while (buffer[i] != ',') {
      if (buffer[i] == '\000') {
        break;
      }else{
        numero[j] = buffer[i];
        i++;
        j++;
      }
    }
    (*arreglo)[k] = atoi(numero);
    *arreglo = (int*)realloc(*arreglo, arregloSize += 4);
    k++;
    *arregloLength = k;

  }
}

void FiltrarCaracteres(char **alfabeto, char *buffer){
  int alfabetSize = 1;
  *alfabeto = (char*)malloc(alfabetSize);
  for (size_t i = 0; i < strlen(buffer); i++) {
    if (isspace(buffer[i]) || (buffer[i]) == ',') {

    } else {
      *alfabeto = (char*)realloc(*alfabeto, alfabetSize += 1);
      (*alfabeto)[alfabetSize - 2] = buffer[i];
      (*alfabeto)[alfabetSize - 1] = '\0';

    }
  }
}
#endif
