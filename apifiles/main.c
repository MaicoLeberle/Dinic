#include "API.h"

int main(void) {
    DovahkiinP dragon = NULL;
    LadoP lado = NULL;
    
    dragon = NuevoDovahkiin();
    lado = LeerUnLado();
    CargarUnLado(dragon, lado);
    
    return 1;
}
