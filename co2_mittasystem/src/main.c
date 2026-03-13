#include "merkkivalot.h"
#include "shiftreg.h"
#include <stdint.h>
#include <util/delay.h>

void main(void)
{
    shiftreg_viesti_t viesti;
    //viesti.tavut.tavut_h = 0xAA;
    //viesti.tavut.tavut_m = 0x12;
    //viesti.tavut.tavut_l = 0x48;
    viesti.kentat.statusvalot = 0xBB;
    viesti.kentat.numero = ~SSEG_5;
    viesti.kentat.digit_no = 0;
    viesti.kentat.valintavalot = 0xF;
    uint16_t arvo  = 1234;
    shiftreg_laheta_viesti(&viesti, 1);
    for(;;)
    {
        tulosta_arvo(arvo, &viesti);
    }
}