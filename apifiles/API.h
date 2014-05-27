#ifndef _API_H
#define _API_H

#include <ctype.h>
#include <stdbool.h>
#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Lado.h"
#include "List.h"
#include "Typedef.h"
#include "Vertice.h"

/*	Integrantes:
				Granier, Pierre-Yves	->	flammeus8@gmail.com
				Schmidt, Mariano		->	nanoschmidt22@gmail.com
				Drazile, Eduardo		->	edudraz@gmail.com
				Leberle, Maico C.		->	maico.leberle@gmail.com
*/

struct DovahkiinSt {
	VerticeP fuente, resumidero;
	/*	En flujo se guarda el valor del flujo hasta el momento. */
	u64 flujo;
	/*	data es la lista de vértices, junto con toda su información asociada.
		temp se utiliza para establecer los niveles de BFS en ActualizarDistancias. */
	list_t data, temp;
	/*	iteracion es la manera que tuvimos de evitar tener que actualizar las distancias
		de todos los vértices en cada ActualizarDistancias: sólo se acepta como válida a
		la distancia v->distancia, de un VerticeP v, si v->iteracion es igual al valor
		actual de la variable iteracion que se define aquí. Si v->iteracion es distinto
		(a saber, menor), entonces la distancia BFS de v no está determinada (con lo cual
		implementamos la idea de que v no puede formar parte de un camino aumentante
		de la fuente al resumidero. */
	unsigned int iteracion;
	/*	flujo_maximal == true sii ya se ha hallado un flujo maximal. */
	unsigned int cantidad_busqueda;
	unsigned int cantidad_aumento;
	bool flujo_maximal;
};

/*	Crea y un nuevo Dovahkiin y devuelve un puntero (DovahkiinP) a éste en caso de que
	todo haya salido bien, o NULL en caso contrario. */
DovahkiinP NuevoDovahkiin();

/*	Destruye D y libera su memoria, retornando 1 si fue posible y 0 si no. */
int DestruirDovahkiin(DovahkiinP D);

/*	FijarFuente asegura que se fija el número x como la fuente del network D. */
void FijarFuente(DovahkiinP D, u64 x);

/*	FijarResumidero asegura que se fija el número x como el resumidero del network D. */
void FijarResumidero(DovahkiinP D, u64 x);

/*	Se imprime por standard output 
	"Fuente: x"
	, con x la fuente ya fijada a través de FijarFuente y devuelve 0,
	o devuelve -1 en caso de que la fuente no esté fijada. */
int ImprimirFuente(DovahkiinP D);

/*	Hace lo análogo a ImprimirFuente, pero con el resumidero. */
int ImprimirResumidero(DovahkiinP D);

/*	Lee, desde standard output,
	"x y c"
	, representando un lado, con 'x' un vértice, 'y' otro vértice y 'c' la capacidad del 
	vértice, y devuelve un puntero a struct Lado que representa este nuevo lado, o LadoNulo
	en caso de que la linea no sea válida.
	Una línea no es válida si 'x' o 'y' no son un valor de tipo u64. */
Lado LeerUnLado();

/*	Carga en D un lado que fue creado previamente por LeerUnLado. Su flujo inicial es 0. */
int CargarUnLado(DovahkiinP D, LadoP L);

/*	Simplemente verifica que la fuente y el resumidero estén fijados. */
int Prepararse(DovahkiinP D);

/*	Actualiza la distancia de cada uno de los vértices internos del network, acorde a la distancia
	desde la fuente.
	Devuelve 1 si existe un camino de s a t, y 0 si no. */
int ActualizarDistancias(DovahkiinP D);

/*	Esta función obtiene un camino aumentante desde la fuente al resumidero de D respetando
	los niveles establecidos en ActualizarDistancias, en el sentido de que sólo pueden elegirse
	caminos X1,...,Xn tales que nivel(Xi) = nivel(Xi) + 1, para todo i entre 1 y n-1, X1 es la
	fuente y Xn el resumidero. */
int BusquedaCaminoAumentante(DovahkiinP D);

/*	Aumenta el flujo según corresponda en cado uno de los lados involucrados en el camino aumentante
	conseguido en la última llamada a BusquedaCaminoAumentante. De este modo, "aumenta" puede 
	significar que se devuelve flujo en cierto lado si éste se usar en forma backward en el camino
	aumentante. */
u64 AumentarFlujo(DovahkiinP D);

/*	Hace lo mismo que AumentarFlujo, pero además imprime el camino aumentante en orden inverso; i.e.,
	desde el resumidero a la fuente.
	La fuente será imprimida como 's', el resumidero como 't' y el resto de los vértices como fueron
	nombrados al momento de ejecutar LeerUnLado. */
u64 AumentarFlujoYTambienImprimirCamino(DovahkiinP D);

/*	Imprime el flujo en cada lado, indicando al principio si se trata de un flujo maximal o no. */
void ImprimirFlujo(DovahkiinP D);

/*	Imprime el valor del flujo en el network, indicando si se trata de un flujo maximal o no. */
void ImprimirValorFlujo(DovahkiinP D);

/*	Imprime el corte minimal obtenido en la última llamada a ActualizarDistancias, que debe haber
	devuelto un valor de 0, pues no hubo modo de llegar al resumidero desde la fuente. */
void ImprimirCorte(DovahkiinP D);

#endif
