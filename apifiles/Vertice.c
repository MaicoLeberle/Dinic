#include "Lado.h"
#include "Vertice.h"

VerticeP crear_vertice(u64 nombre) {
    VerticeP new = calloc(1, sizeof(struct Vertice));
    if(new) {
        new->nombre = nombre;
        new->vecinos_forward = list_create();
        new->vecinos_backward = list_create();
        new->distancia = 0;
        new->iteracion = 0;
    }
    
    return new;
}

void *destruir_vertice(void *ptr) {
    assert(ptr);
    
    VerticeP vertice = (VerticeP)ptr;
    if(vertice->vecinos_forward) {
        vertice->vecinos_forward = list_destroy(vertice->vecinos_forward, &destruir_lado);
    }
    free(vertice->vecinos_backward);
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

list_t add_neighboor_to_list(list_t list, VerticeP v, unsigned int i) {
    assert(list);
    assert(v);
    
    member_t member = list_get_first(v->vecinos_forward);
    LadoP lado = NULL;
    
    while(member) {
        lado = get_content(member);
        if(lado->y->iteracion != i) {
            //Si no lo visite esta iteracion ...
            lado->y->distancia = lado->x->distancia + 1;
            lado->y->iteracion = i;
            list = list_add(list, lado->y);
        }
        member = list_next(member);
    }
    
    return list;
}
