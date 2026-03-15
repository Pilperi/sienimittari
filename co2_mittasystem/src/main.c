#include "merkkivalot.h"
#include "shiftreg.h"
#include <stdint.h>
#include <util/delay.h>

extern shiftreg_viesti_t VIESTI_OBJ;

void main(void)
{
    VIESTI_OBJ.kentat.statusvalot = 0xBB;
    VIESTI_OBJ.kentat.numero = ~SSEG_5;
    VIESTI_OBJ.kentat.digit_no = 0;
    VIESTI_OBJ.kentat.valintavalot = 0xF;
    uint16_t luvut[]  = {1234, 567, 89, 0};
    uint8_t merkkivalot[] = {2<<0, 2<<2, 2<<4, 2<<6};
    shiftreg_laheta_viesti(&VIESTI_OBJ, 1);
    uint8_t luku = 0;
    for(;;)
    {
        VIESTI_OBJ.kentat.statusvalot = merkkivalot[luku];
        shiftreg_laheta_viesti(&VIESTI_OBJ, 1);
        for(uint16_t cnt=100; cnt; cnt--)
        {
            tulosta_arvo(luvut[luku]);
        }
        tyhjaa_ruutu();
        _delay_ms(500);
        luku++;
        if (luku > 3)
        {
            luku = 0;
        }
    }
}