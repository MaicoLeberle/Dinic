#include "Lado.h"
#include "Vertice.h"

VerticeP crear_vertice(u64 nombre) {
    VerticeP new = calloc(1, sizeof(struct Vertice));
    if(new) {
        new->nombre = nombre;
        new->vecinos_forward = list_create();
        new->vecinos_backward = list_create();
        new->iteracion = 0;
    }
    
    return new;
}

VerticeP destruir_vertice(VerticeP vertice) {
    assert(vertice);
    
    void *(*puntero_funcion)(void *);
    puntero_funcion = &destruir_lado;

    if(vertice->vecinos_forward) {
        vertice->vecinos_forward = list_destroy(vertice->vecinos_forward, &puntero_funcion);
    }
    if(vertice->vecinos_backward) {
        vertice->vecinos_backward = list_destroy(vertice->vecinos_backward, &puntero_funcion);
    }
    free(vertice);
    
    return NULL;
}

bool comparar_vertice(void *x, void *y) {
    assert(x);
    assert(y);
    
    VerticeP v_1 = (VerticeP)x;
    VerticeP v_2 = (VerticeP)y;
    return(v_1->nombre == v_2->nombre);
}
