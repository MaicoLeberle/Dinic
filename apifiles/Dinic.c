#include "Dinic.h"

/*	Definiciones de las structs definidas en Dinic.h. */

struct Elemento_grafo_niveles_Dinic {
	Dinic_nodo nodo;
	Dinic_nodo* next;
};

struct Dinic_nodo {
	/*  iteracionFF_DFS indica el número de iteración en el que se encuentra 
		este nodo, lo cual indica si se debe enviar flujo cuando se lo pida o 
		no. Es decir, si la última iteración de FF-DFS fue n, entonces sólo se
		debería enviar flujo por los lados Dinic_nodo cuyo iteracionFF_DFS sea
		n. */
	unsigned int iteracionFF_DFS;
	/*	Recordar que el siguiente lado debería tener toda la información 
		necesaria, como su flujo, su capacidad, un indicador de si se trata de 
		un lado forward o backward, un puntero a sus vértices de partida y de
		llegada, y un puntero al lado inverso. */
	LadoP lado;
	lista_lados_vecinosP lados_vecinos;
};

struct Grafo_niveles_Dinic {
	/*	llega_a_t indica si siguen existiendo caminos que llegan al 
		resumidero. */
	bool llega_a_t;
	/* 	iteracionFF_DFS indica la iteración actual en la que se encuentra el 
		algoritmo. */
	unsigned int iteracionFF_DFS;
	Elemento_grafo_niveles_DinicP primer_nodo_de_lista;
};

/*	Definición de las funciones definidas en Dinic.h. */