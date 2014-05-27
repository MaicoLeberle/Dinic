#include <string.h>
#include "../apifiles/API.h"


/*  Las opciones de entrada aceptadas son:
    -ifmax      : Imprime el flujo maximal al finalizar. Por default no se hace.
    -icaminos   : Imprime los caminos aumentantes a medida que va enviando flujo. Por default no se hace.
    -icorte     : Imprime el corte minimal una vez conseguido. */

int main(int argc, char* argv[]) {
    bool imprimir_flujo_maximal = false;
    bool imprimir_caminos_aumentantes = false;
    bool imprimir_corte = false;
    while(argc > 1) {
        if(strcmp(argv[1], "-ifmax") == 0) {    
            imprimir_flujo_maximal = true;
        } else if (strcmp(argv[1], "-icaminos") == 0) {
            imprimir_caminos_aumentantes = true;
        } else if (strcmp(argv[1], "-icorte") == 0) {
            imprimir_corte = true;
        }
        argc--;
        argv++;
    }
    DovahkiinP D = NULL;
    Lado lado;
    
    D = NuevoDovahkiin();
    lado = LeerUnLado();
    
    while(lado != LadoNulo) {
        CargarUnLado(D, lado);
        lado = LeerUnLado();
    }
    FijarFuente(D, (u64)0);
    FijarResumidero(D, (u64)1);
    if(imprimir_caminos_aumentantes) {
        while(ActualizarDistancias(D)) {
            while(BusquedaCaminoAumentante(D)) {
                AumentarFlujoYTambienImprimirCamino(D);
            }   
        }
    } else {
        while(ActualizarDistancias(D)) {
            while(BusquedaCaminoAumentante(D)) {
                AumentarFlujo(D);
            }
        }
    }
    if(imprimir_flujo_maximal) {
        ImprimirFlujo(D);
    }
    if(imprimir_corte) {
        ImprimirCorte(D);
    }
    printf("\n");
    ImprimirValorFlujo(D);
    printf("\n");
    DestruirDovahkiin(D);
    
    return 1;
}
