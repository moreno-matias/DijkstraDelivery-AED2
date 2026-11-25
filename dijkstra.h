/*
TAD Dijkstra
*/

#include <stdio.h>
#include <stdbool.h>
#include <float.h>

#define INFINITO FLT_MAX //infinito
/*Declaracion de tipos de datos personalizados*/
//Estructura para pares de provincias (vectores)
typedef struct{
	tProvincia origen;
	tProvincia destino;
}tPares;
//Vector de pares de provincias.
typedef tPares tVectorPares[MAXPROV];
//Estructura para devolver el camino mas corto en Dijkstra
typedef struct{
	tProvincia secuenciaProvincias[MAXPROV];//para guardar las distancias hasta ese punto
	tVectorPares tramos;//para separar los tramos de paqueteria
	int cantidadProvincias;//para saber cuantas provincias recorremos en el camino mas corto
	float peso;//es el peso en kilometros del camino
	bool existeCamino;//es una bandera para saber rapidamente si se puede llegar
}tCaminoMasCorto;


typedef struct{
	float distanciaMinima[MAXPROV];
	bool visitado[MAXPROV];//funciona para saber directamente si se visito esa provincia-camino
	int predecesor[MAXPROV];//contiene las provincias que son predecesor para poder al final retornar el camino
}tDatosDijkstra;//tipo de dato para poder agrupar los 3 vectores de datos en dijkstra

//Prototipado de funciones
tCaminoMasCorto dijkstra(int, int, tMapa);//funcion principal
//funciones del algoritmo
void inicializarCamino(tCaminoMasCorto *);//por puntero ya que modificaria el contenido original
void inicializarDatosDijkstra(tDatosDijkstra *, int);//por puntero ya que modificaria el contenido original
bool rangoValido(int, int);//para las verificaciones
int buscarNodoMinimo(tDatosDijkstra);
void actualizarDistanciasVecinos(tDatosDijkstra *, int, tMapa);
void reconstruirCamino(tDatosDijkstra, int, tCaminoMasCorto *);//por puntero el camino mas corto ya que lo modifica
void cargarTramos(tCaminoMasCorto *);

void inicializarVectorPares(tVectorPares);
void mostrarCaminoMasCorto(tCaminoMasCorto);

//Implementacion de funciones
tCaminoMasCorto dijkstra(int pOrigen, int pDestino, tMapa pMapa){//recibe el grafo y los dos vertices para calcular su distancia minima
	tCaminoMasCorto resultado;//dato que retornaremos
	tDatosDijkstra datos;
	int nodoActual;
	
	inicializarCamino(&resultado);
	//validacion de rango de las provincias
	if(!rangoValido(pOrigen, pDestino)){
		return resultado;//valor inicializado
	}
	inicializarDatosDijkstra(&datos, pOrigen);
	nodoActual = buscarNodoMinimo(datos);//buscar el nodo no visitado con menor distancia acumulada
	
	
	while(nodoActual != -1){
		datos.visitado[nodoActual] = true;
		if(nodoActual == pDestino){
			break;//para cortar la ejecucion
		}
		actualizarDistanciasVecinos(&datos, nodoActual, pMapa);//revisamos los vecinos del nodoActual para ver si mejoramos sus distancias
		nodoActual = buscarNodoMinimo(datos);//buscar el nodo no visitado con menor distancia acumulada
	}
	
	if(datos.distanciaMinima[pDestino] < INFINITO){
		resultado.existeCamino = true;
		resultado.peso = datos.distanciaMinima[pDestino];
		reconstruirCamino(datos, pDestino, &resultado);// reconstrucción del camino si se logró llegar a pDestino
		cargarTramos(&resultado);//para terminar de cargar cada par teniendo en cuenta la secuencia de provincias que recorrimos
	}
	return resultado;

}

void inicializarVectorPares(tVectorPares pVectorPares){
	int i;
	for (i=0;i<MAXPROV;i++){
		pVectorPares[i].origen=-1;
		pVectorPares[i].destino=-1;
	}
}

void mostrarCaminoMasCorto(tCaminoMasCorto pCamino){
	int i;
	if(!pCamino.existeCamino){
		printf("\nNo existe camino entre las provincias seleccionadas...\n");
	}else{
		printf("\nDistancia minima: %.2f km\n", pCamino.peso);
		printf("\nRecorrido por tramos (pares): ");
		for(i=0; i<pCamino.cantidadProvincias-1; i++){
			printf("\n%s -> %s \t", provincias[pCamino.tramos[i].origen], provincias[pCamino.tramos[i].destino]);	
		}
		printf("\n");
	}
}

void inicializarCamino(tCaminoMasCorto *pCamino){
	int k;
	
	pCamino->cantidadProvincias = 0;
	pCamino->peso = INFINITO;
	pCamino->existeCamino = false;
	inicializarVectorPares(pCamino->tramos);
	for (k = 0; k < MAXPROV; k++) {
		pCamino->secuenciaProvincias[k] = -1;
	}
}

bool rangoValido(int pOrigen, int pDestino){
	return pOrigen>=0 && pOrigen<MAXPROV && pDestino>=0 && pDestino<MAXPROV;
}

void inicializarDatosDijkstra(tDatosDijkstra *pDatos, int pOrigen){
	int i;
	for(i=0; i<MAXPROV ; i++){
		pDatos->distanciaMinima[i] = INFINITO; //al principio del algoritmo debemos inicializar todas las distancias en infinito
		pDatos->visitado[i] = false;
		pDatos->predecesor[i] = -1; //inicializamos los predecesores en -1 para saber el corte del camino
	}
	pDatos->distanciaMinima[pOrigen] = 0.0; //la distancia a si mismo es cero ya que es el origen
}
//devuelve -1 si no quedan nodos alcanzables
int buscarNodoMinimo(tDatosDijkstra pDatos){
	int candidato;
	int minimo = -1;//inicializamos
	for(candidato=0; candidato<MAXPROV; candidato++){
		if(!pDatos.visitado[candidato] && pDatos.distanciaMinima[candidato]<INFINITO && (minimo==-1 || pDatos.distanciaMinima[candidato]<pDatos.distanciaMinima[minimo])){
			minimo = candidato;//actualizo el valor de minimo por el candidato actual
		}
	}
	return minimo;
}

void actualizarDistanciasVecinos(tDatosDijkstra *pDatos, int pNodo, tMapa pMapa){
	int adyacente;
	float pesoArista, nuevaDistancia;
	for(adyacente = 0; adyacente<MAXPROV; adyacente++){
		pesoArista = pMapa.trayectorias[pNodo][adyacente];
		if(!pDatos->visitado[adyacente] && pesoArista>0){
			nuevaDistancia = pDatos->distanciaMinima[pNodo] + pesoArista;
			if(nuevaDistancia < pDatos->distanciaMinima[adyacente]){
				pDatos->distanciaMinima[adyacente] = nuevaDistancia;
				pDatos->predecesor[adyacente] = pNodo;
			}
		}
	}
}

void reconstruirCamino(tDatosDijkstra pDatos, int pDestino, tCaminoMasCorto *pCamino){
	tProvincia invertido[MAXPROV];//arreglo para poder guardar el recorrido que hicimos
	int contador = 0;
	int actual = pDestino;
	int j;
	
	while(actual!=-1){
		//invertido[contador++] = actual; tambien podriamos hacerlo asi
		invertido[contador] = actual;
		contador++;//vamos iterando el indice para recorrer el camino
		actual = pDatos.predecesor[actual];//asi va avanzando por los predecesores		
	}
	pCamino->cantidadProvincias = contador;
	
	//ahora tenemos que dar vuelta y para eso usariamos la formula "contador - 1 - j" con la cual vamos obteniendo los valores contrarios para guardar al reves
	for(j=0; j<contador;j++){
		pCamino->secuenciaProvincias[j] = invertido[contador-1-j];
	}
}

void cargarTramos(tCaminoMasCorto *pCamino){
	int j;
	for(j=0;j<pCamino->cantidadProvincias-1;j++){//cantidadProvincias-1 ya que vamos tomando de a pares para los tramos
		pCamino->tramos[j].origen = pCamino->secuenciaProvincias[j];
		pCamino->tramos[j].destino = pCamino->secuenciaProvincias[j+1];
	}
}


