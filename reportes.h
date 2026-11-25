/*TAD de reportes.h donde estaran implementadas las funciones
 para el corte de control y el ordenamiento y busqueda.*/
//se agrega en el main solamente

/*La pila se recibe como puntero a la cima (tPilaEntregas*): los
   reportes solo la leen, nunca la modifican.

   Estados del paquete (campo estado de tPaquete):
       1 - En sucursal
       2 - En camino
       3 - Entregado*/
#include <stdio.h>

#define MAX_PAQUETES 500   /* maximo de paquetes que entran en un reporte */

/* Declaracion de tipos de datos personalizados */

typedef struct{
    int enSucursal;
    int enCamino;
}tConteoEstados;//Resultado del conteo de paquetes restantes por estado

//Lista de paquetes para ordenar y buscar ya que usamos vectores estaticos para ello

typedef tPaquete tVectorPaquetes[MAX_PAQUETES];

/* Declaracion de prototipos de funciones */
tConteoEstados contarPorEstado(tColaPaquetes *);
void reporteResumen(tColaPaquetes *, int);
void reporteEntregados(tPilaEntregas *);
int juntarPaquetes(tColaPaquetes *, tPilaEntregas *, tVectorPaquetes);//recibe la cola por puntero ya que queremos mantener el orden original
void ordenarPorID(tVectorPaquetes, int);
void ordenarPorDestino(tVectorPaquetes , int);
int busquedaBinariaPaquete(tVectorPaquetes , int, int);
void buscarPaquete(tColaPaquetes *, tPilaEntregas *, int);
void reportePorDestino(tColaPaquetes *, tPilaEntregas *);
//funciones del esquema de corte de control para el vector
void inicializacion();
void procesoCorte();
void finalizacion();
void principioCorte();
void unaProvincia();
void finCorte();
void leerPaquete();

/*Declaracion de variables globales para el corte de control*/
tPaquete paquete; //para el paquete actual dentro del bucle
tVectorPaquetes paquetesCorte; //para los paquetes ordenados por provinciaDestino
int cantPaquetes;
int posActual;
bool finVector;
int provinciaDestinoAnt; //campo de control anterior
int cantProv, prioridadProv, entregadosProv; //contadores parciales (de cada provincia)
int cantTotal, prioridadTotal, entregadosTotal; //contadores generales 
/*Implementacion de las funciones*/
//recorre la cola SIN perder el orden: saca cada paquete con desencolar y lo vuelve a encolar
tConteoEstados contarPorEstado(tColaPaquetes *cola){
    int i, n;
    tPaquete p;
    tConteoEstados conteo;//dato compuesto para listar los paquetes por estado

    conteo.enSucursal = 0;
    conteo.enCamino = 0;

    n = cantidadCola(*cola);
    for (i = 0; i < n; i++)
    {
        p = desencolar(cola);
        if (p.estado == 1)
            conteo.enSucursal++;
        else if (p.estado == 2)
            conteo.enCamino++;
        encolar(cola, p);
    }
    //como desencolar esta seguido de un encolar, la cola sigue igual que como estaba al principio
    
    return conteo;//Devuelve cuantos restantes estan en sucursal y cuantos en camino
}
//Resumen general: restantes (con detalle por estado), entregados y total de paquetes, cantEntregados lo da la pila (entregas.h)
void reporteResumen(tColaPaquetes *cola, int cantEntregados){
    tConteoEstados conteo;
    int restantes;

    conteo = contarPorEstado(cola);
    restantes = cantidadCola(*cola);

	printf("%s \n",LINEA);
    printf("\n===== RESUMEN GENERAL =====\n");
    printf("Paquetes restantes:  %d\n", restantes);
    printf("   En sucursal:      %d\n", conteo.enSucursal);
    printf("   En camino:        %d\n", conteo.enCamino);
    printf("Paquetes entregados: %d\n", cantEntregados);
    printf("---------------------------\n");
    printf("Total de paquetes:   %d\n", restantes + cantEntregados);
    printf("%s \n",LINEA);
}
//Recorre la pila y va mostrando los entregados y al final muestra la cantidad total de entregados
void reporteEntregados(tPilaEntregas *pPila){
	tPilaEntregas *aux = pPila; //auxiliar para recorrer los entregados
	printf("%s ",LINEA);
	printf("\n===== PAQUETES ENTREGADOS (del ultimo al primero) =====\n");
	
	if(sinEntregas(pPila)){
		printf("Todavia no hay paquetes entregados\n");
	}else{
		while(!sinEntregas(aux)){
			mostrarPaquete(aux->paquete);//funcion de paquete.h
			aux = aux->siguiente;
		}
		printf("\nTotal de paquetes entregados: %d\n", cantidadEntregas(pPila));//funcion de entregas.h que devuelve el total de entregas
	}
	printf("%s \n",LINEA);	
}
//Junta en el vector todos los paquetes (pendientes y entregados) y, si hay mas que MAX_PAQUETES, los sobrantes no entran
int juntarPaquetes(tColaPaquetes *cola, tPilaEntregas *pPila, tVectorPaquetes pVector){
	int i, tamanioCola;
	int cantidad = 0;//cantidad entre los pendientes y los entregados
	tPaquete paquete;
	tPilaEntregas *aux;
	
	tamanioCola = cantidadCola(*cola);
	for(i=0; i<tamanioCola; i++){//para recorrer la cola
		paquete = desencolar(cola);
		
		if(cantidad < MAX_PAQUETES){
			pVector[cantidad] = paquete;
			cantidad++;
		}
		
		encolar(cola, paquete);
	}
	//recorremos con un for ya que queremos mantener el orden de la cola como estaba 
	
	aux = pPila; //para recorrer la pila
	while(!sinEntregas(aux) && cantidad < MAX_PAQUETES){
		pVector[cantidad] = aux->paquete;
		cantidad++;
		aux = aux->siguiente;
	}
	//como solo accedemos al valor podemos recorrerla con el while y un aux
	
	return cantidad;//devuelve la cantidad de paquetes cargados
}
//Ordena el vector por IDPaquete con el metodo de intercambioDirectoOptimizado, los vectores se pasan por referencia, asi que queda ordenado el original.
void ordenarPorID(tVectorPaquetes pVector, int pCantidad){
	int i, j;
	tPaquete aux;
	bool flag = false;//flag para optimizar el metodo de intercambio directo
	
	for(i=0; (flag == false) && (i<pCantidad-1); i++){
		flag = true;//para que si el vector ya esta ordenado no se vuelva a recorrer
		for( j=0; j<pCantidad-1-i; j++ ) {
			
			if( pVector[j].IDPaquete > pVector[j+1].IDPaquete ) {
				aux =  pVector[j];
				pVector[j] =  pVector[j+1];
				pVector[j+1] = aux;
				flag = false;
			}		
		}
	}	
}

//ordena el vector por provinciaDestino con el metodo de intercambioDirectoOptimizado, necesario para el corte de control de reportePorDestino.
void ordenarPorDestino(tVectorPaquetes pVector, int pCantidad){
	int i, j;
	tPaquete aux;
	bool flag = false;//flag para optimizar el metodo de intercambio directo
	
	for(i=0; (flag == false) && (i<pCantidad-1); i++){
		flag = true;//para que si el vector ya esta ordenado no se vuelva a recorrer
		for( j=0; j<pCantidad-1-i; j++ ) {
			
			if( pVector[j].provinciaDestino > pVector[j+1].provinciaDestino ) {
				aux =  pVector[j];
				pVector[j] =  pVector[j+1];
				pVector[j+1] = aux;
				flag = false;
			}		
		}
	}
}

//retorna la posicion del paquete, si no esta retorna -1
int busquedaBinariaPaquete(tVectorPaquetes pVector, int pCantidad, int pId){
	int extInf = 0;
    int extSup = pCantidad - 1;
    int posMedio = -1;
    bool encontrado = false;
    
    while (!encontrado && (extSup >= extInf)){
        /* Determinar la posicion central */
        posMedio = (extInf + extSup) / 2;

        /* Consultar si el elemento buscado es el que esta en la posicion central */
        if (pId == pVector[posMedio].IDPaquete){
            encontrado = true;
        }else{
            /* Determinar de que lado esta el elemento */
            if (pId > pVector[posMedio].IDPaquete){
                extInf = posMedio + 1;
            }else{
                extSup = posMedio - 1;
            }
        }
    }

    if (!encontrado){
        posMedio = -1;
    }

    return posMedio;//contiene la posicion si se encontro y si no contiene -1
}
//busca un paquete por id en toda la cola y toda la pila, pasa todo a un vector, lo ordena y aplica busqueda binaria
void buscarPaquete(tColaPaquetes *pCola, tPilaEntregas *pPila, int pId){//es la funcion que luego usamos en el main.c
	tVectorPaquetes paquetes;
	int cantidad, pos;
	
	cantidad = juntarPaquetes(pCola, pPila, paquetes);
	ordenarPorID(paquetes, cantidad);
	pos = busquedaBinariaPaquete(paquetes, cantidad, pId);
	
	if(pos == -1){
		printf(ROJO"\nNo se encontro ningun paquete con ID %d.\n"RESET, pId);
	}else{
		mostrarPaquete(paquetes[pos]);
	}
}
/* ---------- Corte de control por provincia de destino ---------- */
/*es la funcion que luego usamos en el main.c: junta todos los paquetes en un vector, lo ordena por provinciaDestino
y luego aplica la tecnica de corte de control pero sobre el vector ordenado para poder imprimir por provincias*/
void reportePorDestino(tColaPaquetes *pCola, tPilaEntregas *pPila){
	cantPaquetes = juntarPaquetes(pCola, pPila, paquetesCorte);
	ordenarPorDestino(paquetesCorte, cantPaquetes);
	
	inicializacion();
	procesoCorte();
	finalizacion();
}

void inicializacion(){
	posActual = 0;
	finVector = false;
	
	crearPaquete(&paquete);//para que no contenga basura lo inicializamos
	leerPaquete();
	provinciaDestinoAnt = paquete.provinciaDestino;
	
	cantTotal = 0;
	prioridadTotal = 0;
	entregadosTotal = 0;
	printf("%s \n",LINEA);
	printf("\n===== PAQUETES POR PROVINCIA DE DESTINO =====\n");
}

void procesoCorte(){
	//recorre el vector hasta el final
	while(!finVector){
		principioCorte();
		//con este while trabajamos con cada grupo
		while(!finVector && paquete.provinciaDestino == provinciaDestinoAnt){
			unaProvincia();
		}
		//cuando termine el bucle significa que ya terminamos ese grupo
		finCorte();
	}
}

void principioCorte(){
	cantProv = 0;
	prioridadProv = 0;
	entregadosProv = 0;
}

void unaProvincia(){
	cantProv++;
	if(paquete.prioridad){
		prioridadProv++;
	}
	if(paquete.estado == 3){
		//3 es entregado
		entregadosProv++;
	}
	
	leerPaquete();
}

void finCorte(){
	printf("%s: %d paquetes (%d con prioridad, %d entregados)\n",
			provincias[provinciaDestinoAnt-1], cantProv, prioridadProv, entregadosProv);
			
			
	cantTotal += cantProv;
	prioridadTotal += prioridadProv;
	entregadosTotal += entregadosProv;
	
	provinciaDestinoAnt = paquete.provinciaDestino;
}

void finalizacion(){
	if(cantTotal == 0){
		printf("No hay paquetes cargados.\n");
		
	}else{
		printf("---------------------------------------------\n");
		printf(NEGRITA "TOTAL: %d paquetes (%d con prioridad, %d entregados)\n" RESET, 
				cantTotal, prioridadTotal, entregadosTotal);
		
	}
}

void leerPaquete(){
	if(posActual < cantPaquetes){
		paquete = paquetesCorte[posActual];
		posActual++;
	}else{
		finVector = true;
	}
}
