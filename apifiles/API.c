#include "API.h"

/* 	Definición de las structs definidas en API.h, más algunas adicionales 
	necesarias. */

typedef struct Nodo_lado* Nodo_ladoP;
typedef struct Lista_lados* Lista_ladosP;
typedef struct Vertice* VerticeP;
typedef struct Nodo_vertice* Nodo_verticeP;

struct Lado {
	VerticeP vertice_de_salida;
	VerticeP vertice_de_llegada;
	/* direccion == true sii el lado es forward. */
	bool direccion;
	/* 	lado_inverso es el correspondiente backward, si el lado es forward, y 
		forward, si el lado es backward. */
	LadoP lado_inverso;
};

struct Nodo_lado{
	LadoP lado;
	Nodo_ladoP siguiente;
};

struct Lista_lados {
	Nodo_ladoP primer_lado;
};

struct Vertice {
	u64 nombre;
	Lista_ladosP lados_forward;
	Lista_ladosP lados_backward;
};

struct Nodo_vertice {
	VerticeP vertice;
	Nodo_verticeP siguiente;
};

struct NetworkYFlujo {
	Nodo_verticeP primer_vertice;
	VerticeP s;
	VerticeP t;
	GrafoNivelesDinicP grafo_dinic;
};
