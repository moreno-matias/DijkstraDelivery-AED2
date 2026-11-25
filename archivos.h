/*TAD de archivos.h para lectura y grabado de paquetes.dat (binario) y para cargar el mapa desde mapa.txt (texto)
  Funciona como "base de datos": se cargan la cola y la pila al iniciar el programa
  y se guardan al finalizarlo. Se agrega en el main DESPUES de entregas.h y cola.h.*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/* Declaración de prototipos de funciones */
void iniciarEscrituraPaquetes();
bool iniciarLecturaPaquetes();
void finalizarArchivoPaquetes();

void grabarPaquete(tPaquete);
void guardarPaquetesEnArchivo(tColaPaquetes *, tPilaEntregas **);
void cargarPaquetesDesdeArchivo(tColaPaquetes *, tPilaEntregas **);
//funciones para cargar el grafo
bool iniciarLecturaMapa();
void finalizarArchivoMapa();
bool cargarMapaDesdeArchivo(tMapa *);
/* Declaración de variables globales */
FILE* paquetesArch; // archivo paquetes
FILE* mapaArch; //archivo de texto del mapa (conexiones entre las provincias y las distancias en km)
/* Implementación de funciones */
void iniciarEscrituraPaquetes() {
	// Abrir el archivo en modo escritura
	paquetesArch = fopen("paquetes.dat", "wb");

	// Si no se pudo grabar, hay que emitir error y terminar
	if(paquetesArch==NULL){
		printf("No se pudo crear el archivo paquetes.dat\n");
		exit(EXIT_FAILURE);
	}
}
bool iniciarLecturaPaquetes() {
	// Abrir el archivo en modo lectura
	paquetesArch = fopen("paquetes.dat", "rb");

	// Devuelve false si no existe (ej: la primera vez que se ejecuta)
	return paquetesArch != NULL;
}

void finalizarArchivoPaquetes() {
	// cerrar el archivo
	fclose(paquetesArch);
}
void grabarPaquete(tPaquete paquete) {
	fwrite(&paquete, sizeof(tPaquete), 1, paquetesArch);
	//separamos esta funcion para poder luego usarla en un bucle y se entienda mejor 
}

/*las siguientes funciones las usaremos en el menu*/

/* Graba todos los paquetes (cola y pila) sin perderlos de las estructuras */
void guardarPaquetesEnArchivo(tColaPaquetes *cola, tPilaEntregas **pila) {
	tColaPaquetes colaAux;
	tPilaEntregas *pilaAux;
	tPaquete paquete;

	iniciarEscrituraPaquetes();
	crearCola(&colaAux);
	crearPila(&pilaAux);

	// 1) cola: grabar y pasar a la auxiliar, luego devolver
	while(!colaVacia(*cola)){
		paquete = desencolar(cola);
		grabarPaquete(paquete);
		encolar(&colaAux, paquete);
	}
	while(!colaVacia(colaAux)){
		paquete = desencolar(&colaAux);
		encolar(cola, paquete);
	}

	// 2) pila: pasar a la auxiliar (queda al reves)
	while(!sinEntregas(*pila)){
		paquete = (*pila)->paquete; //guardo el paquete de la cima
		quitarUltimaEntrega(pila);
		guardarEntrega(&pilaAux, paquete);
	}
	// y al devolver, se graba del final hacia el tope
	while(!sinEntregas(pilaAux)){
		paquete = pilaAux->paquete;
		quitarUltimaEntrega(&pilaAux);
		grabarPaquete(paquete);
		guardarEntrega(pila, paquete);
	}

	finalizarArchivoPaquetes();
}

/* Lee el archivo y reparte cada paquete segun su estado */
void cargarPaquetesDesdeArchivo(tColaPaquetes *cola, tPilaEntregas **pila) {
	tPaquete paquete;

	if(iniciarLecturaPaquetes()){
		//la condicion seria true si se pudo abrir el archivo correctamente para lectura
		// leer el primer paquete
		fread(&paquete, sizeof(tPaquete), 1, paquetesArch);
		// continuar mientras no se haya llegado al final del archivo
		while(!feof(paquetesArch)){
			if(paquete.estado == 3){//el estado 3 es ENTREGADO
				guardarEntrega(pila, paquete); //pila de entregados
			} else {
				encolar(cola, paquete); //cola de restantes
			}

			// leer el siguiente paquete
			fread(&paquete, sizeof(tPaquete), 1, paquetesArch);
		}
		finalizarArchivoPaquetes();
	}
}

/* ---------- Mapa de provincias (grafo.h) ---------- */
bool iniciarLecturaMapa(){
	// abrir el archivo de texto en modo lectura
	bool abiertoCorrectamente;
	mapaArch = fopen("mapa.txt", "r");
	if(mapaArch == NULL){
		abiertoCorrectamente = false;
	}else{
		abiertoCorrectamente = true;
	}
	return abiertoCorrectamente;
}

void finalizarArchivoMapa(){
	// cerrar el archivo
	fclose(mapaArch);
}

//lee mapa.txt y agrega cada camino al grafo con agregarCamino (que carga la ida y la vuelta)
//formato por linea del archivo: provinciaOrigen provinciaDestino km

bool cargarMapaDesdeArchivo(tMapa *mapa){
	tTrayectoria trayecto; //una arista
	int origen, destino, leidos;
	int numCamino = 0;
	float km;
	bool cargado;//para retornar si se pudo leer el archivo
	
	inicializarMapa(mapa);//tengo que inicializar el mapa antes de cargar nada, nos "ahorramos" inicializar en el main
	cargado = iniciarLecturaMapa();//se guardaria true si se pudo abrir correctamente
	
	if(cargado){
		//%*[^\n] salta el resto de la linea (para los nombres y eso)
		leidos = fscanf(mapaArch, "%d %d %f%*[^\n]", &origen, &destino, &km);
		//usamos leidos para poder verificar que se hayan podido leer bien los 3 datos que necesitamos
		while(leidos == 3){
			numCamino++;//para poder mostrar el numero de camino
			if(origen<1 || origen>MAXPROV || destino<1 || destino>MAXPROV || km<=0){
				printf("mapa.txt: el camino numero %d tiene datos invalidos\n", numCamino);
			}else{
				trayecto.origen = origen-1; //le restamos uno porque los indices del grafo van de 0 a 23
				trayecto.destino = destino-1;
				trayecto.distancia = km;
				agregarCamino(mapa, trayecto);
			}
			//leemos el siguiente camino
			leidos = fscanf(mapaArch, "%d %d %f%*[^\n]", &origen, &destino, &km);
			
		}
		//fscanf devuelve EOF cuando termina el archivo con lo cual podemos hacer esta verificacion
		if(leidos != EOF){
			printf("mapa.txt: hay una linea mal escrita o con formato incorrecto despues del camino %d por lo cual no leyo el resto",
					numCamino);
		}
		finalizarArchivoMapa();
		
	}
	return cargado; //retorna true si se pudo arbir o false si no se pudo abrir el archivo
}