#ifndef _LADO_H
#define _LADO_H

#include <stdbool.h>
#include "Typedef.h"

struct Lado {
	VerticeP x, y;
    u64 c, f;
};

struct FF_DFSLado {
	Lado lado;
	/*	Si el lado se utiliza de modo forward en BusquedaCaminoAumentante, entonces is_forward == true.
		Si se utiliza de modo backward, is_forward == false. */
	bool is_forward;
};

LadoP crear_lado(VerticeP x, VerticeP y, u64 c);
FF_DFSLadoP crear_FF_DFSLado(Lado lado, bool direccion);
void *destruir_FF_DFSLado(void *ptr);
void *destruir_lado(void *ptr);
bool comparar_lados(void *a, void *b);
#endif
