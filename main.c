/* main.c - Sistema de paqueteria (Proyecto integrador AED2)*/
     
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include "paquete.h"
#include "cola.h"//cola de pendientes
#include "entregas.h"//pila de entregas
#include "grafo.h"//grafo de provincias
#include "dijkstra.h"
#include "archivos.h"//manejo de archivos
#include "reportes.h"
#include "mapa.h"//libreria que trabaja con raylib para graficar el mapa del pais
     
/* ---------- prototipado ---------- */
void procesarColores();
void menuPrincipal();
int mostrarOpciones();//se carga la opcion ingresada
int leerEntero(int, int);//podremos usar para el menu principal y tambien para los submenu
int obtenerUltimoID(tColaPaquetes*, tPilaEntregas*);
int cargarNuevoPaquete(tColaPaquetes *pCola, int pUltimoID);
bool entregarSiguientePaquete(tColaPaquetes *pCola, tPilaEntregas**, tMapa);
void menuMapa(tMapa*, tGrafico*);
void menuReportes(tColaPaquetes *, tPilaEntregas *);
void mostrarGrafico(tGrafico *, tMapa*);//para mostrar el grafico
/* ---------- funcion principal ---------- */
int main(){
	procesarColores();
	menuPrincipal();
	return 0;
}
/* ---------- implmementacion de funciones ---------- */
void procesarColores(){
	system("");//usamos para que el sistema reconozca siempre los colores de nuestro menu
}


/* Menu principal: aca vive el sistema y el switch
   que reparte el trabajo entre los TADs. */
   
void menuPrincipal(){
    tColaPaquetes cola;
    tPilaEntregas *pila;
    tMapa mapa;
    int ultimoID = 0;//inicializamos pero luego lo usaremos para poder darle un ID a cada paquete y que no se repita
    int opcion;
    
    //para graficar
    tGrafico *graficoArgentina;//tipo de dato para poder graficar el mapa interactivo
	

    crearCola(&cola);
    crearPila(&pila);
    
    cargarPaquetesDesdeArchivo(&cola, &pila);
    ultimoID = obtenerUltimoID(&cola, pila);
    
    bool mapaCargado = cargarMapaDesdeArchivo(&mapa);
	if(!mapaCargado){
	    printf(ROJO"\nAviso: no se pudo cargar el mapa, las rutas no estaran disponibles.\n"RESET);
	}

    do{
        opcion = mostrarOpciones();

        switch (opcion){
            case 1://cargar nuevo paquete
                ultimoID = cargarNuevoPaquete(&cola, ultimoID);
                break;

            case 2://mostrar pendientes
                mostrarCola(cola);
                break;

            case 3://mostrar pendientes por prioridad
                mostrarColaPorPrioridad(cola, true);
                break;

            case 4://entregar siguiente paquete
                entregarSiguientePaquete(&cola, &pila, mapa);//daria true mientras la cola no este vacia basicamente
                break;

            case 5://mostrar entregados
                mostrarEntregas(pila);
				break;

            case 6://mostrar ultima entrega
                ultimaEntrega(pila);
                break;

			case 7://menu mapa
				menuMapa(&mapa, graficoArgentina);
				break;
				
			case 8://menu reportes
				menuReportes(&cola, pila);
				break;
			
			case 9://guardar los datos del programa
				guardarPaquetesEnArchivo(&cola, &pila);
				printf("\nDatos guardados!\n");
				break;
				
            case 0:
                guardarPaquetesEnArchivo(&cola, &pila);
                printf("\nDatos guardados. Hasta luego.\n");
                break;

            default:
                printf("\nOpcion invalida.\n");//es imposible que ocurra porque la verificacion ya se hace en leerEntero
        }
    } while (opcion != 0);

    vaciarCola(&cola);
    vaciarPila(&pila);
}

/* Muestra las opciones y devuelve la elegida */
int mostrarOpciones(){
	//para mostrar la fecha
	time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    char fecha[11]; // "DD/MM/AAAA\0"

    strftime(fecha, sizeof(fecha), "%d/%m/%Y", tm_info);
    
	
	int opcion;
	printf("%s", LINEA);
    printf(NEGRITA"\n===== SISTEMA DE PAQUETERIA %s =====\n"RESET, fecha);
    printf(VERDE"1. Cargar nuevo paquete\n"RESET);
    printf(VERDE"2. Ver paquetes restantes\n"RESET);
    printf(VERDE"3. Ver restantes con prioridad\n"RESET);
    printf(VERDE"4. Entregar siguiente paquete\n"RESET);
    printf(VERDE"5. Ver paquetes entregados\n"RESET);
    printf(VERDE"6. Ver ultima entrega\n"RESET);
    printf(VERDE"7. Mapa de provincias\n"RESET);//submenu mapa
    printf(VERDE"8. Reportes\n"RESET);//submenu reportes
    printf(VERDE"9. Guardar datos\n"RESET);
    printf(ROJO"0. Salir\n"RESET);
	
	printf("%s", LINEA);
	
	printf("\nIngrese la opcion que desea: ");
	opcion = leerEntero(0, 9);//le pasamos los limites de opciones
    return opcion;
}

/* Carga un paquete, le asigna el ID siguiente y lo encola.
   Devuelve el ultimo ID usado. */
int cargarNuevoPaquete(tColaPaquetes *pCola, int pUltimoID){
    tPaquete nuevo;

    nuevo = cargarPaquete();
    pUltimoID++;
    nuevo.IDPaquete = pUltimoID;

    encolar(pCola, nuevo);

    printf(NEGRITA"\nPaquete cargado correctamente:\n"RESET);
    mostrarPaquete(nuevo);

    return pUltimoID;
}
/* desencola, marca como entregado, lo apila y muestra la ruta usada */
bool entregarSiguientePaquete(tColaPaquetes *pCola, tPilaEntregas** pPila, tMapa pMapa){
    tPaquete paquete;
    bool hayPaquete;

    hayPaquete = !colaVacia(*pCola);

    if (hayPaquete){
        paquete = desencolar(pCola);
        paquete.estado = 3;
        guardarEntrega(pPila, paquete);

        printf("\nPaquete entregado:\n");
        mostrarPaquete(paquete);
        
        printf("\nRuta utilizada:\n");
        mostrarCaminoMasCorto(dijkstra(paquete.provinciaOrigen-1, paquete.provinciaDestino-1, pMapa));//agrego un -1 ya que el paquete tiene el numero de provincia pero dijkstra necesita el indice
    }else{
        printf("\nNo hay paquetes restantes.\n");
    }

    return hayPaquete;
}

int leerEntero(int pMinimo, int pMaximo){
	int entero;
	do{
		scanf("%d", &entero);
		if(entero>pMaximo || entero<pMinimo){
			printf(ROJO"\nValor incorrecta, ingrese una valor dentro del rango (%d - %d): "RESET, pMinimo, pMaximo);
		}		
	}
	while(entero>pMaximo || entero<pMinimo);	
	
	return entero;
}

void menuMapa(tMapa *pMapa, tGrafico *pGrafico){
	int opcion, origen, destino;
	
	printf("%s", LINEA);
	
	printf(NEGRITA"\n--- MAPA DE PROVINCIAS ---\n"RESET);
	printf(VERDE"1. Ver conexiones de las provincias\n"); //muestra la matriz de adyacencia
	printf("2. Ver distancias en km\n");//muestra la matriz de pesos
	printf("3. Ver mapa interactivo\n"RESET);//grafico de aristas + dijkstra
	printf(ROJO"0. Volver\n"RESET);
	
	printf("%s", LINEA);
	
	printf("\nIngrese la opcion que desea: ");
	opcion = leerEntero(0, 3);//ya hace la verificacion por lo cual es innecesario usar el default
	
	switch(opcion){
		case 1: 
			mostrarMapaAdyacencia(*pMapa);
			break;
		case 2: 
			mostrarMapaDePesos(*pMapa);
			break;
		case 3://dijkstra
		
			//metodo anterior 
			//printf("Provincia de origen (1-24): ");
			//origen = leerEntero(1, 24);
			//printf("\nProvincia de destino (1-24): ");
			//destino = leerEntero(1, 24);
			//mostrarCaminoMasCorto(dijkstra(origen-1, destino-1, pMapa));//recibe la funcion aunque podriamos primero guardar el valor y luego pasarle una variable
			//se le pasa con -1 para que reciba los indices
			
			//metodo actual
			mostrarGrafico(pGrafico, pMapa);//grafico interactivo para mostrar dijkstra
			break;
	}
}

void menuReportes(tColaPaquetes *pCola, tPilaEntregas *pPila){//recibe la cola por puntero porque hay funciones dentro de las llamadas las cuales internamente usan encolar/desencolar
	int opcion, id;
	
	printf("%s", LINEA);
	
	printf(NEGRITA"\n--- REPORTES ---\n"RESET);
	printf(VERDE"1. Ver resumen general\n");
	printf("2. Reporte de entregados\n");
	printf("3. Paquetes por provincia de destino\n");
	printf("4. Buscar paquete por ID\n"RESET);
	printf(ROJO"0. Volver\n"RESET);
	
	printf("%s", LINEA);
	
	printf("\nIngrese la opcion que desea: ");
	opcion = leerEntero(0, 4);//ya hace la verificacion por lo cual es innecesario usar el default
	
	switch(opcion){
		case 1:
			reporteResumen(pCola, cantidadEntregas(pPila));
			break;
		case 2:
			reporteEntregados(pPila);
			break;
		case 3:
			reportePorDestino(pCola, pPila);
			break;	
		case 4:
			printf("\nID del paquete: ");
			id = leerEntero(1, 999999);
			buscarPaquete(pCola, pPila, id);
			break;
	}
}
/* retorna el mayor IDPaquete entre cola y pila (0 si no hay paquetes) */
int obtenerUltimoID(tColaPaquetes *pCola, tPilaEntregas *pPila){
	tVectorPaquetes vector;
	int cantidad, i, mayor;
	mayor = 0;
	
	cantidad = juntarPaquetes(pCola, pPila, vector);
	for(i=0; i<cantidad; i++){//para recorrer todos los paquetes existentes 
		if(vector[i].IDPaquete > mayor){
			mayor = vector[i].IDPaquete;//tecnica del supuesto prepo
		}
	}
	
	return mayor;
}

void mostrarGrafico(tGrafico *pGrafico, tMapa *pGrafoMapa){
	pGrafico = crearMapa("mapaArgentina.png");//funcion de mapa.h para graficar
	while(mapaAbierto(pGrafico)){
		dibujarCamino(pGrafico, pGrafoMapa);
	}
	borrarMapa(pGrafico);
}