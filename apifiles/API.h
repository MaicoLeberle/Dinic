#ifndef _API_H
#define _API_H

#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Lado.h"
#include "List.h"
#include "Typedef.h"
#include "Vertice.h"

struct DovahkiinSt {
	VerticeP fuente, resumidero;
	u64 flujo;
	list_t data, temp;
};

DovahkiinP NuevoDovahkiin();
int DestruirDovahkiin();
LadoP LeerUnLado();
int CargarUnLado(DovahkiinP D, LadoP L);
#endif
