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
    /*  Recordar que todos los lados están presentes entre todos los vecinos_forward de los vértices
        y están espejados entre todos los vecinos_backward de los vértices, es por eso que no se deben
        eliminar en este punto, ya que ya fueorn eliminados. */
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

list_t add_neighboor_to_list(list_t list, VerticeP v, VerticeP resumidero, unsigned int i) {
    assert(list);
    assert(v);
   
    member_t member = list_get_first(v->vecinos_forward);
    LadoP lado = get_content(member);
    bool found = false;
    
    while(member && !found) {
        lado = get_content(member);
        if(lado->y->iteracion != i && (lado->c - lado->f) > 0) {
            //Si no lo visite esta iteracion y hay capacidad
            (lado->y)->distancia = (lado->x)->distancia + 1;
            (lado->y)->iteracion = i;
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
        if(lado->y->iteracion != i && lado->f > 0) {
            //Si no lo visite esta iteracion y hay flujo
            (lado->x)->distancia = (lado->y)->distancia + 1;
            (lado->x)->iteracion = i;
            list = list_add(list, lado->y);
            if(comparar_vertice(resumidero, lado->x)) {
                found = true;
            }
        }
        member = list_next(member);
    }
    return list;
}
