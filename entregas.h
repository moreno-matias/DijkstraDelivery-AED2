#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
typedef struct nodoEntrega{
	tPaquete paquete;
	int cantEntregas;// Cada nodo guarda cuantas entregas hay HASTA el, asi que el nodo de la cima tiene el total
	struct nodoEntrega* siguiente;
}tPilaEntregas;
//Declaracion de Funciones
void crearPila(tPilaEntregas**);
bool sinEntregas(tPilaEntregas*);
int cantidadEntregas(tPilaEntregas*);
void guardarEntrega(tPilaEntregas**,tPaquete);
void quitarUltimaEntrega(tPilaEntregas**);
void mostrarEntregas(tPilaEntregas*);
void ultimaEntrega(tPilaEntregas*);
void vaciarPila(tPilaEntregas**);
//Implementacion de Funciones
void crearPila(tPilaEntregas** pEntregas){
	*pEntregas=NULL;
}


bool sinEntregas(tPilaEntregas* pEntregas){
	return pEntregas==NULL;
}

int cantidadEntregas(tPilaEntregas* pEntregas){
	int cantidad=0;
	if(!sinEntregas(pEntregas)){
		cantidad=pEntregas->cantEntregas;
	}
	return cantidad;
}

void guardarEntrega(tPilaEntregas** pEntrega,tPaquete pPaquete){
	tPilaEntregas* nuevoPaquete;
	nuevoPaquete=(tPilaEntregas*) malloc(sizeof(tPilaEntregas));
	nuevoPaquete->paquete=pPaquete;
	
	nuevoPaquete->cantEntregas=cantidadEntregas(*pEntrega)+1;//aumentamos el contador de la cantidad de entregas
	nuevoPaquete->siguiente=*pEntrega;
	*pEntrega=nuevoPaquete;
}



void quitarUltimaEntrega(tPilaEntregas** pEntrega){
	if(sinEntregas(*pEntrega)){
		return;//si no hay entregas simplemente sale del programa
	}else{
		tPilaEntregas* aux;
		aux=*pEntrega;
		*pEntrega=aux->siguiente;
		free(aux);
		aux=NULL;
		//no disminuimos el contador ya que directamente se libera su memoria
	}
}

void mostrarEntregas(tPilaEntregas* pEntrega){
	if(sinEntregas(pEntrega)){
		printf("No hay entregas\n");
	}else{
		tPilaEntregas* aux;
		aux=pEntrega;
		printf("\t---Paquetes Entregados: %d---\n",cantidadEntregas(pEntrega));
		while(aux!=NULL){
			mostrarPaquete(aux->paquete);//funcion del TAD de paquete.h
			aux=aux->siguiente;		
		}
	}
}

void ultimaEntrega(tPilaEntregas* pPaquete){
	if(sinEntregas(pPaquete)){
		printf("Sin entregas\n");
	}else{
		mostrarPaquete(pPaquete->paquete);
	}
}

void vaciarPila(tPilaEntregas** pEntrega){
	while(!sinEntregas(*pEntrega)){
		quitarUltimaEntrega(pEntrega);
	}
}