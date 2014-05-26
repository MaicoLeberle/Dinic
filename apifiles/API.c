#include "API.h"

#define MAX_SIZE 50 //No quiero tener problemas con esto , doy mas espacio de lo que se necesita



u64 min(u64 a, u64 b);

/*  Crea y un nuevo Dovahkiin y devuelve un puntero (DovahkiinP) a éste en caso de que
    todo haya salido bien, o NULL en caso contrario. */
DovahkiinP NuevoDovahkiin() {
    DovahkiinP new = calloc(1, sizeof(struct DovahkiinSt));
    
    if(new) {
        /*  Se setean todos los atributos con valores que representan el network vacío y sin
            ningún tipo de ejecución. */
        new->fuente = NULL;
        new->resumidero = NULL;
        new->flujo = 0;
        new->data = list_create();
        new->temp = list_create();
        new->corte = list_create();
        new->iteracion = 0;
        new->flujo_maximal = false;
    }
    
    return new;
}

/*  Destruye D y libera su memoria, retornando 1 si fue posible y 0 si no. */
int DestruirDovahkiin(DovahkiinP D) {
    assert(D);
    
    /*  Se limpian todas las listas necesarias para ejecutar los algoritmos requeridos. */
    if(D->data) {
        D->data = list_destroy(D->data, &destruir_vertice);
    }
    if(D->temp) {
        D->temp = list_destroy_keep_members(D->temp); 
    }
    if(D->corte) {
        D->corte = list_destroy_keep_members(D->corte);
    }
    free(D);
    D = NULL;
    return 1;
}

/*  FijarFuente asegura que se fija el número x como la fuente del network D. */
void FijarFuente(DovahkiinP D, u64 x) {
    assert(D);
    
    /*  Se crea un vértice auxiliar para chequear si el vértice que se desea asignar como
        fuente ya existe. */
    VerticeP v = crear_vertice(x);
    /*  Se busca el vértice en la lista de vértices del network. */
    VerticeP check = list_search(D->data, v, &comparar_vertice);

    if(check) {
        /*  Si el vértice ya existe, se registra como fuente y se elimina el vértice 
            auxiliar. */
        D->fuente = check;
        v = destruir_vertice(v);
    } else {
        /*  Si el vértice no existe, se lo agrega a la lista de vértices del network y se
            registra como fuente. */
        D->data = list_add(D->data, v);
        D->fuente = v;
    }
}

/*  FijarResumidero asegura que se fija el número x como el resumidero del network D. */
void FijarResumidero(DovahkiinP D, u64 x) {
    assert(D);
    
    /*  Se crea un vértice auxiliar para chequear si el vértice que se desea asignar como
        resumidero ya existe. */
    VerticeP v = crear_vertice(x);
    /*  Se busca el vértice en la lista de vértices del network. */
    VerticeP check = list_search(D->data, v, &comparar_vertice);
    
    if(check) {
        /*  Si el vértice ya existe, se registra como resumidero y se elimina el vértice 
            auxiliar. */
        D->resumidero = check;
        v = destruir_vertice(v);
    } else {
        /*  Si el vértice no existe, se lo agrega a la lista de vértices del network y se
            registra como resumidero. */
        D->data = list_add(D->data, v);
        D->resumidero = v;
    }
}

/*  Se imprime por standard output 
    "Fuente: x"
    , con x la fuente ya fijada a través de FijarFuente y devuelve 0,
    o devuelve -1 en caso de que la fuente no esté fijada. */
int ImprimirFuente(DovahkiinP D) {
    assert(D);
    
    int result = -1;
    
    if(D->fuente) {
        printf("Fuente: %" PRIu64 "\n", D->fuente->nombre);
        result = 0;
    }
    
    return result;
}

/*  Hace lo análogo a ImprimirFuente, pero con el resumidero. */
int ImprimirResumidero(DovahkiinP D) {
    assert(D);
    
    int result = -1;
    
    if(D->resumidero) {
        printf("Fuente: %" PRIu64 "\n", D->resumidero->nombre);
        result = 0;
    }
    
    return result;
}

/*  La siguiente función convierte un string en un número de tipo u64.
    Básicamente, agarra cada uno de los dígitos del string, desde el más
    grande al más pequeño, y multiplica el valor actual del resultado por 10
    y le suma el último dígito leido.
    Por ejemplo, para pasar de "1234" a se debe realizar lo siguiente:
    result := (result * 10) + ('1' - '0')
            = (0 * 10) + 1
            = 0 + 1
            = 1
    result := (result*10) + ('2' - '0')
            = (1*10) + 2
            = 12
    result := 12*10 + 3
            = 123
    result := 123*10 + 4
            = 1234
    */
static uint64_t atoi64(char *s) {
    uint64_t result = 0;
    char c = *s++;
    
    while (c != '\0' && isdigit(c)) {
		result = result * 10  + (c - '0');
		c = *s++;
	}
	
	return result;
}

/*  is_valid chequea que s esté enteramente compuesto por dígitos. */
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

/*  Lee, desde standard output,
    "x y c"
    , representando un lado, con 'x' un vértice, 'y' otro vértice y 'c' la capacidad del 
    vértice, y devuelve un puntero a struct Lado que representa este nuevo lado, o LadoNulo
    en caso de que la linea no sea válida.
    Una línea no es válida si 'x' o 'y' no son un valor de tipo u64. */
Lado LeerUnLado() {
    /*
        La funcion lee una linea desde stdin que representara un lado
        y devuelve el elemento de tipo Lado que represente este lado si
        la linea es valida , o bien LadoNulo si la linea no es valida.
    */
    /*  buffer representa un buffer para leer desde standard input con fgets. */
    char *buffer = calloc(MAX_SIZE, sizeof(uint8_t));
    /*  tokenx, tokeny y tokenx representan los números 'x', 'y' y 'c' no convertidos a u64
        todavía. */
    char *tokenx, *tokeny, *tokenc;
    VerticeP x = NULL;
    VerticeP y = NULL;
    LadoP new = LadoNulo;
    
    if(fgets(buffer, MAX_SIZE, stdin) != NULL) {
        tokenx = strtok(buffer, " ");
        tokeny = strtok(NULL, " ");
        tokenc = strtok(NULL, "\n");
        if(is_valid(tokenx) && is_valid(tokeny) && is_valid(tokenc)){
            /*  Si los valores ingresados son válidos, se deben crear los vértices y el lado
                correspondiente. Aquí no hay problema de que se cree un vértice ya existente,
                pues esto será corregido en CargarUnLado. */
            x = crear_vertice(atoi64(tokenx));
            y = crear_vertice(atoi64(tokeny));
            new = crear_lado(x, y, atoi64(tokenc));
        }
    }
    free(buffer);
    
    return new;
}

/*  Carga en D un lado que fue creado previamente por LeerUnLado. Su flujo inicial es 0. */
int CargarUnLado(DovahkiinP D, LadoP L) {
    assert(D);
    assert(L);
    
    /*  Se deben buscar los vértices a los que asignárseles L en sus listas de vecinos 
        (forward o backward, según corresponda).
        Además, se debe crear el nodo para cada una de las listas a modificar. */
    VerticeP x = (VerticeP)list_search(D->data, L->x, &comparar_vertice);
    VerticeP y = (VerticeP)list_search(D->data, L->y, &comparar_vertice);
    member_t new = member_create(L);
    member_t new_2 = member_create(L);
    int result = 0;
    
    if(x == NULL) {
        /*  Si no se encontró el vértice de salida, se debe agregarlo a la lista de 
            vértices del network. */
        D->data = list_add(D->data, L->x);
    }
    else {
        /*  Si se encontró el vértice de salida, se debe eliminar el vértice de 
            salida que se asignó al lado L al momento de crearlo, y en cambio asignarle
            el vértice encontrado. */
        L->x = destruir_vertice(L->x);
        L->x = x;
    }
    if(y == NULL) {
        /*  Si no se encontró el vértice de llegada, se debe agregarlo a la lista de 
            vértices del network. */
        D->data = list_add(D->data, L->y);
    }
    else {
        /*  Si se encontró el vértice de llegada, se debe eliminar el vértice de 
            llegada que se asignó al lado L al momento de crearlo, y en cambio asignarle
            el vértice encontrado. */
        L->y = destruir_vertice(L->y);
        L->y = y;
    }
    
    L->x->vecinos_forward = list_direct_add(L->x->vecinos_forward, new);
    L->y->vecinos_backward = list_direct_add(L->y->vecinos_backward, new_2);
    result =  1;
    
    return result;
}

/*  Simplemente verifica que la fuente y el resumidero estén fijados. */
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
   
    D->fuente->iteracion = D->iteracion;
    D->temp = list_add(D->temp, D->fuente);
    temp = list_get_first(D->temp);
     
    while(temp && !comparar_vertice(get_content(temp), D->resumidero)) {
        D->temp = add_neighboor_to_list(D->temp, get_content(temp), D->resumidero,  D->iteracion);
        temp = list_next(temp);
    }
    D->corte = list_copy(D->temp);
    free(D->temp);
    D->temp = NULL;
    if(temp && comparar_vertice(get_content(temp), D->resumidero)) {
        /*  Se encontró un camino al resumidero, por lo que D->temp se puede limpiar. */
        result = 1;
        D->corte = list_destroy_keep_members(D->corte);
    }
    else {
        D->flujo_maximal = true;
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
    
    VerticeP vertice_actual = D->resumidero->ancestro, next = NULL;
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

void ImprimirFlujo(DovahkiinP D){
    assert(D);
    
    member_t member_v = list_get_first(D->data);
    VerticeP v_actual = NULL;
    member_t member_l = NULL;
    LadoP lado = NULL;
    
    if (D->flujo_maximal) {
        printf("Flujo Maximal:\n");
    }
    else {
        printf("Flujo (no maximal):\n");
    }
    
    
    while(member_v) {
        v_actual = get_content(member_v);
        member_l = list_get_first(v_actual->vecinos_forward);
        while(member_l) {
            lado = get_content(member_l);
            printf("Lado %" PRIu64 ",%" PRIu64 ": %" PRIu64 "\n", lado->x->nombre, lado->y->nombre, lado->f);
            member_l = list_next(member_l);
        }
        member_v = list_next(member_v);
    }
}

void ImprimirValorFlujo(DovahkiinP D) {
    assert(D);
    
    if (D->flujo_maximal){
        printf("Valor del flujo Maximal: %" PRIu64, D->flujo);
    }
    else {
        printf("Valor del flujo (no maximal): %" PRIu64, D->flujo);
    }
}

void ImprimirCorte(DovahkiinP D) {
    assert(D);
    assert(D->corte);
    
    member_t member = list_get_first(D->corte);
    VerticeP v_actual = NULL;
    u64 capacidad = 0;
    member_t member_l = NULL;
    LadoP lado = NULL;
    
    if(!D->temp) {
        D->temp = list_create();
    }
    
    printf("\nCorte Minimal: S = {");
    
    while(member) {
        v_actual = get_content(member);
        member_l = list_get_first(v_actual->vecinos_forward);
        while(member_l) {
            lado = get_content(member_l);
            if(!list_search(D->corte, lado->y, &comparar_lados)) {
                capacidad += lado->c;
            }
            member_l = list_next(member_l);
        }
        if(comparar_vertice(v_actual, D->fuente)) {
            printf("s,");
        }
        else if(comparar_vertice(v_actual, D->resumidero)) {
            printf("t}\n");
        }
        else {
            printf("%" PRIu64 ",", v_actual->nombre);
        }
        member = list_next(member);
    }
    
    printf("Capacidad: %" PRIu64, capacidad);
    
}

u64 min(u64 a, u64 b) {
    return (a <= b ? a : b);
}
