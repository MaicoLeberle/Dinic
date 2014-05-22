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

/*  La siguiente implementación de BusquedaCaminoAumentante va guardando en D->FF_DFS 
    todas las aristas involucradas en un camino de s a t. */
int BusquedaCaminoAumentante(DovahkiinP D) {
    /*  Si ya existía una lista D->FF_DFS, entonces no se llamó a alguna función AumentarFlujo.
        ¿Está bien proseguir? CHEQUEAR ESTO! */
    if(D->FF_DFS) {
        D->FF_DFS = list_destroy_keep_members(D->FF_DFS);
    }
    D->FF_DFS = list_create();
    /*  Se debe iterar sobre todos los vecinos de la fuente para ver si se puede llegar al resumidero. */
    member_t vecinos_de_s = list_get_first((D->fuente)->vecinos_forward);
    while(vecinos_de_s) {
        /*  Se agrega el lado forward a D->FF_DFS y se procede a ver si a partir de aquí se puede llegar al resumidero. */
        D->FF_DFS = list_add(vecinos_de_s->member);
        /*  El flujo desde el cual se debe comenzar el análisis es el flujo restante a enviar a través de este lado
            desde la fuente al vértice (vecinos_de_s->member)->y. */
        D->flujo = (vecinos_de_s->member)->c - (vecinos_de_s->member)->f;
        /*  Se debe pasar como parámetro la iteración actual que estableció nivel(x) de cada vértice visitado. */
        if(BusquedaCaminoAumentanteAux((vecinos_de_s->member)->y, D->FF_DFS, D->resumidero, &(D->flujo), D->iteracion) {
            return 1;
        } else {
            /*  Si no se llega al resumidero desde (vecinos_de_s->member)->y, entonces se debe eliminar el último
                lado agregado a la lista que está marcando el camino de la fuente al resumidero. */
            D->FF_DFS = remove_last(D->FF_DFS);
        }
        /*  Se debe analizar a continuación el siguiente vecino forward de la fuente. */
        vecinos_de_s = list_next(vecinos_de_s);
    }
    return 0;
}

/*  La siguiente función auxiliar sirve para implementar DFS desde la fuente respetando
    los niveles de BFS establecidos al llamar a ActualizarDistancias(). 
    Devuelve true si se llega al rseumidero y false si no se llega. */
bool BusquedaCaminoAumentanteAux(VerticeP vertice, list_t camino_inicial, VerticeP resumidero, u64P flujo, unsigned int iteracion) {
    member_t aux;

    /*  Primero, se consideran los vecinos forward. */
    aux = list_get_first(vertice->vecinos_forward);
    while(aux) {
        /*  Se calcula el flujo que se debería enviar si se elige el lado aux->member. */
        min_flujo = min(flujo, (aux->member)->c - (aux->member)->f);
        if(((aux->member)->y)->iteracion != iteracion || ((aux->member)->y)->distancia > resumidero->distancia || ((aux->member)->y)->distancia != vertice->distancia + 1 || !min_flujo) {
            /*  Si la iteracion que estableció la distancia desde la fuente al vértice y (del lado xy, representado en aux->member) no es
                la última iteración, entonces la distancia de y es considerada como inválida.
                Si la distancia del vértice y es mayor que la distancia del resumidero, entonces no hay forma de continuar buscando 
                caminos al resumidero respetando los niveles de BFS.
                Por otro lado, si la nivel(y) != nivel(x) + 1, entonces este no es un vértice válido y se debe descartar su 
                subsiguiente análisis. 
                Por último, si el flujo a mandar es 0, entonces el lado está saturado. */
            continue;
        } else {
            /*  Es un vértice válido para continuar el análisis. */
            if(comparar_vertice((aux->member)->y, resumidero)) {
                /*  Se llegó al resumidero. Se debe agregar el último lado de este camino y actualizar el flujo que se debería mandar
                    al llamar a AumentarFlujo o AumentarFlujoYTambienImprimirCamino. */
                list_add(camino_inicial, aux->member);
                (*flujo) = min_flujo;
                return true;
            } else {
                /*  Se debe seguir buscando. */
                if(!BusquedaCaminoAumentanteAux((aux->member)->y, list_add(camino_inicial, aux->member), resumidero, min_flujo, iteracion)) {
                    /*  No se llega al resumidero a través de y. Se debe eliminar el último lado agregado a la lista del camino 
                        al llamar a list_add(camino_inicial, aux->member). */
                    /*  ¿Se debe hacer un destruir_vertice()? ¿Esto no elimina el lado guardado en D->data? */
                    remove_last_keep_content(camino_inicial);
                    /*  Pasar a considerar el siguiente vecino forward de vertice. */
                    aux = list_next(aux);
                } else {
                    /*  ¡Se llegó al resumidero! Observar que las llamadas recursivas van alterando D->FF_DFS, por lo que ésta ya
                        contiene el camino de la fuente al resumidero. */
                    return true;
                }
            }
        }
    }
    /*  Después, se consideran los vecinos backward. Observar como el vértice analizado en los lados forward es y, mientras que en los 
        lados backward es necesario analizar el vértice x. */
    aux = list_get_first(vertice->vecinos_backward);
    while(aux) {
        /*  Se calcula el flujo que se debería enviar si se elige el lado aux->member. */
        min_flujo = min(flujo, (aux->member)->f);
        if(((aux->member)->x)->iteracion != iteracion || ((aux->member)->x)->distancia > resumidero->distancia || ((aux->member)->x)->distancia != vertice->distancia + 1 || !min_flujo) {
            /*  Si la iteracion que estableció la distancia desde la fuente al vértice x (del lado xy, representado en aux->member) no es
                la última iteración, entonces la distancia de x es considerada como inválida.
                Si la distancia del vértice x es mayor que la distancia del resumidero, entonces no hay forma de continuar buscando 
                caminos al resumidero respetando los niveles de BFS.
                Por otro lado, si la nivel(x) != nivel(y) + 1, entonces este no es un vértice válido y se debe descartar su 
                subsiguiente análisis. 
                Por último, si el flujo a mandar es 0, entonces el flujo en el lado es 0, y no se puede devolver flujo. */
            continue;
        } else {
            /*  Es un vértice válido para continuar el análisis. Como no existen (¿es así? chequear con el enunciado) lados forward desde
                el resumidero, entonces no existe la posibilidad de llegar a éste a través de un último lado backward.
                Luego, sólo se debe proceder a evaluar los lados subsiguientes. */
            if(!BusquedaCaminoAumentanteAux((aux->member)->x, list_add(camino_inicial, aux->member), resumidero, min_flujo, iteracion) {
                /*  No se llega al resumidero a través de x. Se debe eliminar el último lado agregado a la lista del camino 
                    al llamar a list_add(camino_inicial, aux->member). */
                /*  ¿Se debe hacer un destruir_vertice()? ¿Esto no elimina el lado guardado en D->data? */
                remove_last_keep_content(camino_inicial);
                /*  Pasar a considerar el siguiente vecino forward de vertice. */
                aux = list_next(aux);
            } else {
                /*  ¡Se llegó al resumidero! Observar que las llamadas recursivas van alterando D->FF_DFS, por lo que ésta ya
                    contiene el camino de la fuente al resumidero. */
                return true;
            }
        }

    }
    /*  Si no se llegó a t por vecinos forward ni vecinos backward, entonces no es posible llegar a t
        con estos niveles de BFS desde vertice. */
    return false;
}


u64 AumentarFlujo(DovahkiinP D) {
    if(D->FF_DFS) {
        /*  BusquedaCaminoAumentante encontró un camino de la fuente al resumidero y lo almacenó en D->FF_DFS.
            Se procede a actualizar el flujo en los lados involucrados.
            Se empieza desde el último lado guardado porque se pide el imprimir desde t a s. */

        /*  Verifico que efectivamente se ha encontrado un flujo distinto de 0 para enviar. */
        assert(D->flujo);

        member_t lado = list_get_last(D->FF_DFS);
        while(lado) {
            (lado->member)->f = (lado->member)->flujo + D->flujo;
            lado = list_next(lado);
        }
        /*  Se debe resetear el flujo a enviar y limpiar la lista D->FF_DFS. */
        D->flujo = 0;
        D->FF_DFS = list_destroy_keep_members(D->FF_DFS);
        return 1;
    } else {
        /*  No se ha hecho una llamada a BusquedaCaminoAumentante que haya establecido un camino de la fuente
            al resumidero en D->FF_DFS. */
        return 0;
    }
}


u64 AumentarFlujoYTambienImprimirCamino(DovahkiinP D) {
    if(D->FF_DFS) {
        /*  BusquedaCaminoAumentante encontró un camino de la fuente al resumidero y lo almacenó en D->FF_DFS.
            Se procede a actualizar el flujo en los lados involucrados.
            Se empieza desde el último lado guardado porque se pide el imprimir desde t a s. */

        /*  Verifico que efectivamente se ha encontrado un flujo distinto de 0 para enviar. */
        assert(D->flujo);

        member_t lado = list_get_last(D->FF_DFS);
        while(lado) {
            (lado->member)->f = (lado->member)->flujo + D->flujo;
            printf("%" PRIu64 ";", (lado->member)->y));
            lado = list_next(lado);
        }
        printf("s:\t%" PRIu64, D->flujo);
        /*  Se debe resetear el flujo a enviar. */
        D->flujo = 0;
        D->FF_DFS = list_destroy_keep_members(D->FF_DFS);
        return 1;
    } else {
        /*  No se ha hecho una llamada a BusquedaCaminoAumentante que haya establecido un camino de la fuente
            al resumidero en D->FF_DFS. */
        return 0;
    }
}

/*  La estrategia a seguir en la siguiente función es sencilla: se recorre cada uno de los vértices
    y se imprimen los lados que comiencen en él (i.e., los lados en vecinos_forward). Esto es suficiente
    y correcto para lograr el acometido de ImprimirFlujo, pues cada lado ingresado está en 
    vecinos_forward de algún vértice y no se repite en ninguna otra lista vecinos_forward (evidentemente,
    sí se repiten en alguna lista vecinos_backward de algún vértice). */
void ImprimirFlujo(DovahkiinP D); {
    // Terminar
}

void ImprimirValorFlujo(DovahkiinP D) {
    // Terminar
}

void ImprimirCorte(DovahkiinP D) {
    // Terminar
}

u64 min(u64 a, u64 b) {
    return (a <= b ? a : b);
}