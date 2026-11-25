#ifndef MAPA_H
#define MAPA_H
#include <stdbool.h>
#include "raylib.h" //Importa las funciones y los tipos de datos (como texture2D) necesarias para graficar 
#include <stdlib.h>
#include <stdio.h>
#define INFO 320//Tamaño del panel
typedef char tString[40]; 
typedef struct{
	Texture2D fondo;//Struct que guarda el tamño del mapa(alto, ancho y un id por grafico)
	float escala;//Escala de la imagen en la pantalla 
	float ancho;
	int origen;
	int destino;
	tCaminoMasCorto ruta;//Struct del TAD de dijkstra
}tGrafico;
//Declaracion de funciones
tGrafico* crearMapa(tString);//Crea el mapa escalandolo segun el tamaño del monitor
bool mapaAbierto(tGrafico*);////Devuelve true mientras la ventana siga abierta
void dibujarCamino(tGrafico*,tMapa*);//Dibuja el mapa, aristas, vertices y el panel, también lee el mouse
void borrarMapa(tGrafico*);//Libera la memoria del gpu, cierra la ventana del mapa y libera la struct
Vector2 coordImagen(tGrafico*, Vector2);//Elige un punto de la imagen a la ventana que se multiplica por la escala
Vector2 coordVentana(tGrafico*,Vector2);//Elige un punto de la ventana a  la imagen que se divide por la escala
void dibujarAristas(tGrafico*,tMapa*,float,Color);//Dibuja las aristas de cada provincia con sus adyacentes
int provinciaSeleccionada(tGrafico*);//Retorna la provincia  mas cerca al mouse en un radio de 12px o-1 sino hay ninguna
void actualizarSeleccion(tGrafico*,tMapa*);//Actualiza el origen o destino segun si ya se hizo click, click derecho deshace la seleccion e inicializa en -1 origen y destino
bool caminoProvincias(tGrafico*,int);//Devuelve true si la provincia esta está  en el camino mas corto calculado
void dibujarDijkstra(tGrafico*);//Dibuja el camino mas corto con dijkstra
void nomProvincias(Vector2,int);//Muestra el nombre de las provincias en el mapa
void panelInformacion(tGrafico*,tMapa*);//Panel que nos muestra que provinicas usamos en dijkstra y su recorrido con peso
//Implementacion
typedef struct{
	tString nomPro;
	Vector2 coords;
}tNodoProv;
const tNodoProv PROVINCIAS[MAXPROV]={{"Buenos Aires",{420,419}},{"CABA",{403,395}},{"Catamarca",{213,210}},{"Chaco",{387,176}},{"Chubut",{215,662}},{"Cordoba",{258,305}},
{"Corrientes",{397,177}},{"Entre Rios",{359,328}},{"Formosa",{411,131}},{"Jujuy",{223,76}},{"La Pampa",{256,452}},{"La Rioja",{186,242}}, {"Mendoza",{126,346}},{"Misiones",{471,175}},
{"Neuquen",{131,526}},{"Rio Negro",{271,589}},{"Salta",{229,100}},{"San Juan",{136,300}},{"San Luis",{194,349}},{"Santa Cruz",{98,917}},{"Santa Fe",{341,303}},{"Stg.Estero",{251,185}},
{"Tierra del Fuego",{139,1011}},{"Tucuman",{223,160}}};

tGrafico* crearMapa(tString mapaArgentina){
	tGrafico* mapa=(tGrafico*)malloc(sizeof(tGrafico));
	if(mapa==NULL) return NULL;//
	SetTraceLogLevel(LOG_NONE);//apaga todos los mensajes de raylib en la consola
	
	InitWindow(800,600,"Mapa de Argentina");
	SetTargetFPS(60);//Limita el programa 60 veces por segundo evitando colapsar el cpu
	mapa->fondo=LoadTexture(mapaArgentina);//Carga la imagen(mapa)
	if(mapa->fondo.id==0){
		printf("No se pudo cargar %s\n",mapaArgentina);
		CloseWindow();//Si no se cargo el mapa cierra la ventana donde se muestra
		free(mapa);
		return NULL;
	}
	int altMonitor= GetMonitorHeight(GetCurrentMonitor());//Funcion de raylib que obtiene la altura del monitor
	int altVentana= altMonitor-120;
	if (altVentana>1000){
		altVentana=1000;
	}
	if(altVentana<400){
		altVentana=400;
	}
	mapa->escala=(float) altVentana/mapa->fondo.height;
	mapa->ancho=mapa->fondo.width*mapa->escala;
	SetWindowSize((int)mapa->ancho+INFO,altVentana);//Tamaño de la ventana segun lo calculado
	SetWindowPosition(100,30);//Posicion donde se abre la ventana
	mapa->origen=-1;
	mapa->destino=-1;
	inicializarCamino(&mapa->ruta);//Funcion del TAD de dijkstra
	return mapa;
}
bool mapaAbierto(tGrafico* pMapa){
	return (pMapa!=NULL) && !WindowShouldClose();//WindowShouldClose da true al cerrar, negamos y devolvemos true mientras siga abierta
}
void dibujarAristas(tGrafico* pMapa,tMapa* pGrafo,float pGrosor,Color pColor){
	int i,j;
	for(i=0;i<MAXPROV;i++){
		for(j=i+1;j<MAXPROV;j++){//j+1 para evitar que se duplique la arista al ser un grafo no dirigido
			if(pGrafo->trayectorias[i][j]>0){
			Vector2 provInicio=coordImagen(pMapa,PROVINCIAS[i].coords);
			Vector2 provDestino=coordImagen(pMapa,PROVINCIAS[j].coords);
			DrawLineEx(provInicio,provDestino,pGrosor,pColor);//Dibuja la arista entre los vertices
			}
		}
	}
}
int provinciaSeleccionada(tGrafico* pMapa){
	int i;
	int provCercana=-1;//Va a ser la provincia que se selecciona, la inicializamos en -1 porque el vector de provincias va de 0 a 23
	float radioSeleccion=12.0f*12.0f;//Rango para seleccionar una provincia
	Vector2 mouse=GetMousePosition();//Obtiene la posicion del mouse (funcion de raylib)
	for(i=0;i<MAXPROV;i++){
		Vector2 provincia=coordImagen(pMapa,PROVINCIAS[i].coords);//Coordenadas de cada provincia
		float posX=mouse.x-provincia.x;//Calcula la posicion en el eje x de cada provincia
		float posY=mouse.y-provincia.y;//Calcula la posicion en el eje y de cada provincia
		float posibleSeleccion=posX*posX+posY*posY;
		if(posibleSeleccion<radioSeleccion){//Para evaluar si esta en el rango de una provincia
			radioSeleccion=posibleSeleccion;
			provCercana=i;//Guarda la provincia seleccionada
		}
	}
	return provCercana;
}
void actualizarSeleccion(tGrafico* pMapa,tMapa* pGrafo){
	int prov;
	if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)){//Evalua si se hizo click derecho
	pMapa->origen=-1;
	pMapa->destino=-1;
	inicializarCamino(&pMapa->ruta);
	return;	//Si se selecciono una provincia o ambas se deshace
	}
	if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){//Evalua si se hizo click izquierdo
		prov=provinciaSeleccionada(pMapa);//Guarda la provincia que se selecciono
		if(prov!=-1){//Evalua si se selecciono una provincia 
			if(pMapa->origen==-1 || pMapa->destino!=-1){//Si no se eligió origen o hay un camino, empieza otra seleccion
				pMapa->origen=prov;
				pMapa->destino=-1;
				inicializarCamino(&pMapa->ruta);
			}else if(prov!=pMapa->origen){//Ya se eligió el origen entonces guarda la provinicas seleccionada en el destino
				pMapa->destino=prov;
				pMapa->ruta=dijkstra(pMapa->origen,pMapa->destino,*pGrafo);//Calcula Dijkstra con las provincias seleccionadas
			}
		}
	}	
}
bool caminoProvincias(tGrafico* pMapa,int pProv){//retorna true si la provincia está en la  secuencia de camino mas corto
	if(!pMapa->ruta.existeCamino){
		return false;
	}
	int k;
	for(k=0;k<pMapa->ruta.cantidadProvincias;k++){
		if(pMapa->ruta.secuenciaProvincias[k]==pProv){
			return true;
		}
	}
	return false;
}
void dibujarDijkstra(tGrafico* pMapa){//Dibuja en amarillo el camino de mas corto
	if(!pMapa->ruta.existeCamino){
		return;
	}
	int k;
	for(k=0;k<pMapa->ruta.cantidadProvincias-1;k++){//Hay n-1 tramos en n provincias 
		Vector2 inicio=coordImagen(pMapa,PROVINCIAS[pMapa->ruta.tramos[k].origen].coords);
		Vector2 destino=coordImagen(pMapa,PROVINCIAS[pMapa->ruta.tramos[k].destino].coords);
		DrawLineEx(inicio,destino,4.0f,YELLOW);//Linea recta hasta la otro provincia
	}
}
void nomProvincias(Vector2 pProv, int pPos){ //Dibuja el nombre de cada provincia 
	if(strcmp(PROVINCIAS[pPos].nomPro, "Chaco") == 0){
		DrawText(PROVINCIAS[pPos].nomPro, (int)pProv.x-38, (int)pProv.y-8, 8, BLACK);
	}else if(strcmp(PROVINCIAS[pPos].nomPro, "Corrientes") == 0){
		DrawText(PROVINCIAS[pPos].nomPro, (int)pProv.x-6, (int)pProv.y+2, 8, BLACK);
	}else{
		DrawText(PROVINCIAS[pPos].nomPro, (int)pProv.x-8, (int)pProv.y-8, 8, BLACK);
	}
}
void panelInformacion(tGrafico* pMapa,tMapa* pGrafo){//Rectangulo a la derecha del mapa, escribe la informacion de dijkstra
	int k;
	int x=(int)pMapa->ancho +15;
	DrawLine((int)pMapa->ancho,0,(int)pMapa->ancho,GetScreenHeight(),LIGHTGRAY);
	DrawText("CAMINO MAS CORTO",x,20,20,BLACK);
	if(pMapa->origen==-1){
		DrawText("Elegir Origen:",x,60,14,DARKGRAY);
	}else{
		DrawText(TextFormat("Provincia Origen: %s",PROVINCIAS[pMapa->origen].nomPro),x,60,16,DARKGRAY);//Se dibuja una vez se hizo click en una provincia
	}if(pMapa->destino==-1){
		DrawText("Elegir Destino:",x,80,14,DARKGRAY);
	}else{
		DrawText(TextFormat("Provincia Destino:%s",PROVINCIAS[pMapa->destino].nomPro),x,80,16,DARKGRAY);//Se dibuja una vez que se hizo click en otra provincia
		if(!pMapa->ruta.existeCamino){
			DrawText("No existe camino entre ambas provincias",x,115,16,RED);
		}else{
			DrawText(TextFormat("Distancia minima: %.0fkm",pMapa->ruta.peso),x,115,16,BLUE);//Se dibuja una vez se haya seleccionado dos provincias
			DrawText("Recorrido",x,145,16,DARKGRAY);
			for(k=0;k<pMapa->ruta.cantidadProvincias;k++){
				int prov=pMapa->ruta.secuenciaProvincias[k];
				if(k==0){
					DrawText(TextFormat("%d. %s",k+1,PROVINCIAS[prov].nomPro),x,170+k*18,14,BLACK);//Dibuja que provincia se recorrio
				}else{
					int provAnt=pMapa->ruta.secuenciaProvincias[k-1];
					DrawText(TextFormat("%d. %s: (+%.0f km)",k+1,PROVINCIAS[prov].nomPro,pGrafo->trayectorias[provAnt][prov]),x,170+k*18,14,BLACK);//Dibuja los  km de la provincia anterior a la actual
				}
			}
		}
	}
}

void dibujarCamino(tGrafico* pMapa,tMapa* pGrafo){
	int i,provincia;
	actualizarSeleccion(pMapa,pGrafo);
	provincia=provinciaSeleccionada(pMapa);
	SetMouseCursor(provincia != -1 ? MOUSE_CURSOR_POINTING_HAND : MOUSE_CURSOR_DEFAULT);//Para cambiar la flecha del mouse a una manita :D
	BeginDrawing();//Todo lo que esté ente esta funcion y endDrawing es lo que se dibujará
	ClearBackground(RAYWHITE);//Coloca fondo en todo el cuadro
	Vector2 esquina={0,0};
	DrawTextureEx(pMapa->fondo,esquina,0.0f,pMapa->escala,WHITE);//Dibuja el mapa ubicandose en la esquina superior izquiera (0,0)
	if(pMapa->ruta.existeCamino){
		dibujarAristas(pMapa,pGrafo,1.0f,Fade(BLACK,0.25f));
	}else{
		dibujarAristas(pMapa,pGrafo,1.5f,Fade(BLACK,0.8f));//Dibuja las aristas ya cargadas del grafo
	}
	dibujarDijkstra(pMapa);
	for(i=0;i<MAXPROV;i++){
	Vector2 nodo=coordImagen(pMapa,PROVINCIAS[i].coords);
	Color color=DARKBLUE;	
	float radio=3;
	if(caminoProvincias(pMapa,i)){//Pone en rojo las provincias que están en el camino de dijkstra
		color=RED;
		radio=5;
	}
	if(i==pMapa->origen){//Pone de color verde el origen de dijkstra
		color=GREEN;
		radio=6;		
	}
	if(i==pMapa->destino){//Pone de color morado el destino de dijkstra
		color=PURPLE;
		radio=6;				
	}
	if(i==provincia){//Crece 2px al pasar el mouse por encima de esa provincia
		radio+=2;
	}
	DrawCircleV(nodo,radio,color);//Dibuja el circulo en cada capital
	nomProvincias(nodo,i);
	}
	panelInformacion(pMapa,pGrafo);
	EndDrawing();//Muestra el cuadro terminado
}
void borrarMapa(tGrafico* pMapa){
	if(pMapa!=NULL){
		UnloadTexture(pMapa->fondo);//Libera la imagen de la memoria de la gpu
		CloseWindow();//Cierra la ventana del mapa 
		free(pMapa);
	}
}
Vector2 coordImagen(tGrafico* pMapa,Vector2 pCoord){
	Vector2 img={pCoord.x*pMapa->escala,pCoord.y*pMapa->escala};
	return img;
}
Vector2 coordVentana(tGrafico* pMapa,Vector2 pCoord){
	Vector2 img2={pCoord.x/pMapa->escala,pCoord.y/pMapa->escala};
	return img2;
}
#endif //MAPA_H
