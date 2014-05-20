#ifndef _VERTICE_H
#define _VERTICE_H

#include "Typedef.h"
#include "List.h"

struct Vertice {
	u64 nombre;
	list_t vecinos_forward;
	list_t  vecinos_backward;
	unsigned int distancia, iteracion;
};

VerticeP crear_vertice(u64 nombre);
void *destruir_vertice(void *ptr);
bool comparar_vertice(void *x, void *y);
list_t add_neighboor_to_list(list_t list, VerticeP v, unsigned int i);
#endif
