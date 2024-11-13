/*
Kommunikointifunktiot
*/
#include <Arduino.h>
#include "co2_datatyypit.hpp"
#include <stdint.h>


enum DIG_VALINNAT {
    DIG_NULL = 0x00,
    DIG_1 = (1<<0),
    DIG_2 = (1<<1),
    DIG_3 = (1<<2),
    DIG_4 = (1<<3)
};


/* Lähetä viesti: 3 8 bit lukuarvoa merkkivaloihin ja ruudulle */
void laheta_viesti(viesti_t*);
/* Tulosta vain valot, älä laita mitään ruudulle */
void tulosta_valot(viesti_t*);
/* Tulosta valot ja annettu lukuarvo (0-9999) ruudulle. Kerro paljon meni aikaa */
unsigned char tulosta_arvo(uint16_t, viesti_t*);
/* Tulosta lukua n ms */
uint16_t tulosta_lukua(uint16_t, viesti_t*, uint16_t);
/* Latchaa signaalivalot */
void latch(void);
