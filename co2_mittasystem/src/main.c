#include "merkkivalot.h"
#include "shiftreg.h"
#include "ioexp.h"
#include <stdint.h>
#include <util/delay.h>

extern shiftreg_viesti_t VIESTI_OBJ;
extern volatile sseg_status_t MERKKIVALOT_STATUS;

void main(void)
{
    MERKKIVALOT_STATUS.data = 0x00;
    VIESTI_OBJ.kentat.statusvalot = 0xBB;
    VIESTI_OBJ.kentat.numero = ~SSEG_5;
    VIESTI_OBJ.kentat.digit_no = 0;
    VIESTI_OBJ.kentat.valintavalot = 0xF;
    uint16_t luvut[]  = {1234, 567, 89, 0};
    uint8_t merkkivalot[] = {2<<0, 2<<2, 2<<4, 2<<6};
    shiftreg_laheta_viesti(&VIESTI_OBJ, 1);
    uint8_t luku = 0;
    uint8_t arvo;
    ioexp_porttisuunta(0, 0xFF);
    for(;;)
    {
        VIESTI_OBJ.kentat.statusvalot = merkkivalot[luku];
        shiftreg_laheta_viesti(&VIESTI_OBJ, 1);
        arvo = ioexp_lue(0);
        merkkivalot_tulosta_arvoa(arvo, 100);
        tyhjaa_ruutu();
    }
}
