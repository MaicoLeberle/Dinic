#ifndef _LADO_H
#define _LADO_H

#include "Typedef.h"

struct Lado {
	VerticeP x;
	VerticeP y;
    u64 c;
    u64 f;
};

LadoP crear_lado(VerticeP x, VerticeP y, u64 c);
void *destruir_lado(void *lado);
#endif
