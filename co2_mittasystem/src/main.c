#include "merkkivalot.h"
#include "shiftreg.h"
#include <stdint.h>
#include <util/delay.h>

void main(void)
{
    shiftreg_viesti_t viesti;
    viesti.kentat.statusvalot = 0xBB;
    viesti.kentat.numero = ~SSEG_5;
    viesti.kentat.digit_no = 0;
    viesti.kentat.valintavalot = 0xF;
    uint16_t luvut[]  = {1234, 123, 12, 1};
    uint8_t merkkivalot[] = {2<<0, 2<<2, 2<<4, 2<<6};
    shiftreg_laheta_viesti(&viesti, 1);
    uint8_t luku = 0;
    for(;;)
    {
        viesti.kentat.statusvalot = merkkivalot[luku];
        shiftreg_laheta_viesti(&viesti, 1);
        for(uint16_t cnt=0x00FF; cnt; cnt--)
        {
            tulosta_arvo(1234, &viesti);
        }
        tyhjaa_ruutu(&viesti);
        _delay_ms(500);
        if (luku++ > 3)
        {
            luku = 0;
        }
    }
}