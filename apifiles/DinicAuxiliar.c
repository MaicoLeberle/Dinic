#include "DinicAuxiliar.h"


/*	
	Definiciones de estructuras.
 */


struct Elemento_grafo_niveles_Dinic {
	Dinic_nodo nodo;
	Dinic_nodoP next;
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
	Lado lado;
	lista_lados_vecinosP lados_vecinos;
	Dinic_nodoP padre_en_iteracion;
};


/*
	Fin definición de estructuras.
*/