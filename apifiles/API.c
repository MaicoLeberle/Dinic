#include "API.h"

#define MAX_SIZE 21 //2^64 -> 21 campos (+1 por \0)
#define FORWARD 1
#define BACKWARD 0

u64 min(u64 a, u64 b);

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
        /*  Sólo es necesario destruir la lista, pues los vértices ya se han eliminado
            al destruir D->data. */
        D->temp = list_destroy_keep_members(D->temp); 
    }
    free(D);
    D = NULL;
    return 1;
}

void FijarFuente(DovahkiinP D, u64 x) {
    assert(D);
    
    VerticeP v = crear_vertice(x);
    VerticeP check = list_search(D->data, v, &comparar_vertice);

    if(check) {
        D->fuente = check;
        v = destruir_vertice(v);
    } else {
        D->fuente = v;
    }
}

void FijarResumidero(DovahkiinP D, u64 x) {
    assert(D);
    /* Con una Main traicionera podrian fijarse muchas fuentes. deberia fijar 1 sola fuente y no mas */
    //RP de PY : El main lo hacemos nosotros por lo cual no hay trampa pero entiendo la idea.
    //Lo que si se podria hacer es prohibir cambiar la fuente y el resumidero si ya estamos
    //buscando un camino (tipo un booleano en dovakin) pero es un detalle menor creo.
    
    VerticeP v = crear_vertice(x);
    VerticeP check = list_search(D->data, v, &comparar_vertice);
    
    if(check) {
        D->resumidero = check;
        v = destruir_vertice(v);
    } else {
        D->resumidero = v;
    }
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
static bool is_valid(char *s) {
    bool valid = false;
    char c = '\0';
    
    if(s && strlen(s) > 0) {  
        c = *s++;
        while (c != '\0' && isdigit(c)) {
            c = *s++;
        }
        if(c == '\0') {
            valid = true;
        }
    }
    return valid;
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
        if(is_valid(tokenx) && is_valid(tokeny) && is_valid(tokenc)){
            x = crear_vertice(atoi64(tokenx));
            y = crear_vertice(atoi64(tokeny));
            new = crear_lado(x, y, atoi64(tokenc));
        }
        else {
            /*  "free(new);" significa "free(NULL);", pues "LadoP new = NULL;", 
                lo cual no es necesario realizar. */
            new = LadoNulo;
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
    member_t new = member_create(L);
    member_t new_2 = member_create(L);
    int result = 0;
    
    if(x == NULL) {
        D->data = list_add(D->data, L->x);
    }
    else {
        L->x = destruir_vertice(L->x);
        L->x = x;
    }
    if(y == NULL) {
        D->data = list_add(D->data, L->y);
    }
    else {
        L->y = destruir_vertice(L->y);
        L->y = y;
    }
    /*  aux es el lado inverso de L. */
    Lado aux = crear_lado(L->y, L->x, L->c);
    if(!comparar_vertice(L->x, L->y) 
        && !list_search(L->x->vecinos_forward, L, &comparar_lados) 
        && !list_search(L->x->vecinos_backward, aux, &comparar_lados) 
        && !list_search(L->y->vecinos_backward, L, &comparar_lados) 
        && !list_search(L->y->vecinos_forward, aux, &comparar_lados)) {
        /*  Aquí se comprobó que el lado no es un loop, que no está ya registrado, y que el lado inverso no existe. */
        L->x->vecinos_forward = list_direct_add(L->x->vecinos_forward, new);
        L->y->vecinos_backward = list_direct_add(L->y->vecinos_backward, new_2);
        result =  1;
    } else {
        result = 0;
    }
    aux = destruir_lado(aux);
    return result;
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
    /*
        Hace una busqueda FF-BFS (forwards y los backwards
        si lado->c - lado->f > 0 y no fue visitado entonces lo agrega (forwards)
        si lado->f > 0 y no fue visitado lo agrega (backwards)
        sino no agrega.
    */
    assert(D);
 
    member_t temp = NULL;
    int result = 0;
   
    D->iteracion += 1;
   
    if(!D->temp) {
        D->temp = list_create();
    }
   
    D->temp = list_add(D->temp, D->fuente);//Destruir member_t
    D->fuente->iteracion = D->iteracion;
    temp = list_get_first(D->temp);
     
    while(temp && !comparar_vertice(get_content(temp), D->resumidero)) {
        D->temp = add_neighboor_to_list(D->temp, get_content(temp), D->resumidero,  D->iteracion);
        temp = list_next(temp);
    }
    if(temp && comparar_vertice(get_content(temp), D->resumidero)) {
        /*  Se encontró un camino al resumidero, por lo que D->temp se puede limpiar. */
        result = 1;
        D->temp = list_destroy_keep_members(D->temp);
    }
    return result;
}

VerticeP avanzar(VerticeP x, u64 *flujo) {
    /*
        Principalmente actualiza los datos del siguiente vertice.
        Actualiza:
        -el ancestro de next
        -el temp_flujo de next
        -y avisa si es forward
    */
    assert(x);
    
    VerticeP next = x;
    LadoP lado = get_content(list_get_first(x->vecinos_posibles));
    bool posible = false;
    if(comparar_vertice(x, lado->x)) {
        if(lado->c - lado->f) {
            posible = true;
            next = lado->y;
            *flujo = min(*flujo, lado->c - lado->f);
            next->is_forward = true;
        }
    }
    else {
        if(lado->f) {
            posible = true;
            next = lado->x;
            *flujo = min(*flujo, lado->f);
            next->is_forward = false;
        }
    }
    if(posible) {
        next->ancestro = x;
        next->temp_flujo = *flujo;
    }
    else {
        x->vecinos_posibles = remove_first_keep_content(x->vecinos_posibles);
    }
            
    return next;
}

VerticeP retroceder(VerticeP x) {
    assert(x);
    x->ancestro->vecinos_posibles = remove_first_keep_content(x->ancestro->vecinos_posibles);
    return x->ancestro;
}

int BusquedaCaminoAumentante(DovahkiinP D) {
    /*
        Buscar un camino hasta t desde s.
        
        Si el vertice actual tiene aristas disponibles
        entonces avanzar sino retroceder
    */
    assert(D);
    
    int result = 0;
    VerticeP vertice_actual = D->fuente;
    bool continuar = true;
    u64 flujo = (u64)INFINITY;
    
    while(!comparar_vertice(D->resumidero, vertice_actual) && continuar) {
        if(!list_empty(vertice_actual->vecinos_posibles)) {
            vertice_actual = avanzar(vertice_actual, &flujo);
        }
        else if(!comparar_vertice(D->fuente, vertice_actual)) {
            vertice_actual = retroceder(vertice_actual);
        }
        else {
            continuar = false;
        }
    }
    if(comparar_vertice(D->resumidero, vertice_actual)) {
        result = 1; 
    }
    
    return result;
}

u64 AumentarFlujo(DovahkiinP D) {
    /*
        De t sacamos el ancestro.
        Sacamos el ancestro del ancestro del ancestro ... hasta
        llegar a la fuente.
        vertice->temp_aux guarda el flujo que se podria mandar
        hasta tal vertice.
    */
    assert(D);
    
    u64 flujo = D->resumidero->temp_flujo;
    VerticeP vertice_actual = D->resumidero;
    LadoP lado = NULL, aux_l = NULL;
    
    while(!comparar_vertice(D->fuente, vertice_actual)) {
        if(vertice_actual->is_forward) {
            aux_l = crear_lado(vertice_actual->ancestro, vertice_actual, 0);
            lado = list_search(vertice_actual->vecinos_backward, aux_l, &comparar_lados);   
            lado->f += flujo;
        }
        else {
            aux_l = crear_lado(vertice_actual, vertice_actual->ancestro, 0);
            lado = list_search(vertice_actual->vecinos_forward, aux_l, &comparar_lados);   
            lado->f = lado->f - flujo;
        }
        aux_l = destruir_lado(aux_l);
        vertice_actual = vertice_actual->ancestro;
    }
    D->flujo += flujo;
    return flujo;
}

u64 AumentarFlujoYTambienImprimirCamino(DovahkiinP D) {
    assert(D);
    
    VerticeP vertice_actual = D->resumidero, next = NULL;
    u64 flujo = AumentarFlujo(D);
    
    printf("t;");
    while(!comparar_vertice(vertice_actual, D->fuente)) {
        next = vertice_actual->ancestro;
        printf("%" PRIu64, vertice_actual->nombre);
        if(vertice_actual->is_forward) {
            printf(";");
        }
        else {
            printf(">");
        }
        vertice_actual = next;
    }
    printf("s\n");
    
    return flujo;
}


/*  La estrategia a seguir en la siguiente función es sencilla: se recorre cada uno de los vértices
    y se imprimen los lados que comiencen en él (i.e., los lados en vecinos_forward). Esto es suficiente
    y correcto para lograr el acometido de ImprimirFlujo, pues cada lado ingresado está en 
    vecinos_forward de algún vértice y no se repite en ninguna otra lista vecinos_forward (evidentemente,
    sí se repiten en alguna lista vecinos_backward de algún vértice). */
void ImprimirFlujo(DovahkiinP D){
    member_t temp_vertice;
    member_t temp_lado;
    Lado lado;
    if (!(list_empty(D->temp))) {
        /*  Al llamar ActualizarDistancias no se limpió la lista D->temp porque no se llegó al resumidero. 
            Luego, el flujo es maximal y el corte es minimal. */
        printf("Flujo Maximal:\n");
    } else {
        printf("Flujo (no maximal):\n");
    }

    temp_vertice = list_get_first(D->data);
    while(temp_vertice) {
        temp_lado = list_get_first(((VerticeP)(get_content(temp_vertice)))->vecinos_forward);
        while(temp_lado) {
            lado = (Lado)(get_content(temp_lado));
            printf("Lado %"PRIu64", %"PRIu64":\t%"PRIu64"\n", (lado->x)->nombre, (lado->y)->nombre, lado->f);
            temp_lado = list_next(temp_lado);
        }

        temp_vertice = list_next(temp_vertice);
    }
}


u64 ValorFlujo(DovahkiinP D) {
    member_t aux;
    Lado lado;
    u64 f = 0;

    aux = list_get_first((D->fuente)->vecinos_forward);
    while(aux){
        lado = (Lado) get_content(aux);
        f += lado->f;
        aux = list_next(aux);
    }
    return f;
}
/*   */
void ImprimirValorFlujo(DovahkiinP D) {
    u64 f = ValorFlujo(D);
    if (D->temp){
        printf("Valor del flujo Maximal: \t%" PRIu64"\n", f);
    }else{
        printf("Valor del flujo (no maximal):\t%" PRIu64"\n", f);
    }
}


/*  Precondición: Se corrió ActualizarDistancia y no se llegó al resumidero. El corte minimal
    está en D->temp. */
void ImprimirCorte(DovahkiinP D) {
    assert(D->temp);

    member_t temp = list_next(list_get_first(D->temp));
    printf("Corte Minimal:\tS = {s");
    while(temp) {
        printf(",%"PRIu64, ((VerticeP)(get_content(temp)))->nombre);
        temp = list_next(temp);
    }
    printf("}\n");
    printf("Capacidad:\t%" PRIu64"\n", ValorFlujo(D));
}

u64 min(u64 a, u64 b) {
    return (a <= b ? a : b);
}
