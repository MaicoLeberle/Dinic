#include "Lado.h"
#include "Vertice.h"

VerticeP crear_vertice(u64 nombre) {
    VerticeP new = calloc(1, sizeof(struct Vertice));
    if(new) {
        new->nombre = nombre;
        new->temp_flujo = 0;
        new->vecinos_forward = list_create();
        new->vecinos_backward = list_create();
        new->vecinos_posibles = list_create();
        new->distancia = 0;
        new->iteracion = 0;
        new->is_forward = false;
        new->ancestro = NULL;
    }
    
    return new;
}

void *destruir_vertice(void *ptr) {
    assert(ptr);
    
    VerticeP vertice = (VerticeP)ptr;
    if(vertice->vecinos_forward) {
        vertice->vecinos_forward = list_destroy(vertice->vecinos_forward, &destruir_lado);
    }
    vertice->vecinos_backward = list_destroy_keep_members(vertice->vecinos_backward);
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

list_t buscar_vecinos_posibles(VerticeP v, unsigned int i) {
    /*
        Agrega en v->vecinos_posibles todos los vecinos
        que cumplen :
        -Si level(v) = n entonces level(vecino_de_v) = n + 1
        -Si es forward entonces temp->c - temp->f > 0
        -Si es backward entonces temp->f > 0
    */
    assert(v);
    i += 1;
    i -= 1;
    member_t member = list_get_first(v->vecinos_forward);
    LadoP temp = NULL;
    
    if(!list_empty(v->vecinos_posibles)) {
        v->vecinos_posibles = list_destroy_keep_members(v->vecinos_posibles);
        v->vecinos_posibles = list_create();
    }
    
    while(member) {
        temp = get_content(member);
        if((v->distancia == temp->y->distancia - 1) && (temp->c - temp->f > 0)) {
            v->vecinos_posibles = list_add(v->vecinos_posibles, temp);
        }
        member = list_next(member);
    }
    
    member = list_get_first(v->vecinos_backward);
    
    while(member) {
        temp = get_content(member);
        if((v->distancia == temp->x->distancia - 1) && temp->f > 0) {
            v->vecinos_posibles = list_add(v->vecinos_posibles, temp);
        }
        member = list_next(member);
    }
    
    return v->vecinos_posibles;
} 

list_t add_neighboor_to_list(list_t list, VerticeP v, VerticeP resumidero, unsigned int i) {
    assert(list);
    assert(v);
   
    member_t member = list_get_first(v->vecinos_forward);
    LadoP lado = NULL;
    bool found = false;
    
    while(member && !found) {
        lado = get_content(member);
        if(lado->y->iteracion != i && (lado->c - lado->f) > 0) {
            lado->y->distancia = lado->x->distancia + 1;
            lado->y->iteracion = i;
            list = list_add(list, lado->y);
            if(comparar_vertice(resumidero, lado->y)) {
                found = true;
            }
        }
        member = list_next(member);
    }
    member = list_get_first(v->vecinos_backward);
    while(member && !found) {
        lado = get_content(member);
        if(lado->x->iteracion != i && lado->f > 0) {
            lado->x->distancia = lado->y->distancia + 1;
            lado->x->iteracion = i;
            list = list_add(list, lado->x);
            if(comparar_vertice(resumidero, lado->x)) {
                found = true;
            }
        }
        member = list_next(member);
    }
    v->vecinos_posibles = buscar_vecinos_posibles(v, i);
    return list;
}
