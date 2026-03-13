/*
Kommunikointifunktiot
*/
#ifndef CO2_SHIFTREG_H
#define CO2_SHIFTREG_H

#include <stdint.h>
#include <avr/io.h>

/* ATtiny85 */
#if defined (__AVR_ATtiny85__)
    #define SHIFTREG_LATCH_DDR  DDRB
    #define SHIFTREG_LATCH_PORT PORTB
    #define SHIFTREG_LATCH_PIN  PINB1
    #define SHIFTREG_TWI_DDR    DDRB
    #define SHIFTREG_TWI_PORT   PORTB
    #define SHIFTREG_PIN_DO     PINB0
    #define SHIFTREG_PIN_USCK   PINB2
#else
    #error "Ei-tuettu arkkitehtuuri!"
#endif

/* Yksittäinen viesti joka menee kolmeen siirtorekisteriin, yht. 24 bit */
/* ks. https://stackoverflow.com/a/16421577 */
typedef union {
    struct kentat {
        uint8_t statusvalot  : 8;   //LSB
        uint8_t numero       : 8;
        uint8_t digit_no     : 4;
        uint8_t valintavalot : 4;  // MSB
    } kentat;       // Kenttä kerrallaan handlaaminen
    struct tavut {
        uint8_t tavut_l;
        uint8_t tavut_m;
        uint8_t tavut_h;
    } tavut;
} shiftreg_viesti_t;

/* Lähetä kahdeksan bittiä dataa.
Latchaa lopuksi jos niin sanotaan. */
void shiftreg_laheta_viesti(shiftreg_viesti_t* viesti_p, uint8_t latch);


#endif // CO2_SHIFTREG_H
