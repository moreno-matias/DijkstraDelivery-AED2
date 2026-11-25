#include<stdio.h>
#include<ctype.h>
#include<stdbool.h>
#define LINEA "--------------------------------------------------------------"

#define RESET "\033[0m"
#define ROJO "\033[31m"
#define VERDE "\033[32m"
#define AMARILLO "\033[33m"
#define AZUL "\033[34m"
#define NEGRITA "\033[1m"
//declaracion de tipos de datos personalizados
typedef char string[40];


//declaracion del registro para los datos del paquete
typedef struct{
	int IDPaquete;
	int IDCliente;
	int provinciaOrigen; //al trabajar con provincias, incluimos una de origen y otra de destino
	int provinciaDestino;
	bool prioridad;
	int estado; //1: en sucursal, 2: en camino, 3: entregado
}tPaquete;



//declaracion de prototipos
void crearPaquete(tPaquete*);
tPaquete cargarPaquete(); //retorna un struct, que sera enviado por parametros en otra funcion
void mostrarPaquete(tPaquete);

//variables globales
//vector con los nombres de cada provincia
string provincias[24] = {
    "Buenos Aires", "CABA", "Catamarca", "Chaco", "Chubut", "Cordoba", "Corrientes",
    "Entre Rios", "Formosa", "Jujuy", "La Pampa", "La Rioja", "Mendoza", "Misiones",
    "Neuquen", "Rio Negro", "Salta", "San Juan", "San Luis", "Santa Cruz", "Santa Fe",
    "Santiago del Estero", "Tierra del Fuego", "Tucuman"
};

//definicion de funciones
void crearPaquete(tPaquete *pPaquete){
	
	pPaquete->IDPaquete = 0;
	pPaquete->IDCliente = 0;
	pPaquete->provinciaOrigen = 0;
	pPaquete->provinciaDestino = 0;
	pPaquete->prioridad = false;
	pPaquete->estado = 1; //Estados: 1-En sucursal, 2-En camino, 3-Entregado
	
}


tPaquete cargarPaquete(){
	
	tPaquete nuevoPaquete;
	char opcion;

	crearPaquete(&nuevoPaquete); //inicializa el nuevo paquete declarado de forma local
	
	printf("Ingrese el ID del cliente:\n");
	scanf(" %d", &nuevoPaquete.IDCliente);

	do{
	printf("Ingrese la provincia de origen del envio (1 a 24).\n");
	//muestra las provincias disponibles y su numero asociado
	printf("Lista de provincias: \n1-Buenos Aires\t2-CABA\t3-Catamarca\t4-Chaco\n5-Chubut\t6-Cordoba\t7-Corrientes\t8-Entre Rios\n9-Formosa\t10-Jujuy\t11-La Pampa\t12-La Rioja\n13-Mendoza\t14-Misiones\t15-Neuquen\t16-Rio Negro\n17-Salta\t18-San Juan\t19-San Luis\t20-Santa Cruz\n21-Santa Fe\t22-Santiago del Estero\t23-Tierra del Fuego\t24-Tucuman\n");
	scanf(" %d", &nuevoPaquete.provinciaOrigen);

    if(nuevoPaquete.provinciaOrigen < 1 || nuevoPaquete.provinciaOrigen > 24){
    	printf(ROJO "Error, ingrese una provincia valida.\n" RESET); //la provincia no esta en rango de 1 a 24
    }

	}while(nuevoPaquete.provinciaOrigen < 1 || nuevoPaquete.provinciaOrigen > 24);

	do{
	printf("Ingrese la provincia de destino del envio (1 a 24).\n");
	//printf("Lista de provincias: \n1-Buenos Aires\t2-CABA\t3-Catamarca\t4-Chaco\n5-Chubut\t6-Cordoba\t7-Corrientes\t8-Entre Rios\n9-Formosa\t10-Jujuy\t11-La Pampa\t12-La Rioja\n13-Mendoza\t14-Misiones\t15-Neuquen\t16-Rio Negro\n17-Salta\t18-San Juan\t19-San Luis\t20-Santa Cruz\n21-Santa Fe\t22-Santiago del Estero\t23-Tierra del Fuego, Antartida e Islas del Atlantico Sur\t24-Tucuman\n");
	scanf(" %d", &nuevoPaquete.provinciaDestino);

    if(nuevoPaquete.provinciaDestino < 1 || nuevoPaquete.provinciaDestino > 24){
    	printf(ROJO "Error, ingrese una provincia valida.\n" RESET);
    }

	}while(nuevoPaquete.provinciaDestino < 1 || nuevoPaquete.provinciaDestino > 24);
	

	do{
		printf("Es un envio con prioridad?(S / N)\n");
    scanf(" %c", &opcion);
    opcion = toupper(opcion);//convierte a mayuscula el caracter ingresado

	if(opcion != 'N' && opcion != 'S'){
		printf(ROJO "Opcion incorrecta, elija nuevamente\n" RESET); //el caracter no es una de las dos opciones
	}
	}while(opcion != 'N' && opcion != 'S');

	
    if(opcion == 'S'){
    	nuevoPaquete.prioridad = true;
    }else{
		nuevoPaquete.prioridad = false;
	}
    
    return nuevoPaquete;
}

void mostrarPaquete(tPaquete pPaquete){

	//vector que incluye los estados del envio
	string estadoEnvio[3] = {"En sucursal", "En camino", "Entregado"};
	
	printf("%s", LINEA);
	
	printf(NEGRITA "\n---Datos del paquete---\n" RESET);
	printf("ID del paquete: %d\n", pPaquete.IDPaquete);
	printf("ID del cliente: %d\n", pPaquete.IDCliente);
	printf("Provincia de origen: %s\n", provincias[pPaquete.provinciaOrigen - 1]); //muestra el nombre de la provincia correspondiente ubicada en el vector
	printf("Provincia de destino: %s\n", provincias[pPaquete.provinciaDestino - 1]);
	printf("Entrega %s prioridad.\n", pPaquete.prioridad ?  "con" : "sin"); //if implementado de otra manera, imprime "sin" si es falso y "con" si es verdadero
	printf("Estado del envio: %s\n", estadoEnvio[pPaquete.estado - 1]);
	printf("%s \n", LINEA);
}

		
