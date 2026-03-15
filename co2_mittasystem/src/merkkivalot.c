#include <stdint.h>
#include <avr/pgmspace.h> 
#include "merkkivalot.h"
#include "shiftreg.h"
#include "timing.h"
#include <util/delay.h>

shiftreg_viesti_t VIESTI_OBJ;

// Numerot 0-9 seitsensegmentteinä, esim.
// SSEG_NUMERO[3] palauttaa SSEG_3
const uint8_t SSEG_NUMERO[10] = {
  ~SSEG_0,
  ~SSEG_1,
  ~SSEG_2,
  ~SSEG_3,
  ~SSEG_4,
  ~SSEG_5,
  ~SSEG_6,
  ~SSEG_7,
  ~SSEG_8,
  ~SSEG_9
};
const uint8_t SSEG_AJAT_PAALLA[10] = {
  100,
  100,
  100,
  100,
  100,
  100,
  100,
  100,
  100,
  100,
};
/* Tulosta lukuarvo (0-9999) ruudulle, pidä merkkivalot ennallaan */
void tulosta_arvo(uint16_t arvo){
    const uint16_t kymmenet[3] = {1000, 100, 10};
    uint8_t laskuri;
    uint8_t luku_aiemmin = 0;
    VIESTI_OBJ.tavut.tavut_l = 0;
    VIESTI_OBJ.tavut.tavut_m = 0;
    VIESTI_OBJ.tavut.tavut_h = 0;
    // Tuhannet, sadat, kymmenet
    for (register uint8_t dig=0; dig < 3; dig++)
    {
        laskuri = 0;
        while (arvo > kymmenet[dig])
        {
            laskuri++;
            arvo -= kymmenet[dig];
        }
        if(laskuri | luku_aiemmin)
        {
            VIESTI_OBJ.kentat.digit_no = 1<<dig;
            VIESTI_OBJ.kentat.numero = SSEG_NUMERO[laskuri];
            luku_aiemmin |= 1;
        }
        else
        {
            VIESTI_OBJ.kentat.digit_no = 0;
            VIESTI_OBJ.kentat.numero = 0x00;
        }
        shiftreg_laheta_viesti(&VIESTI_OBJ, 0);
        timing_delay_set(SSEG_AJAT_PAALLA[laskuri], 700);
        
    }
    // Yhdet enää jäljellä, näytetään aina
    VIESTI_OBJ.kentat.digit_no = 1<<3;
    VIESTI_OBJ.kentat.numero = SSEG_NUMERO[(uint8_t)arvo];
    shiftreg_laheta_viesti(&VIESTI_OBJ, 0);
    timing_delay_set(SSEG_AJAT_PAALLA[laskuri], 700);
}

void tyhjaa_ruutu(void){
    VIESTI_OBJ.kentat.statusvalot = 0x00;
    VIESTI_OBJ.kentat.numero = SSEG_NULL;
    VIESTI_OBJ.kentat.digit_no = DIG_NULL;
    VIESTI_OBJ.kentat.valintavalot = 0;
    shiftreg_laheta_viesti(&VIESTI_OBJ, 0);
}
