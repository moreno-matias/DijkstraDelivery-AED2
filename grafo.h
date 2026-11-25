/*TAD de Grafos.
"Mapa del país"
*/
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>

#define MAXPROV 24 //Define un maximo para el vector de Provincias (vertices)
//Declaracion de tipos de datos personalizados

typedef int tProvincia; //Tipo de dato personalizado para vertices
typedef bool tConjuntoProvincias[MAXPROV]; 
//Estructura de la arista:
typedef struct{
	tProvincia origen; //Provincia de origen
	tProvincia destino; //Provincia de destino
	float distancia; //En kilometros
}tTrayectoria; //Trayectoria (Arista) entre una Provincia (vertice) y otra.

typedef float tConjuntoDeTrayectorias[MAXPROV][MAXPROV]; //Matriz para los pesos
//Estructura del grafo:
typedef struct{
	tConjuntoDeTrayectorias trayectorias; //Matriz de caminos
	tConjuntoProvincias provincias; //Vector de provincias
}tMapa;

//Prototipado de las funciones
void inicializarMapa(tMapa*); //Inicializa el grafo
bool existeCamino(tMapa, tTrayectoria); //Verifica la existencia de un camino entre una provincia (vertice) a otra
void agregarCamino(tMapa*, tTrayectoria); //Permite agregar un camino entre una o varias provincias
void eliminarCamino(tMapa*, tTrayectoria); //Permite eliminar un camino entre una provincia y otra.
void mostrarMapaAdyacencia(tMapa); //Funcion para mostrar el mapa de adyacencia
void mostrarMapaDePesos(tMapa); //Funciona para mostrar el mapa de pesos

//Definicion de funciones
void inicializarMapa(tMapa* pMapa){
	int i,j;
	for (i=0;i<MAXPROV;i++){
		pMapa->provincias[i]=true;
		for(j=0;j<MAXPROV;j++){
			pMapa->trayectorias[i][j]=-1.0;
		}
	}
}

bool existeCamino(tMapa pMapa, tTrayectoria pTrayecto){
	return pMapa.trayectorias[pTrayecto.origen][pTrayecto.destino]!=-1.0;
}
 
void agregarCamino(tMapa* pMapa, tTrayectoria pTrayecto){
	if(pTrayecto.origen >= 0 && pTrayecto.origen < MAXPROV && pTrayecto.destino >= 0 && pTrayecto.destino < MAXPROV && !existeCamino(*pMapa,pTrayecto) && pTrayecto.origen!=pTrayecto.destino && pTrayecto.distancia > 0){
		pMapa->trayectorias[pTrayecto.origen][pTrayecto.destino]=pTrayecto.distancia;
		pMapa->trayectorias[pTrayecto.destino][pTrayecto.origen]=pTrayecto.distancia;
		//printf("Camino agregado \n"); 
	}else{
		printf("No se pudo agregar camino \n");
	}
}

void eliminarCamino(tMapa* pMapa, tTrayectoria pTrayecto){
	if(existeCamino(*pMapa, pTrayecto)){
		pMapa->trayectorias[pTrayecto.origen][pTrayecto.destino]=-1.0;
		pMapa->trayectorias[pTrayecto.destino][pTrayecto.origen]=-1.0;
		printf("Camino eliminado exitosamente \n");
	}else{
		printf("No existe camino para eliminar \n");
	}
}

void mostrarMapaAdyacencia(tMapa pMapa){
	int i,j;
	for (i=0;i<MAXPROV;i++){
		printf("\t %d",i+1);
	}
	printf("\n");
	for (i=0;i<MAXPROV;i++){
		printf("%d \t",i+1);
		for(j=0;j<MAXPROV;j++){
			if(pMapa.trayectorias[i][j]!=-1.0){
				printf("1 \t");
			}else{
				printf("0 \t");
			}
		}
		printf("\n");
	}
}

void mostrarMapaDePesos(tMapa pMapa){
	int i,j;
	for (i=0;i<MAXPROV;i++){
		printf("\t %d",i+1);
	}
	printf("\n");
	for (i=0;i<MAXPROV;i++){
		printf("%d \t",i+1);
		for(j=0;j<MAXPROV;j++){
			if(pMapa.trayectorias[i][j]!=-1.0){
				printf("%.2f \t",pMapa.trayectorias[i][j]);
			}else{
				printf("- \t");
			}
		}
		printf("\n");
	}
}

