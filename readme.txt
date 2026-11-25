==============================================================
 SISTEMA DE PAQUETERIA - GUIA DE USO
 Proyecto Integrador - Algoritmo y Estructura de Datos 2
 Licenciatura en Sistemas de Informacion - UNNE
==============================================================

Integrantes:    
	Moreno Gonzalez Matias Benjamin
	Palacio Enrique Alfredo
	Penayo Enrique Ariel
	Woznirck Lautaro Ezequiel

1. ARCHIVOS NECESARIOS
--------------------------------------------------------------
Todos estos archivos deben estar en la MISMA carpeta:

	main.c        (unico archivo que se compila)
	paquete.h
	cola.h
	entregas.h
	grafo.h
	dijkstra.h
	archivos.h
	reportes.h
	paquetes.dat          (datos de los paquetes)
	mapa.txt       (distancias en km entre provincias)
	mapa.h
	raylib.h
	librarylib.a
	mapaArgentina.png


2. USO DEL PROGRAMA
--------------------------------------------------------------
Solo se compila main.c. Los archivos .h NO se compilan por
separado: main.c los incluye.

=== COMO EJECUTAR ===

Opcion 1 (recomendada): abrir main.exe con doble clic (Windows).
Debe ejecutarse desde esta misma carpeta.

=== COMO COMPILAR EN DEV-C++ ===

El programa usa la libreria grafica raylib (incluida en esta carpeta:
raylib.h y libraylib.a). Antes de compilar hay que configurar el linker:

1. Abrir Dev-C++ y el archivo main.c.
2. Ir a Herramientas > Opciones del compilador.
3. Tildar "Añadir los siguientes comandos cuando se llama al compilador"
   y escribir:
      -lraylib -lopengl32 -lgdi32 -lwinmm
4. Ir a la pestana "Directorios":
   - En "Bibliotecas", agregar la ruta de esta carpeta (donde esta libraylib.a).
   - En "C Includes", agregar la ruta de esta carpeta (donde esta raylib.h).
5. Aceptar y compilar (F11).

Si no se configura el linker, aparecen errores lógicamente.


3. EJECUCION
--------------------------------------------------------------
Al iniciar, el programa carga el mapa de provincias y los
paquetes guardados en paquetes.dat, y muestra el menu principal.

IMPORTANTE: salir siempre con la opcion 0. Si se cierra la
ventana sin salir por el menu, los cambios de esa sesion no se
guardan.

4. USO DEL MENU
--------------------------------------------------------------
Se elige cada opcion escribiendo su numero y presionando Enter.
Si el numero esta fuera de rango, el programa lo vuelve a pedir.

MENU PRINCIPAL

	1. Cargar nuevo paquete
		Pide los datos del paquete. El ID se asigna solo.
	2. Ver paquetes restantes
		Lista los paquetes pendientes en orden de llegada.
	3. Ver restantes con prioridad
		Lista solo los pendientes prioritarios.
	4. Entregar siguiente paquete
		Entrega el primer paquete pendiente y muestra la ruta
		mas corta entre su origen y su destino.
	5. Ver paquetes entregados
		Lista las entregas, de la mas reciente a la mas antigua.
	6. Ver ultima entrega
		Muestra la entrega mas reciente.
	7. Mapa de provincias (submenu)
		1. Ver conexiones entre provincias
		2. Ver distancias en km
		3. Ver mapa interactivo
		0. Volver
	8. Reportes (submenu)
		1. Resumen general
		2. Reporte de entregados
		3. Paquetes por provincia de destino
		4. Buscar paquete por ID
		0. Volver
	9. Guardar datos
		Guarda los paquetes en paquetes.dat sin salir.
	0. Salir
		Guarda los datos y cierra el programa.

Para el uso del mapa interactivo: Click izquierdo para seleccionar provincia, 
click derecho para reiniciar.

5. DATOS UTILES
--------------------------------------------------------------
Estados de un paquete:
	1 = En sucursal
	2 = En camino
	3 = Entregado

Provincias (se ingresan con su numero, de 1 a 24):
	1 = Buenos Aires.
	2 = CABA.
	3 = Catamarca.
	4 = Chaco.
	5 = Chubut.
	6 = Cordoba.
	7 = Corrientes.
	8 = Entre Rios.
	9 = Formosa.
	10 = Jujuy.
	11 = La Pampa.
	12 = La Rioja.
	13 = Mendoza.
	14 = Misiones.
	15 = Neuquen.
	16 = Rio Negro.
	17 = Salta.
	18 = San Juan.
	19 = San Luis.
	20 = Santa Cruz.
	21 = Santa Fe.
	22 = Santiago del Estero.
	23 = Tierra del Fuego.
	24 = Tucuman.

Prioridad: 1 = prioritario, 0 = normal. Solo sirve para filtrar
la vista de pendientes (opcion 3); los paquetes siempre se
entregan en orden de llegada.


6. EJEMPLO RAPIDO
	El grupo ya cargó un archivo de pruebas con aproximadamente 
	300 paquetes, si no lo desean usar pueden eliminar el 
	archivo (paquetes.dat) y el programa generaría uno nuevo.

--------------------------------------------------------------
	1. Ejecutar el programa.
	2. Opcion 1: cargar un par de paquetes.
	3. Opcion 2: ver la lista de pendientes.
	4. Opcion 4: entregar un paquete y ver la ruta.
	5. Opcion 5: ver las entregas realizadas.
	6. Opcion 0: salir (se guardan los datos).
	7. Volver a ejecutar: los paquetes siguen ahi.