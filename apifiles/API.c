#include "API.h"
#include "Dinic.h"

/* 	
	Definición de estructuras.
*/


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


/*
	Fin definición de estructuras.
*/


/*	
	Definición de funciones.
*/


/*	Crear un nuevo Dovahkiin y regresar el puntero a éste, o null en caso de 
	error. */
DovahkiinP NuevoDovahkiin() {
	DovahkiinP nuevo_Dovahkiin;
	nuevo_Dovahkiin.primer_vertice = NULL;
	nuevo_Dovahkiin.s = NULL;
	nuevo_Dovahkiin.t = NULL;
	nuevo_Dovahkiin.grafo_dinic = NULL;
	return nuevo_Dovahkiin;
}


/*	Destruir el Dovahkiin *D, liberando la memoria asociada a D. */
int DestruirDovahkiin(DovahkiinP D);


/*	Establecer x como la fuente de *D. */
void FijarFuente(DovahkiinP D, u64 x);


/* Establece x como el resumidero de *D. */
void FijarResumidero(DovahkiinP D, u64 x);


/* 	Imprime en pantalla (si es que la fuente está establecida):
	Fuente: x 
	, con x la fuente de D*, y devuelve 0.

	En caso de que la fuente no esté establecida, no imprime nada y devuelve 
	-1. */
int ImprimirFuente(DovahkiinP D);


/* 	Imprime en pantalla (si es que el resumidero está establecido):
	Resumidero: x
	, con x el resumidero de *D, y devuelve 0.

	En caso de que el resumidero no esté establecido, no imprime nada y 
	devuelve -1. */
int ImprimirResumidero(DovahkiinP D);


/* 	Lee una línea desde standard input y devuelve el Lado que representan 
	estos datos, si el input fue válida, o LadoNulo, si el input no fue 
	válido. 
	Una línea válida de input es de la forma:
	x y c
	, donde esto representa el lado xy, con C(xy) = c. Tener en cuenta que al
	imprimir los vértices x e y cuando se lo requeriese, se deberán usar estos
	valores usados en el input. */
Lado LeerUnLado()


/* 	Añade el lado L al network *D. El flujo en dicho lado se inicializa como
	f(L) = 0. 
	Devuelve 1 si se pudo realizar esta operación, y 0 si no se pudo. */
int CargarUnLado(DovahkiinP D, Lado L);


/* 	Está función será llamada una vez finalizada la etapa de construcción del
	network, y antes de comenzar a calcular el flujo sobre éste.
	Devuelve 1 si pudo concretar todas las operaciones, y 0 en caso 
	contrario. */
int Prepararse();


/*	Actualiza las etiquetas de distancias de todos los vértices, corriendo 
	BFS sobre caminos aumentantes según procede DIinic
	Devuelve 1 si existe un camino aumentante restante entre s y t, y 0 en caso
	contrario. */
int ActualizarDistancias();


/*	Busca un camino aumentante al estilo FordFulkerson-DFS usando las etiquetas
	de distancia establecidas y actualizadas por ActualizarDistancia.
	Devuelve 1 si el camino llega a t, y 0 si no.
	Esta función no aumenta el flujo, sólo encuentra un camino aumentante de s
	a t como ya se explicó. */
int BusquedaCaminoAumentante(DovahkiinP D);


/*	Actualiza el flujo en *D, utilizando un camino aumentante encontrado en 
	BusquedaCaminoAumentante para el cual todavía no se ha actualizado el 
	flujo. 
	Devuelve el aumento de flujo, en caso de que se terminen todas las 
	operaciones debidamente.
	En caso de que no haya un camino de tales características, o de
	que haya ocurrido algún tipo de error, devuelve 0. */
u64 AumentarFlujo(DovahkiinP D);


/*	Realiza y devuelve lo mismo que AumentarFlujo, con la diferencia de que
	también imprime en standard output información relativa al aumento en el 
	flujo. Se debe imprimir:
	camino aumentate #:
	t;x_r(; | >)...(; | >)x_1s: e
	, con # número del camino aumentante que se está procesando, x_1, x_r la
	sucesión de vértices de s a t, no inclusives, del camino encontrado en 
	BusquedaCaminoAumentante. Además, la decisión entre ; y > corresponde a si
	cierto lado es fordward o backward, respectivamente. */
u64 AumentarFlujoYtambienImprimirCamino();


/* 	Imprime el flujo que se tiene en ese momento. El formato debe ser:
	Flujo ("Maximal" | "no maximal"):
	Lado x_1,y_1:	f_1
	.
	.
	.
	Lado x_m, y_m:	f_m
	, con x_1, y_1 y f_1 correspondiendo al primer lado y su flujo, etc. */
void ImprimirFlujo();


/* 	Imprime el valor del flujo. El formato debe ser: 
	Valor del flujo ("Maximal" | "no maximal"):   v
	, con v el valor del flujo actual. */
void ImprimirValorFlujo();


/* 	Imprime un corte minimal, del network *D, y su capacidad. El formato debe 
	ser:
	Corte Minimal:	S = {s,x_1,...}
	Capacidad:	c
	, con s,x_1,... los vértices del corte (en cualquier orden) y c la 
	capacidad del corte. */
void ImprimirCorte(DovahkiinP D);


/*
	Fin definción de funciones.
*/