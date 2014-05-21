#include "API.h"

#define MAX_SIZE 21 //2^64 -> 21 campos (+1 por \0)
#define FORWARD 1
#define BACKWARD 0

DovahkiinP NuevoDovahkiin() {
    DovahkiinP new = calloc(1, sizeof(struct DovahkiinSt));
    
    if(new) {
        new->fuente = NULL;
        new->resumidero = NULL;
        new->flujo = 0;
        new->data = list_create();
        new->temp = list_create();
        new->iteracion = 0;
    }
    
    return new;
}

int DestruirDovahkiin(DovahkiinP D) {
    assert(D);
    
    if(D->data) {
        D->data = list_destroy(D->data, &destruir_vertice);
    }
    if(D->temp) {
        free(D->temp);
    }
    free(D);
    
    return 1;
}

void FijarFuente(DovahkiinP D, u64 x) {
    assert(D);
    
    VerticeP v = crear_vertice(x);
    VerticeP check = list_search(D->data, v, &comparar_vertice);
    v = destruir_vertice(v);
    
    D->fuente = check;
}

void FijarResumidero(DovahkiinP D, u64 x) {
    assert(D);
    
    VerticeP v = crear_vertice(x);
    VerticeP check = list_search(D->data, v, &comparar_vertice);
    v = destruir_vertice(v);
    
    D->resumidero = check;
}

int ImprimirFuente(DovahkiinP D) {
    assert(D);
    
    int result = -1;
    
    if(D->fuente) {
        printf("Fuente: %" PRIu64 "\n", D->fuente->nombre);
        result = 0;
    }
    
    return result;
}

int ImprimirResumidero(DovahkiinP D) {
    assert(D);
    
    int result = -1;
    
    if(D->resumidero) {
        printf("Fuente: %" PRIu64 "\n", D->resumidero->nombre);
        result = 0;
    }
    
    return result;
}

static uint64_t atoi64(char *s) {
    /*
        No se checkea si hay overflow !!!
    */
    uint64_t result = 0;
    char c = *s++;
    
    while (c != '\0' && isdigit(c)) {
		result = result * 10  + (c - '0');
		c = *s++;
	}
	
	return result;
}

Lado LeerUnLado() {
    /*
        La funcion lee una linea desde stdin que representara un lado
        y devuelve el elemento de tipo Lado que represente este lado si
        la linea es valida , o bien LadoNulo si la linea no es valida.
    */
    char *buffer = calloc(MAX_SIZE, sizeof(uint8_t));
    char *tokenx, *tokeny, *tokenc;
    VerticeP x = NULL;
    VerticeP y = NULL;
    LadoP new = NULL;
    
    if(fgets(buffer, MAX_SIZE, stdin) != NULL) {
        tokenx = strtok(buffer, " ");
        tokeny = strtok(NULL, " ");
        tokenc = strtok(NULL, "\n");
        if(tokenx && tokeny && tokenc && tokenx[0] != '-' && tokeny[0] != '-'&& tokenc[0] != '-') {
            x = crear_vertice(atoi64(tokenx));
            y = crear_vertice(atoi64(tokeny));
            new = crear_lado(x, y, atoi64(tokenc));
        }
        else {
            free(new);
            new = NULL;
        }
    }
    free(buffer);
    
    return new;
}

int CargarUnLado(DovahkiinP D, LadoP L) {
    assert(D);
    assert(L);
    
    VerticeP x = (VerticeP)list_search(D->data, L->x, &comparar_vertice);
    VerticeP y = (VerticeP)list_search(D->data, L->y, &comparar_vertice);
    if(x == NULL) {
        D->data = list_add(D->data, L->x);
    }
    else {
        L->x = destruir_vertice(L->x);
        L->x = x;
    }
    if(list_search(D->data, L->y, &comparar_vertice) == NULL) {
        D->data = list_add(D->data, L->y);
    }
    else {
        L->y = destruir_vertice(L->y);
        L->y = y;
    }
    member_t new = member_create(L);
    L->x->vecinos_forward = list_direct_add(L->x->vecinos_forward, new);
    L->y->vecinos_backward = list_direct_add(L->y->vecinos_backward, new);
    
    return 1;
}

int Prepararse(DovahkiinP D) {
    assert(D);
    
    int result = 0;
    if(D->fuente && D->resumidero) {
        result = 1;
    }
    
    return result;
}

int ActualizarDistancias(DovahkiinP D) {
    assert(D);

    member_t temp = NULL;
    member_t next = NULL;
    int result = 0;
    
    D->iteracion += 1;
    
    if(!D->temp) {
        D->temp = list_create();
    }
    
    D->temp = list_add(D->temp, D->fuente);//Destruir member_t
    temp = list_get_first(D->temp);
    
    while(temp && !comparar_vertice(get_content(temp), D->resumidero)) {
        D->temp = add_neighboor_to_list(D->temp, get_content(temp), D->iteracion);
        temp = list_next(temp);
    }
    if(comparar_vertice(get_content(temp), D->resumidero)) {
        result = 1;
    }
    
    temp = list_get_first(D->temp);
    while(temp) {
        next = list_next(temp);
        free(temp);
        temp = next;
    }
    return result;
}

/*  La siguiente implementación de BusquedaCaminoAumentante va guardando en D->temp 
    todas las aristas involucradas en un camino de s a t. */
int BusquedaCaminoAumentante(DovahkiinP D) {
    unsigned int nivel = D->fuente->distancia;
    if(!D->temp) {
        D->temp = list_create();
    }
    member_t vecinos_de_s = (D->fuente)->vecinos_forward;
    while(vecinos_de_s) {
        D->temp = list_add(vecinos_de_s);
        if(BusquedaCaminoAumentanteAux(((list_get_last(D->temp))->member)->y, D->temp, D->resumidero, &(D->flujo))) {
            return 1;
        } else {
            D->temp = remove_last(D->temp);
        }
        vecinos_de_s = list_next(vecinos_de_s);
    }
    return 0;
}

/*  La siguiente función auxiliar sirve para implementar DFS desde s respetando
    los niveles de BFS establecidos al llamar a ActualizarDistancias(). 
    Devuelve true si se llega a t y false si no se llega. */
bool BusquedaCaminoAumentanteAux(VerticeP vertice, list_t camino_inicial, VerticeP resumidero, u64P flujo) {
    member_t aux = list_get_first(vertice->vecinos_forward);
    while(aux) {
        min_flujo = min(flujo, (aux->member)->c - (aux->member)->f);
        if(((aux->member)->y)->distancia > resumidero->distancia || ((aux->member)->y)->distancia != vertice->distancia + 1 || !min_flujo) {
            continue;
        } else {
            if(comparar_vertice((aux->member)->y, resumidero)) {
                camino_inicial->add(aux->member);
                (*flujo) = min_flujo;
                return true;
            } else {
                if(!BusquedaCaminoAumentanteAux((aux->member)->y, camino_inicial->add(aux->member), resumidero, min_flujo)) {
                    remove_last(camino_inicial, &destruir_vertice);
                    aux = list_next(aux);
                } else {
                    return true;
                }
            }
        }
    }
    return false;
}


u64 AumentarFlujo(DovahkiinP D) {
    member_t lado = list_get_last(D->temp);
    while(lado) {
        (lado->member)->f = (lado->member)->flujo + D->flujo;
        lado = list_next(lado);
    }
    /*  Falta chequear si algo salió mal. */
    return 1;
}


u64 AumentarFlujoYTambienImprimirCamino(DovahkiinP D) {
    /*  Se empieza desde el último lado guardado porque se pide el camion desde t a s. */
    member_t lado = list_get_last(D->temp);
    while(lado) {
        (lado->member)->f = (lado->member)->flujo + D->flujo;
        printf("%" PRIu64 ";", (lado->member)->y));
        lado = list_next(lado);
    }
    printf("s:\t%" PRIu64, D->flujo);
    /*  Falta chequear si algo salió mal. */
    return 1;
}

u64 min(u64 a, u64 b) {
    return (a <= b ? a : b);
}