#include "API.h"

int main(void) {

    DovahkiinP dragon = NULL;
    Lado lado;
    
    dragon = NuevoDovahkiin();
    lado = LeerUnLado();
    
    while(lado != LadoNulo) {
        CargarUnLado(dragon, lado);
        lado = LeerUnLado();
    }
    FijarFuente(dragon, (u64)0);
    FijarResumidero(dragon, (u64)1);
    while(ActualizarDistancias(dragon)) {
        if(BusquedaCaminoAumentante(dragon)) {
            AumentarFlujo(dragon);
        }
    }
    //DestruirDovahkiin(dragon);
    
    return 1;
}
