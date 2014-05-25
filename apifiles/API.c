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
        new->FF_DFS = NULL;
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
    if(D->FF_DFS) { 
        /*  Sólo es necesario destruir la lista, pues los lados ya se han eliminado al
            destruir D->data. */        
        D->FF_DFS = list_destroy_keep_members(D->FF_DFS);       
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
    assert(D);
 
    member_t temp = NULL;
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
    if(temp && comparar_vertice(get_content(temp), D->resumidero)) {
        /*  Se encontró un camino al resumidero, por lo que D->temp se puede limpiar. */
        result = 1;
        D->temp = list_destroy_keep_members(D->temp);
    }
    return result;
}


/*  La siguiente función auxiliar sirve para implementar DFS desde la fuente respetando
    los niveles de BFS establecidos al llamar a ActualizarDistancias(). 
    Devuelve true si se llega al rseumidero y false si no se llega. */
bool BusquedaCaminoAumentanteAux(VerticeP vertice, list_t camino_inicial, VerticeP resumidero, u64P flujo, unsigned int iteracion) {
    /*  Primero, se consideran los vecinos forward. */
    member_t temp = list_get_first(vertice->vecinos_forward);
    Lado lado;
    FF_DFSLadoP lado_y_direccion;
    u64 min_flujo;

    while(temp) {
        /*  Se calcula el flujo que se debería enviar si se elige el lado lado. */
        min_flujo = min(*flujo, lado->c - lado->f);
        lado = (Lado)(get_content(temp));
        if((lado->y)->iteracion != iteracion || (lado->y)->distancia > resumidero->distancia || (lado->y)->distancia != vertice->distancia + 1 || !min_flujo) {
            /*  Si la iteracion que estableció la distancia desde la fuente al vértice y (del lado xy, representado en lado) no es
                la última iteración, entonces la distancia de y es considerada como inválida.
                Si la distancia del vértice y es mayor que la distancia del resumidero, entonces no hay forma de continuar buscando 
                caminos al resumidero respetando los niveles de BFS.
                Por otro lado, si la nivel(y) != nivel(x) + 1, entonces este no es un vértice válido y se debe descartar su 
                subsiguiente análisis. 
                Por último, si el flujo a mandar es 0, entonces el lado está saturado. */
            continue;
        } else {
            lado_y_direccion = crear_FF_DFSLado(lado, true);
            /*  Es un vértice válido para continuar el análisis. */
            if(comparar_vertice(lado->y, resumidero)) {
                /*  Se llegó al resumidero. Se debe agregar el último lado de este camino y actualizar el flujo que se debería mandar
                    al llamar a AumentarFlujo o AumentarFlujoYTambienImprimirCamino. */
                list_add(camino_inicial, lado_y_direccion);
                (*flujo) = min_flujo;
                return true;
            } else {
                /*  Se debe seguir buscando. */
                (*flujo) = min_flujo;
                if(!BusquedaCaminoAumentanteAux(lado->y, list_add(camino_inicial, lado_y_direccion), resumidero, flujo, iteracion)) {
                    /*  No se llega al resumidero a través de y. Se debe eliminar el último lado agregado a la lista del camino 
                        al llamar a list_add(camino_inicial, lado). */
                    /*  ¿Se debe hacer un destruir_vertice()? ¿Esto no elimina el lado guardado en D->data? */
                    remove_last(camino_inicial, &destruir_FF_DFSLado);
                    /*  Pasar a considerar el siguiente vecino forward de vertice. */
                    temp = list_next(temp);
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
    temp = list_get_first(vertice->vecinos_backward);
    while(temp) {
        /*  Se calcula el flujo que se debería enviar si se elige el lado lado. */
        min_flujo = min(*flujo, lado->f);
        lado = (Lado)(get_content(temp));
        if((lado->x)->iteracion != iteracion || (lado->x)->distancia > resumidero->distancia || (lado->x)->distancia != vertice->distancia + 1 || !min_flujo) {
            /*  Si la iteracion que estableció la distancia desde la fuente al vértice x (del lado xy, representado en lado) no es
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
            lado_y_direccion = crear_FF_DFSLado(lado, false);
            (*flujo) = min_flujo;
            if(!BusquedaCaminoAumentanteAux(lado->x, list_add(camino_inicial, lado_y_direccion), resumidero, flujo, iteracion)) {

                /*  No se llega al resumidero a través de x. Se debe eliminar el último lado agregado a la lista del camino 
                    al llamar a list_add(camino_inicial, lado). */
                /*  ¿Se debe hacer un destruir_vertice()? ¿Esto no elimina el lado guardado en D->data? */
                remove_last(camino_inicial, &destruir_FF_DFSLado);
                /*  Pasar a considerar el siguiente vecino forward de vertice. */
                temp = list_next(temp);
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



/*  La siguiente implementación de BusquedaCaminoAumentante va guardando en D->FF_DFS 
    todas las aristas involucradas en un camino de s a t. */
int BusquedaCaminoAumentante(DovahkiinP D) {
    /*  Maico: ¿Se debería chequear que los caminos no vuelven eventualmente a la fuente? Porque, al
        momento de cargar los lados, no se verifica que el vértice de llegada no sea la fuente.  */
    member_t vecinos_de_s;
    Lado lado;
    FF_DFSLadoP lado_y_direccion;


    if(D->FF_DFS) {
        D->FF_DFS = list_destroy_keep_members(D->FF_DFS);
    }
    D->FF_DFS = list_create();
    /*  Se debe iterar sobre todos los vecinos de la fuente para ver si se puede llegar al resumidero. */
    vecinos_de_s = list_get_first((D->fuente)->vecinos_forward);

    while(vecinos_de_s) {
        /*  Se agrega el lado forward a D->FF_DFS y se procede a ver si a partir de aquí se puede llegar al resumidero. */
        lado = (Lado)(get_content(vecinos_de_s));
        lado_y_direccion = crear_FF_DFSLado(lado, true);
        D->FF_DFS = list_add(D->FF_DFS, lado_y_direccion);
        /*  El flujo desde el cual se debe comenzar el análisis es el flujo restante a enviar a través de este lado
            desde la fuente al vértice (vecinos_de_s->member)->y. */
        //PY : Eu estas pisando el D->flujo , no guardas los flujos anteriores. Me equivoco ?
        //ademas que se calcula aca ? Si tenemos x y 30 y ya se mando un flujo de 15 entonces
        //la capacidad vale 15 y el flujo 15 tambien , 15-15 = 0.
        /*  Maico : No, la capacidad vale 30 (no cambia) y el flujo 15, por lo que D->flujo := 30 - 15 = 0
            Ademas, se pisa el valor de D->flujo, pues D->flujo indica el flujo que se debe enviar en la proxima
            llamada a AumentarFlujo, y ahora se está intentando ver para lado_correspondiente en particular si se
            puede llegar al resumidero (o sea, en el paso i-esimo se puede ver que los primeros (i-1) lado_correspondientes
            no llegan al resumidero, y todavia no se analizo el caso para los restantes lado_correspondientes, por 
            lo que no hay conflicto a la hora de pisar D->flujo). */
        D->flujo = lado->c - lado->f;
        /*  Se debe pasar como parámetro la iteración actual que estableció nivel(x) de cada vértice visitado. */
        if(BusquedaCaminoAumentanteAux(lado->y, D->FF_DFS, D->resumidero, &(D->flujo), D->iteracion)) {
            /*  Se llega al resumidero. */
            return 1;
        } else {
            /*  Si no se llega al resumidero desde (vecinos_de_s->member)->y, entonces se debe eliminar el último
                lado agregado a la lista que está marcando el camino de la fuente al resumidero. */
            remove_last(D->FF_DFS, &destruir_FF_DFSLado);
        }
        /*  Se debe analizar a continuación el siguiente vecino forward de la fuente. */
        vecinos_de_s = list_next(vecinos_de_s);
    }
    return 0;
}


u64 AumentarFlujoAux(DovahkiinP D, bool imprimir) {
    //PY : No me parece una buena idea checkear esto en el if (todavia no se bien quien llama a esta
    //funcion entonces no toco el codigo). Pero si se limpia la lista (y no se inicializa D->Dff_DFS en NULL)
    //entonces podriamos tener una corrupcion de memoria o un segmentation fault no se 
    //(NOOOOOOOOOOOOOOO :'(((((((((( )
    if(D->FF_DFS && !list_empty(D->FF_DFS)) {
        /*  BusquedaCaminoAumentante encontró un camino de la fuente al resumidero y lo almacenó en D->FF_DFS.
            Se procede a actualizar el flujo en los lados involucrados.
            Se empieza desde el último lado guardado porque se pide el imprimir desde t a s. */

        /*  Verifico que efectivamente se ha encontrado un flujo distinto de 0 para enviar. */
        assert(D->flujo);

        member_t temp = list_get_last(D->FF_DFS);
        FF_DFSLadoP lado_y_direccion;
        while(temp) {
            lado_y_direccion = (FF_DFSLadoP)(get_content(temp));
            if(lado_y_direccion->is_forward)
                (lado_y_direccion->lado)->f = (lado_y_direccion->lado)->f + D->flujo;
            else
                (lado_y_direccion->lado)->f = (lado_y_direccion->lado)->f - D->flujo;
            if(imprimir)
                printf("%" PRIu64 ";", ((lado_y_direccion->lado)->y)->nombre);
            temp = list_next(temp);
        }
        /*  Se debe resetear el flujo a enviar y limpiar la lista D->FF_DFS. */
        D->flujo = 0;
        D->FF_DFS = list_destroy(D->FF_DFS, &destruir_FF_DFSLado);
        return 1;
    } else {
        /*  No se ha hecho una llamada a BusquedaCaminoAumentante que haya establecido un camino de la fuente
            al resumidero en D->FF_DFS, o ya se ha enviado flujo por un tal camino aumentante. */
        return 0;
    }
}


u64 AumentarFlujo(DovahkiinP D) {
    return AumentarFlujoAux(D, false);
}


u64 AumentarFlujoYTambienImprimirCamino(DovahkiinP D) {
    return AumentarFlujoAux(D, true);
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
        printf("Valor del flujo Maximal: \t%" PRIu64, f);
    }else{
        printf("Valor del flujo (no maximal):\t%" PRIu64, f);
    }
}


/*  Precondición: Se corrió ActualizarDistancia y no se llegó al resumidero. El corte minimal
    está en D->temp. */
void ImprimirCorte(DovahkiinP D) {
    member_t temp = list_get_first(D->temp);
    printf("Corte Minimal:\tS = {s");
    while(temp) {
        printf(",%"PRIu64, ((VerticeP)(get_content(temp)))->nombre);
    }
    printf("\nCapacidad:\t%" PRIu64, ValorFlujo(D));
}

u64 min(u64 a, u64 b) {
    return (a <= b ? a : b);
}
