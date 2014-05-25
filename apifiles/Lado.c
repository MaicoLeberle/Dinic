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

void *destruir_lado(void *ptr) {
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
