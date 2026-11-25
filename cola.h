/* TAD Cola implementada con lista enlazada dinamica.
   Guarda los paquetes RESTANTES (pendientes de entrega).
   El primero que entra es el primero en ser despachado.*/
//IMPORTANTE: main.c tiene que agregar "paquete.h" antes de "cola.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* Declaración de tipos de datos personalizados */     
  
//estructura de cada nodo de la cola
typedef struct nodoCola{
	tPaquete paquete;
	struct nodoCola* siguiente;
}tNodoCola; 

//tipo de dato cola
typedef struct{
	tNodoCola* principio;
	tNodoCola* final;
	int cantidad;	
}tColaPaquetes;

/* Declaración de prototipos de funciones */

void crearCola(tColaPaquetes*); //inicializar
bool colaVacia(tColaPaquetes);
int cantidadCola(tColaPaquetes);
void encolar(tColaPaquetes*, tPaquete);
tPaquete desencolar(tColaPaquetes*);
void vaciarCola(tColaPaquetes*);
void mostrarCola(tColaPaquetes);
void mostrarColaPorPrioridad(tColaPaquetes, bool);
//bool verFrente(tColaPaquetes, tPaquete*);

/* Implementacion de funciones */
void crearCola(tColaPaquetes* restantes){
	restantes->principio = NULL;
	restantes->final = NULL;
	restantes->cantidad = 0;
}

bool colaVacia(tColaPaquetes restantes){
	return restantes.principio == NULL && restantes.final == NULL;
}

int cantidadCola(tColaPaquetes restantes){
	return restantes.cantidad;
}

void encolar(tColaPaquetes *restantes, tPaquete paquete){
	tNodoCola* nuevoPaquete = (tNodoCola*)malloc(sizeof(tNodoCola)); //reservo el espacio en memoria
	if(nuevoPaquete == NULL){
		printf("Error: no hay memoria para agregar el paquete.\n");
		return;
	}
	
	nuevoPaquete->paquete = paquete; //asigno el paquete del parametro
	nuevoPaquete->siguiente = NULL; // Indicar que el nuevo nodo no apunta a ningún otro nodo porque es el último

	if(colaVacia(*restantes)){
		//si la cola está vacia ambos punteros apuntan al nuevoPaquete
		restantes->final = nuevoPaquete;
		restantes->principio = nuevoPaquete;
	}
	else{
		restantes->final->siguiente = nuevoPaquete;
		restantes->final = nuevoPaquete;
	}
	restantes->cantidad++;
}

tPaquete desencolar(tColaPaquetes* restantes){
	tNodoCola* nodoSuprimir;
	tPaquete paqueteSacado;
	if(colaVacia(*restantes)){
		crearPaquete(&paqueteSacado);   // paquete "vacio": IDPaquete = 0
		return paqueteSacado;
		//podría agregarse un mensaje en pantalla
	}
	else{
		nodoSuprimir = restantes->principio;
		paqueteSacado = nodoSuprimir->paquete;// lo guardo antes de borrar
		if(restantes->principio == restantes->final){
			//significa que hay un solo elemento por lo cual podemos inicializar
			crearCola(restantes);
		}
		else{
			restantes->principio = nodoSuprimir->siguiente;
			restantes->cantidad--;
		}
		// liberar la memoria de nodo a suprimir	
		free(nodoSuprimir);
		return paqueteSacado;
	}	
}

void vaciarCola(tColaPaquetes *restantes){
	while(!colaVacia(*restantes)){
		desencolar(restantes);
	}
}

void mostrarCola(tColaPaquetes restantes){
	tNodoCola* aux = restantes.principio;// puntero para recorrer, empieza en el primero
	
	if(colaVacia(restantes)){
		printf("No hay paquetes pendientes.\n");
	}
	else{
		printf("=== Paquetes pendientes: %d ===\n", restantes.cantidad);
		while(aux != NULL){
			mostrarPaquete(aux->paquete);// funcion del TAD Paquete
			aux = aux->siguiente;// paso al proximo nodo
		}
	}
}

void mostrarColaPorPrioridad(tColaPaquetes restantes, bool conPrioridad){
	tNodoCola *aux = restantes.principio;
	int contador = 0;
	
	printf("*** Paquetes %s prioridad ***\n", conPrioridad ? "con" : "sin");
	
	if(colaVacia(restantes)){
		printf("No hay paquetes pendientes.\n");
	}
	else{
		while(aux != NULL){
			if(aux->paquete.prioridad == conPrioridad){   // solo los que coinciden
				mostrarPaquete(aux->paquete);
				contador++;
			}
			aux = aux->siguiente;
		}
		printf("Total: %d\n", contador);
	}
}