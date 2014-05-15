#include "Dinic.h"
#include "DinicAuxiliar.h"

/*	
	Definiciones de estructuras.
 */

struct Grafo_niveles_Dinic {
	/*	llega_a_t indica si siguen existiendo caminos que llegan al 
		resumidero. */
	bool llega_a_t;
	/* 	iteracionFF_DFS indica la iteración actual en la que se encuentra el 
		algoritmo. */
	unsigned int iteracionFF_DFS;
	u64 flujo_a_enviar;
	Elemento_grafo_niveles_DinicP primer_nodo_de_lista;
};


/*
	Fin definición de estructuras.
*/


/*	
	Definición de funciones.
*/


/*	Esta función será llamada para actualizar las distancias del grafo 
	original. */
Grafo_niveles_DINICP CrearGrafoDistancias(DovahkiinP D);

/*	Claramente, esta función debe copiar todos los resultados de encontrar 
	caminos aumentantes a lo largo del grafo auxiliar creado en 
	CrearGrafoDistancias, y de enviar flujo por estos caminos, al network 
	original. */
int EstablecerCaminoAumentante(DovahkiinP D, Grafo_niveles_DINICP grafo);

/*	No es necesario pasar un DovahkiinP como parámetro en esta función, porque
	se tiene como precondición que grafo ya ha encontrado un camino aumentante,
	y sólo debe trabajar sobre grafo. Recordar que el paso de copiar los 
	resultados de las corridas de FF-DFS a algún network se hace al intentar
	buscar un camino aumentante en EstablecerCaminoAumentante. */
u64 AumentarFlujo(Grafo_niveles_DINICP grafo);

void ImprimirCorte(Grafo_niveles_DINICP grafo);

/*	Si no se llega al resumidero, es necesario eliminar el Grafo_niveles_Dinic 
	asociado a grafo y crearle uno nuevo. Esta función debe ser usada 
	internamente en EstablecerCaminoAumentante. */
bool SeLlegaResumidero(Grafo_niveles_DINICP grafo);


/*
	Fin definición de funciones.
*/	