#include <stdint.h>
#include <avr/pgmspace.h> 
#include "merkkivalot.h"
#include "shiftreg.h"
#include "timing.h"
#include <util/delay.h>

// Toistojen määrä ajastuksessa, että saadaan järkevän kokoisia aikoja
extern volatile uint16_t TOISTOJA;
extern uint16_t TOISTOJA_REF;

// Yksi ainoa viestiobjekti
shiftreg_viesti_t VIESTI_OBJ;

// Yksi ainoa status
volatile sseg_status_t MERKKIVALOT_STATUS;

// Viestin tuhannet, sadat, kymmenet, ykköset
uint8_t VIESTI_NUMEROT[4];

// Numerot 0-9 seitsensegmentteinä, esim.
// SSEG_NUMERO[3] palauttaa SSEG_3
static const uint8_t SSEG_NUMERO[10] = {
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

/* Pilko lukuarvo tuhansiin, satoihin, kymmeniin, yksiin
ja aseta globaaliin muuttujaan VIESTI_NUMEROT. */
void merkkivalot_pilko_numero(uint16_t arvo)
{
    const uint16_t kymmenet[3] = {1000, 100, 10};
    uint8_t laskuri;
    // Tuhannet, sadat, kymmenet
    for (register uint8_t dig=0; dig < 3; dig++)
    {
        laskuri = 0;
        while (arvo > kymmenet[dig])
        {
            laskuri++;
            arvo -= kymmenet[dig];
        }
        VIESTI_NUMEROT[dig] = laskuri;
    }
    // Yhdet enää jäljellä arvossa, muu miinustettu pois
    VIESTI_NUMEROT[3] = (uint8_t)arvo;
    MERKKIVALOT_STATUS.status_kentat.edellinen = 0;
    MERKKIVALOT_STATUS.status_kentat.digit = 0;
    MERKKIVALOT_STATUS.status_kentat.numero = VIESTI_NUMEROT[0];
}


/* Tulosta lukuarvo (0-9999) ruudulle, pidä merkkivalot ennallaan */
void merkkivalot_tulosta_arvo(void){
    //if(TOISTOJA){return;}
    //TOISTOJA = TOISTOJA_REF;
    if(MERKKIVALOT_STATUS.status_kentat.nayta)
    {
        // Ei näytetä alun nollia, mutta näytetään aina ykkösten arvo
        if(
            MERKKIVALOT_STATUS.status_kentat.edellinen
            | MERKKIVALOT_STATUS.status_kentat.numero
            | MERKKIVALOT_STATUS.status_kentat.digit==3
        )
        {
            VIESTI_OBJ.kentat.digit_no = 1<<MERKKIVALOT_STATUS.status_kentat.digit;
            VIESTI_OBJ.kentat.numero = SSEG_NUMERO[MERKKIVALOT_STATUS.status_kentat.numero];
            MERKKIVALOT_STATUS.status_kentat.edellinen |= 1;
            shiftreg_laheta_viesti(&VIESTI_OBJ, 0);
        }
        else
        {
                VIESTI_OBJ.kentat.numero = SSEG_NULL;
        }
        MERKKIVALOT_STATUS.status_kentat.digit++;
        if(!MERKKIVALOT_STATUS.status_kentat.digit)
        {
            MERKKIVALOT_STATUS.status_kentat.edellinen = 0;
        }
        MERKKIVALOT_STATUS.status_kentat.numero = VIESTI_NUMEROT[MERKKIVALOT_STATUS.status_kentat.digit];
    }
}


void tyhjaa_ruutu(void){
    VIESTI_OBJ.kentat.statusvalot = 0x00;
    VIESTI_OBJ.kentat.numero = SSEG_NULL;
    VIESTI_OBJ.kentat.digit_no = DIG_NULL;
    VIESTI_OBJ.kentat.valintavalot = 0;
    shiftreg_laheta_viesti(&VIESTI_OBJ, 0);
}

void merkkivalot_tulosta_arvoa(uint16_t arvo, uint16_t aikaa)
{
    merkkivalot_pilko_numero(arvo);
    timing_delay_set(100,400);
    MERKKIVALOT_STATUS.status_kentat.nayta = 1;
    while(--aikaa)
    {
        //while(MERKKIVALOT_STATUS.status_kentat.digit < 4)
        for(uint8_t laskuri=4; laskuri; --laskuri)
        {
            merkkivalot_tulosta_arvo();
            while(--TOISTOJA)
            {
                __asm__("sleep");
            }
            TOISTOJA = TOISTOJA_REF;
        }
    }
    timing_delay_unset();
    MERKKIVALOT_STATUS.status_kentat.nayta = 0;
}
