#ifndef automata
#define automata

#include <string.h>

typedef struct estado Estado;
typedef struct nodo Nodo;

struct estado{
    int estadoID;
    Estado* transiciones[100][100]; //arreglo bidimensional de direcciones de estados
    Estado* apuntadorTemp;
};

struct nodo{
    char cadena[100];
    int posicionCadena;
    int posicionRecorrido;
    int recorrido[1000];
    Estado* siguiente;
};

typedef struct lista{
  Estado* cabeza;
}Lista;

Estado* crearEstado(int ID);
void insertarFinal(Lista* lista, int ID);
Estado* crearEstados(Estado** zombie, char*alfabeto, int estadoInicial, int *estados, int estadoLength, char* transition, int *estadosOrigen, int* estadosDestino, int origenLength, int destinoLength);
void inicializarTransiciones(Lista* lista);
int posicionDeCaracter(char letra, char* alfabeto);
Estado* direccionDeEstadoDestino(Lista* lista, int destinoID);
Nodo* crearNodo(char palabra[100]);
void recorrerEstados(Estado* zombie, Nodo* nodo, Estado* estadoActual, char* alfabeto, int* estadosFinales, int estadosFinalesLength);
Nodo* clonarNodo(Nodo* nodo);
int yaLlegoAUnEstadoFinal(Nodo* nodo, int *estadosFinales, int estadosFinalesLength);
int yaConsumioTodasLasLetras(Nodo* nodo);
int existeTransicionEpsilon(Nodo * nodo);
void mostrarRecorrido(Nodo* nodo);
int existeTransicion(Nodo *nodo, char *alfabeto, Estado* zombie);

void recorrerEstados(Estado* zombie, Nodo* nodo, Estado* estadoActual, char* alfabeto, int* estadosFinales, int estadosFinalesLength){
  //Estado* pasado = nodo->siguiente;
  nodo->siguiente = estadoActual;
  Nodo* clon = NULL;
  int posicion;
  nodo->recorrido[nodo->posicionRecorrido] = nodo->siguiente->estadoID;

  if (yaConsumioTodasLasLetras(nodo) && !existeTransicionEpsilon(nodo)) {
    if (yaLlegoAUnEstadoFinal(nodo, estadosFinales, estadosFinalesLength)) {
      mostrarRecorrido(nodo);
    }
  } else {
    clon = clonarNodo(nodo);
    //Hace el recorrido respecto a la letra que está actualmente anlizando
    if (!yaConsumioTodasLasLetras(clon) && existeTransicion(clon, alfabeto, zombie)) {
      posicion = posicionDeCaracter(clon->cadena[clon->posicionCadena], alfabeto);
      clon->posicionRecorrido++;
      clon->posicionCadena++;
      int k = 0;
      while (clon->siguiente->transiciones[k][posicion] != NULL) {
        recorrerEstados(zombie, clon, clon->siguiente->transiciones[k][posicion], alfabeto, estadosFinales, estadosFinalesLength);
        k++;
        clon->siguiente = nodo->siguiente;
      }
      clon->posicionRecorrido--;
      clon->posicionCadena--;
    }

    //Hace el recorrido respecto a las transiciones epsilon
    if (existeTransicionEpsilon(clon)) {
      clon->posicionRecorrido++;
      int k = 0;
      while (clon->siguiente->transiciones[k][31] != NULL) {
        recorrerEstados(zombie, clon, clon->siguiente->transiciones[k][31], alfabeto, estadosFinales, estadosFinalesLength);
        k++;
        clon->siguiente = nodo->siguiente;
      }
      clon->posicionRecorrido--;
    }
  }

  free(clon);
}

int existeTransicion(Nodo *nodo, char *alfabeto, Estado* zombie){
  int posicion = posicionDeCaracter(nodo->cadena[nodo->posicionCadena], alfabeto);
  if (nodo->siguiente->transiciones[0][posicion] == zombie) {
    return 0;
  }else{
    return 1;
  }
}

void mostrarRecorrido(Nodo* nodo){
  printf("[");
  for (size_t i = 0; i < nodo->posicionRecorrido + 1; i++) {
    printf(" %d ", nodo->recorrido[i]);
  }
  printf("]\n");
}

int existeTransicionEpsilon(Nodo * nodo){
  if (nodo->siguiente->transiciones[0][31] == NULL) {
    return 0;
  }else{
    return 1;
  }
}

int yaConsumioTodasLasLetras(Nodo* nodo){
  if (nodo->posicionCadena == (strlen(nodo->cadena))) {
    return 1;
  }
  return 0;
}

int yaLlegoAUnEstadoFinal(Nodo* nodo, int *estadosFinales, int estadosFinalesLength){
  int existe = 0;
  for (size_t i = 0; i < estadosFinalesLength; i++) {
    if (nodo->recorrido[(nodo->posicionRecorrido)] == estadosFinales[i]) {
      existe = 1;
    }
  }
  return existe;
}

Nodo* clonarNodo(Nodo* nodo){
    Nodo* clon = crearNodo(nodo->cadena);
    for (size_t i = 0; i <= nodo->posicionRecorrido; i++) {
        clon->recorrido[i] = nodo->recorrido[i];
    }
    clon->posicionCadena = nodo->posicionCadena;
    clon->siguiente = nodo->siguiente;
    clon->posicionRecorrido = nodo->posicionRecorrido;
    return clon;
}

Nodo* crearNodo(char palabra[100]){
    Nodo* nodo = (Nodo*)malloc(sizeof(Nodo));
    strcpy(nodo->cadena, palabra);
    nodo->posicionRecorrido = 0;
    nodo->siguiente = NULL;
    nodo->posicionCadena = 0;
    for (size_t i = 0; i < 100; i++) {
        nodo->recorrido[i] = 9;
    }
    return nodo;
}

void insertarFinal(Lista* lista, int ID){
  Estado* estado = crearEstado(ID);
  Estado* puntero= NULL;
  if (lista->cabeza == NULL) {
    lista->cabeza = estado;

  } else {
    puntero = lista->cabeza;
    while (puntero->apuntadorTemp) {
      puntero = puntero->apuntadorTemp;
    }
    puntero->apuntadorTemp = estado;
  }
}

Estado* crearEstados(Estado** zombie, char* alfabeto, int estadoInicial, int *estados, int estadoLength, char* transition, int *estadosOrigen, int* estadosDestino, int origenLength, int destinoLength){
    Lista* lista = (Lista*)malloc(sizeof(Lista));
    Estado* initialState = NULL;
    Estado* punteroAux = NULL;
    lista->cabeza = NULL;

    for (size_t i = 0; i < estadoLength; i++) {
        insertarFinal(lista, estados[i]);
    }

    initialState = lista->cabeza;
    while ((initialState->estadoID) != estadoInicial) {
        initialState = initialState->apuntadorTemp;
    }

    inicializarTransiciones(lista); //iniciliza matriz de puntores(ambiguedades) en NULL
    punteroAux = lista->cabeza; //Asigna las direcciones de las transicciones a la
    int posicion;               //matriz de direcciones
    while (punteroAux) {
        for (size_t j = 0; j < origenLength; j++) {
            if ((punteroAux->estadoID) == estadosOrigen[j]) {
                if (transition[j] == 'E') {
                    posicion = 31;
                }else{
                    posicion = posicionDeCaracter(transition[j], alfabeto);
                }
                int k = 0;
                while (punteroAux->transiciones[k][posicion] != NULL) {
                    k++;
                }
                punteroAux->transiciones[k][posicion] = direccionDeEstadoDestino(lista, estadosDestino[j]);
            }
        }
        punteroAux = punteroAux->apuntadorTemp;
    }


    punteroAux = lista->cabeza;

    while (punteroAux) {
        for (size_t l = 0; l < strlen(alfabeto); l++) {
            if (punteroAux->transiciones[0][l] == NULL) {
                punteroAux->transiciones[0][l] = *zombie;
            }
        }
        punteroAux = punteroAux->apuntadorTemp;
    }

    for (size_t z = 0; z < 100; z++) {
        (*zombie)->transiciones[0][z] = *zombie;
    }


    free(lista);
    return initialState;
}

Estado* direccionDeEstadoDestino(Lista* lista, int destinoID){
  Estado* direccionDestino = NULL;
  Estado* puntero = lista->cabeza;

  while (puntero) {
    if (puntero->estadoID == destinoID) {
      direccionDestino = puntero;
    }
    puntero = puntero->apuntadorTemp;
  }

  return direccionDestino;
}

int posicionDeCaracter(char letra, char* alfabeto){
    int posicion = 0;
    while (alfabeto[posicion] != letra) {
        posicion++;
    }
    return posicion;
}

void inicializarTransiciones(Lista* lista){
  Estado* puntero = lista->cabeza;
  while (puntero->apuntadorTemp) {
    for (size_t i = 0; i < 100; i++) {
      for (size_t j = 0; j < 100; j++) {
        puntero->transiciones[j][i] = NULL;
      }
    }
    puntero = puntero->apuntadorTemp;
  }
}

Estado* crearEstado(int ID){
  Estado* estado = (Estado*)malloc(sizeof(Estado));
  estado->estadoID = ID;
  estado->apuntadorTemp = NULL;
  return estado;
}

#endif
