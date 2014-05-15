#ifndef __DINIC_H__
#define __DINIC_H__

#include "API.h"

/* 	Esta estructura contiene la información necesaria para encontrar un camino 
	aumentante en el grafo de los niveles de Dinic, y para enviar flujo cuando
	sea necesario. */
struct Dinic_nodo;
typedef struct Dinic_nodo* Dinic_nodoP;

/* 	Las siguientes dos estructuras básicamente implementan el modo de tener en 
 	una sóla lista enlazada todos los lados sX de nuestro grafo de niveles de 
 	Dinic (s: fuente, X:vertice conectado a s en modo backward. */
struct Elemento_grafo_niveles_Dinic;
typedef struct Elemento_grafo_niveles_Dinic* Elemento_grafo_niveles_DinicP;
struct Grafo_de_niveles_de_dinic;
/*	Los siguientes dos sinónimos muestran que una lista de Dinic_nodos se usa
	para mantener todo el grafo de niveles de Dinic y tambien para mantener
	los subgrafos de niveles de Dinic asociados a cada nodo en el grafo. */
typedef struct Grafo_niveles_Dinic* Grafo_niveles_DINICP;
typedef struct Grafo_de_niveles_de_Dinic* lista_lados_vecinosP;

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


#endif