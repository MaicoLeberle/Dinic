#include "API.h"

int main(void) {

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
    while(ActualizarDistancias(D)) {
        while(BusquedaCaminoAumentante(D)) {
            AumentarFlujo(D);
        }
    }
    ImprimirValorFlujo(dragon);
    ImprimirCorte(dragon);
    DestruirDovahkiin(D);
    
    return 1;
}
