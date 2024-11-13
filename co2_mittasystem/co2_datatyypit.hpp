/*
Structit sun muut datatyypit ja tän tällaiset headerissa,
koska alkoi olla inosssa täyttä...
*/
#ifndef SIENIMITTARI_TYYPIT
#define SIENIMITTARI_TYYPIT

#include <stdint.h>

#define SSEG_A 1<<0
#define SSEG_B 1<<1
#define SSEG_C 1<<2
#define SSEG_D 1<<3
#define SSEG_E 1<<4
#define SSEG_F 1<<5
#define SSEG_G 1<<6
#define SSEG_DP 1<<7 // Ei käytössä, kaapattu valintavaloksi

#define SSEG_NULL 0xFF
#define SSEG_0 (0xFF^(SSEG_G|SSEG_DP))
#define SSEG_1 (SSEG_B|SSEG_C)
#define SSEG_2 (SSEG_A|SSEG_B|SSEG_G|SSEG_E|SSEG_D)
#define SSEG_3 (SSEG_A|SSEG_B|SSEG_G|SSEG_C|SSEG_D)
#define SSEG_4 (SSEG_F|SSEG_G|SSEG_B|SSEG_C)
#define SSEG_5 (SSEG_A|SSEG_F|SSEG_G|SSEG_C|SSEG_D)
#define SSEG_6 (SSEG_F|SSEG_G|SSEG_E|SSEG_C|SSEG_D)
#define SSEG_7 (SSEG_A|SSEG_B|SSEG_C)
#define SSEG_8 (0xFF^SSEG_DP)
#define SSEG_9 (0xFF^(SSEG_E|SSEG_D|SSEG_DP))


// Mäppää seitsensegmenttinumerot, e.g.
// SSEG_NUMERO[3] palauttaa SSEG_3
static unsigned char SSEG_NUMERO[] = {
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


// Aktiivisten segmenttien määrä (annetaan armoa hastaville)
static unsigned char SSEG_AIKA[] = {
  9, // 0 (6)
  4, // 1 (2)
  7, // 2 (5)
  5, // 3 (2)
  5, // 4 (4)
  7, // 5 (5)
  7, // 6 (5)
  5, // 7 (3)
  10, // 8 (7)
  7, // 9 (5)
};



/*
8 bit mittastatus, ts. onko arvo ok vai ei ok vai jotain siltä väliltä
*/
typedef union {
    struct status {
        unsigned int co2_nok : 1;
        unsigned int co2_ok  : 1;
        unsigned int ilmankosteus_nok : 1;
        unsigned int ilmankosteus_ok  : 1;
        unsigned int lampotila_nok : 1;
        unsigned int lampotila_ok  : 1;
        unsigned int _rsv : 2; // msb
    }status;
    uint8_t data; // 8bit repr
}mittastatus_t;


/*
32 bit mittaustulos: onko tilanne ok ja millä arvoilla
Statusflageihin menee aina 6 bittiä (3x nok/ok/mid) mutta varattu 8
CO2 jossain tuhansien alueella, ei ainakaan pitäisi mennä yli 4000 niin 12 bit
Ilmankosteus 0-100 niin 7 bit (0-127)
Lämpötilalle riittää 5 (0-31)
*/
typedef struct mittatulos_t {
  mittastatus_t mstatus;
  uint16_t co2 : 12;
  uint16_t ilmankosteus : 7;
  uint16_t lampotila : 5;
}mittatulos_t;


/* 64 bit */
typedef struct rajat_t {
    /* Ylä- ja alarajat */
    // Hiilidioksidi
    uint16_t co2_raja_huono : 13;
    uint16_t co2_raja_hyva : 13;
    // Ilmankosteus
    uint16_t ilmankosteus_raja_huono : 7;
    uint16_t ilmankosteus_raja_hyva : 7;
    // Lämpötila
    uint16_t lampotila_raja_huono : 6;
    uint16_t lampotila_raja_hyva : 6;
}rajat_t;


/* Yksittäinen viesti joka menee kahteen siirtorekisteriin, yht. 24 bit */
/* ks. https://stackoverflow.com/a/16421577 */
typedef union {
    struct kentat {
        unsigned char statusvalot : 8;   //LSB
        unsigned char numero : 8;
        unsigned char digit_no : 4;
        unsigned char valintavalot : 4;  // MSB
    }kentat;       // Kenttä kerrallaan handlaaminen
    struct tavut {
        unsigned char tavut_l;
        unsigned char tavut_m;
        unsigned char tavut_h;
    }tavut;
}viesti_t;


/* Valintavalon händläämiseen */
typedef enum Valinnat {
    VALINTA_NULL = 0,
    VALINTA_CO2 = 1<<0,
    VALINTA_ILMANKOSTEUS = 1<<1,
    VALINTA_LAMPOTILA = 1<<2
}valinta_t;


/* Statusvalojen händläämiseen */
enum Statukset {
    STATUS_CO2_NOK = 1<<0,
    STATUS_CO2_OK = 1<<1,
    STATUS_ILMANKOSTEUS_NOK = 1<<2,
    STATUS_ILMANKOSTEUS_OK = 1<<3,
    STATUS_LAMPOTILA_NOK = 1<<4,
    STATUS_LAMPOTILA_OK = 1<<5
};


// Funktioprototyypit
//-------------------

// Luomis- ja tuhoamisfunktiot
viesti_t* uusi_viesti(void);
void tuhoa_viesti(viesti_t*);
mittatulos_t* uusi_mittatulos(void);
void tuhoa_mittatulos(mittatulos_t*);
rajat_t* uudet_rajat(void);
void tuhoa_rajat(rajat_t*);

void paivita_mittatulos(mittatulos_t*, uint8_t*);
void paivita_mittastatus(mittatulos_t*, rajat_t*);
void paivita_valot_viestiin(mittatulos_t*, viesti_t*);

#endif
