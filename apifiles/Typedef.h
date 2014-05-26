#ifndef _TYPEDEF_H
#define _TYPEDEF_H

#define LadoNulo ((Lado)(NULL))
#include <stdint.h>

/*  Notar que a lo largo de todo el proyecto se utiliza el macro PRIu64 para imprimir,
    por standard output con printf, números de tipo u64. */

typedef uint64_t u64;
typedef u64 *u64P;
typedef struct DovahkiinSt *DovahkiinP;
typedef struct Vertice *VerticeP;
typedef struct Lado *LadoP;
typedef struct Lado *Lado;
typedef struct FF_DFSLado *FF_DFSLadoP;
#endif
