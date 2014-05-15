#ifndef __DINIC_AUXILIAR__
#define __DINIC_AUXILIAR__

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

Dinic_nodoP NuevoDinic_nodoP();

int 




#endif