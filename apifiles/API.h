#ifndef _API_H
#define _API_H

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <queue.h>

typedef uint64_t u64;
typedef struct DovahkiinSt *DovahkiinP;
typedef struct _vertice_t *vertice_t;
typedef struct Lado *lado_t;

DovahkiinP NuevoDovahkiin();
int DestruirDovahkiin()
void FijarFuente(Dovahkiin D, u64 x)
void FijarResumidero(DovahkiinP, u64x)
int ImprimirFuente()
int ImprimirResumidero()
Lado LeerUnLado()
int CargarUnLado(DovahkiinP D, Lado L)
int Prepararse(Dovahkiinp D)
int ActualizarDistancias(DovahkiinP D)
int BusquedaCaminoAumentante(Dovahkiin D)
u64 AumentarFlujo(DovahkiinP D)
u64 AumentarFlujoYtambienImprimirCamino(DovahkiinP N)
void imprimirFlujo(DovahkiinP D)
void ImprimirCorte(DovahkiinP D)
#endif
