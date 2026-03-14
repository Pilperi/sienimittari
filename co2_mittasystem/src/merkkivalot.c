#include <stdint.h>
#include <avr/pgmspace.h> 
#include "merkkivalot.h"
#include "shiftreg.h"
#include "timing.h"

extern uint8_t TIMER_COUNT;

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
  200,
  200,
  200,
  200,
  200,
  200,
  200,
  200,
  200,
  200,
};
const uint8_t SSEG_AJAT_POIS[10] = {
  200,
  200,
  200,
  200,
  200,
  200,
  200,
  200,
  200,
  200,
};

/* Tulosta lukuarvo (0-9999) ruudulle, pidä merkkivalot ennallaan */
void tulosta_arvo(uint16_t arvo, shiftreg_viesti_t* viesti_p){
    const uint16_t kymmenet[3] = {1000, 100, 10};
    uint8_t laskuri;
    uint8_t luku_aiemmin = 0;
    viesti_p->tavut.tavut_l = 0;
    viesti_p->tavut.tavut_m = 0;
    viesti_p->tavut.tavut_h = 0;
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
            viesti_p->kentat.digit_no = 1<<dig;
            viesti_p->kentat.numero = SSEG_NUMERO[laskuri];
            shiftreg_laheta_viesti(viesti_p, 0);
            luku_aiemmin |= 1;
        }
        TIMER_COUNT = 1;
        timing_delay_set(100, 100);
        while (TIMER_COUNT)
        {
            __asm__("sei");
            __asm__("sleep");
        }
        tyhjaa_ruutu(viesti_p);
    }
    // Yhdet enää jäljellä, näytetään aina
    viesti_p->kentat.digit_no = 1<<3;
    viesti_p->kentat.numero = SSEG_NUMERO[(uint8_t)arvo];
    shiftreg_laheta_viesti(viesti_p, 0);
    TIMER_COUNT = 1;
    timing_delay_set(100, 100);
    while (TIMER_COUNT)
    {
        __asm__("sei");
        __asm__("sleep");
    }
    tyhjaa_ruutu(viesti_p);
}

void tyhjaa_ruutu(shiftreg_viesti_t* viesti){
    viesti->kentat.statusvalot = 0x00;
    viesti->kentat.numero = SSEG_NULL;
    viesti->kentat.digit_no = DIG_NULL;
    viesti->kentat.valintavalot = 0;
    shiftreg_laheta_viesti(viesti, 0);
}
