#include "Lado.h"
#include "Vertice.h"

LadoP crear_lado(VerticeP x, VerticeP y, u64 c) {
    assert(x);
    assert(y);

    LadoP new = calloc(1, sizeof(struct Lado));
    if(new) {
        new->x = x;
        new->y = y;
        new->c = c;
        new->f = 0;
    }
    return new;
}

FF_DFSLadoP crear_FF_DFSLado(Lado lado, bool direccion) {
    assert(lado);
    FF_DFSLadoP new = calloc(1, sizeof(struct FF_DFSLado));
    if(new) {
        new->lado = lado;
        new->is_forward = direccion;
    }
    return new;
}

void *destruir_lado(void *ptr) {
    assert(ptr);
    
    free(ptr);
    ptr = NULL;
    
    return ptr;
}

void *destruir_FF_DFSLado(void *ptr) {
    assert(ptr);

    free(ptr);
    ptr = NULL;

    return ptr;
}

bool comparar_lados(void *a, void *b) {
    Lado xy = (Lado) a;
    Lado uv = (Lado) b;
    if(xy == LadoNulo)
        return (uv == LadoNulo);
    if(uv == LadoNulo)
        return (xy == LadoNulo);
    return (xy->x == uv->x && xy->y == uv->y);
}