#ifndef MITTATULOS_H
#define MITTATULOS_H
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
#endif // MITTATULOS_H