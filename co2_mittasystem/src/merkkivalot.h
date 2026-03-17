/* Merkkivaloihin (LED, seitsensegmentti) liittyvät jutut */
#ifndef MERKKIVALOT_H
#define MERKKIVALOT_H

#include <stdint.h>
#include "shiftreg.h"


// Segmenttikohtaiset koodit
#define SSEG_A 1<<0
#define SSEG_B 1<<1
#define SSEG_C 1<<2
#define SSEG_D 1<<3
#define SSEG_E 1<<4
#define SSEG_F 1<<5
#define SSEG_G 1<<6
#define SSEG_DP 1<<7

// Lukuarvot segmenttikomboina
#define SSEG_NULL 0xFF
#define SSEG_0   (0xFF^(SSEG_G|SSEG_DP))
#define SSEG_1   (SSEG_B|SSEG_C)
#define SSEG_2   (SSEG_A|SSEG_B|SSEG_G|SSEG_E|SSEG_D)
#define SSEG_3   (SSEG_A|SSEG_B|SSEG_G|SSEG_C|SSEG_D)
#define SSEG_4   (SSEG_F|SSEG_G|SSEG_B|SSEG_C)
#define SSEG_5   (SSEG_A|SSEG_F|SSEG_G|SSEG_C|SSEG_D)
#define SSEG_6   (SSEG_F|SSEG_G|SSEG_E|SSEG_C|SSEG_D)
#define SSEG_7   (SSEG_A|SSEG_B|SSEG_C)
#define SSEG_8   (0xFF^SSEG_DP)
#define SSEG_9   (0xFF^(SSEG_E|SSEG_D|SSEG_DP))


// Monesko numero nelinumeroisessä näytössä aktiivisena
#define DIG_NULL 0x00  // Ei numeroa
#define DIG_4   (1<<0) // Tuhannet
#define DIG_3   (1<<1) // Sadat
#define DIG_2   (1<<2) // Kymmenet
#define DIG_1   (1<<3) // Yhdet

/* Statusvalojen händläämiseen */
#define STATUS_CO2_NOK          1<<0
#define STATUS_CO2_OK           1<<1
#define STATUS_ILMANKOSTEUS_NOK 1<<2
#define STATUS_ILMANKOSTEUS_OK  1<<3
#define STATUS_LAMPOTILA_NOK    1<<4
#define STATUS_LAMPOTILA_OK     1<<5

/*
8 bit mittastatus, ts. onko arvo ok vai ei ok vai jotain siltä väliltä
*/
typedef union {
    struct status {
        uint8_t co2_nok : 1;
        uint8_t co2_ok : 1;
        uint8_t ilmankosteus_nok : 1;
        uint8_t ilmankosteus_ok : 1;
        uint8_t lampotila_nok : 1;
        uint8_t lampotila_ok : 1;
        uint8_t ctrl_nok : 1;
        uint8_t ctrl_ok : 1; // msb
    } status;
    uint8_t data; // 8bit repr
} mittastatus_t;


/* Valintavalon händläämiseen */
typedef enum Valinnat {
    VALINTA_NULL = 0,
    VALINTA_CO2 = 1<<0,
    VALINTA_ILMANKOSTEUS = 1<<1,
    VALINTA_LAMPOTILA = 1<<2
} valinta_t;

/* Lukuarvon tulostamiseen */
typedef union {
    struct status_kentat {
        uint8_t edellinen : 1;
        uint8_t digit     : 2;
        uint8_t nayta     : 1;
        uint8_t numero    : 4;
    }status_kentat;
    uint8_t data;
}sseg_status_t;

/* Tee pilkottu versio numeroarvosta */
void merkkivalot_pilko_numero(uint16_t arvo);

/* Tulosta valot ja annettu lukuarvo (0-9999) ruudulle.
Lukee arvon `merkkivalot_pilko_numero` tuloksesta. */
void merkkivalot_tulosta_arvo(void);

void merkkivalot_tulosta_arvoa(uint16_t arvo, uint16_t aikaa);

/* Tyhjennä seitsensegmenttiruutu lukuarvoista */
void tyhjaa_ruutu();


#endif // MERKKIVALOT_H