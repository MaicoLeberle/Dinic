#ifndef _LADO_H
#define _LADO_H

#include <stdbool.h>
#include "Typedef.h"

struct Lado {
	VerticeP x, y;
    u64 c, f;
};

LadoP crear_lado(VerticeP x, VerticeP y, u64 c);
void *destruir_lado(void *ptr);
bool comparar_lados(void *a, void *b);
#endif
