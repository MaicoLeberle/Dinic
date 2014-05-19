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
    }
    return new;
    
}

int DestruiDovahkiin(DovahkiinP D) {
    assert(DovahkiinP);
    
    void *(*puntero_funcion)(void *);
    puntero_funcion = &destruir_vertice;
    
    if(D->data) {
        D->data = list_destroy(D->data, puntero_funcion);
    }
    if(D->temp) {
        D->temp = list_destroy(D->temp, puntero_funcion);
    }
}

void FijarFuente(DovahkiinP D, u64 x) {
    assert(D);
    
    VerticeP search = NULL;
    bool (*puntero_funcion)(void *, void *);
    puntero_funcion = &comparar_vertice;
    
    VerticeP temp_x = crear_vertice(x);
    search = list_search(D->data, (void *)temp_x, puntero_funcion)
    if(search) {
        D->fuente = search;
    }
}

void FijarResumidero(DovahkiinP D, u64 x) {
    assert(D);
    
    VerticeP search = NULL;
    bool (*puntero_funcion)(void *, void *);
    puntero_funcion = &comparar_vertice;
    
    VerticeP temp_x = crear_vertice(x);
    search = list_search(D->data, (void *)temp_x, puntero_funcion)
    if(search) {
        D->resumidero = search;
    }
}

int ImprimirFuente(DovahkiinP D) {
    assert(D);
    
    int result = -1;
    
    if(D->fuente) {
        printf("Fuente: %" PRIu64 "\n", D->fuente);
        result = 0;
    }
    
    return result;
}

int ImprimirFuente(DovahkiinP D) {
    assert(D);
    
    int result = -1;
    
    if(D->resumidero) {
        printf("Resumidero: %" PRIu64 "\n", D->resumidero);
        result = 0;
    }
    
    return result;
}

uint64_t atoi64(char *s) {
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
    char *buffer = calloc(1, sizeof(MAX_SIZE));
    char *tokenx, *tokeny, *tokenc;
    VerticeP x = NULL;
    VerticeP y = NULL;
    LadoP new = NULL;
    FILE *text = fopen("text", "r");//à virer si stdin
    
    if(fgets(buffer, MAX_SIZE, text) != NULL) {
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
        if(buffer != NULL) {
            free(buffer);
        }
    }
    fclose(text);//à virer aussi si stdin
    return &new;
}

int CargarUnLado(DovahkiinP D, LadoP L) {
    assert(D);
    assert(L);
    
    bool (*puntero_funcion)(void *, void *);
    puntero_funcion = &comparar_vertice;
    
    VerticeP x = (VerticeP)list_search(D->data, (void *)(L->x), puntero_funcion);
    VerticeP y = (VerticeP)list_search(D->data, (void *)(L->y), puntero_funcion);
    if(x == NULL) {
        x = L->x;
        D->data = list_add(D->data, x);
    }
    if(y == NULL) {
        y = L->y;
        D->data = list_add(D->data, y);
    }
    x->vecinos_forward = list_add(x->vecinos_forward, L);
    y->vecinos_backward = list_add(y->vecinos_backward, L);
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
    
    /*Para ustedes*/   
}

int BusquedaCaminoAumetante(DovahkiinP D) {
    /*Para ustedes*/
}


