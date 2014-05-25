#ifndef _VERTICE_H
#define _VERTICE_H

#include "Typedef.h"
#include "List.h"

struct Vertice {
	u64 nombre, temp_flujo;
	list_t vecinos_forward, vecinos_backward;
	unsigned int distancia, iteracion;
	bool is_forward;
	list_t vecinos_posibles;
	VerticeP ancestro;
};

VerticeP crear_vertice(u64 nombre);
void *destruir_vertice(void *ptr);
bool comparar_vertice(void *x, void *y);
list_t add_neighboor_to_list(list_t list, VerticeP v,VerticeP resumidero, unsigned int i);
#endif
