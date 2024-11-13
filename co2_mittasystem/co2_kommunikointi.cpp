/*
Kommunikaation itse funktiot
*/
#include <Arduino.h>
#include "co2_kommunikointi.hpp"
#include "co2_pinnit.hpp"

/* Huudata latch-signaali niin valot vaihtuu stabiilisti */
void latch(void){
    digitalWrite(LATCH, LOW);
    digitalWrite(LATCH, HIGH);
    digitalWrite(LATCH, LOW);
}

/* Lähetä viesti: 3 8 bit lukuarvoa, älä välitä datan tulkkaamisesta */
void laheta_viesti(viesti_t* viesti){
    shiftOut(SHIFT_DAT, SHIFT_CLK, MSBFIRST, viesti->tavut.tavut_h);
    shiftOut(SHIFT_DAT, SHIFT_CLK, MSBFIRST, viesti->tavut.tavut_m);
    shiftOut(SHIFT_DAT, SHIFT_CLK, MSBFIRST, viesti->tavut.tavut_l);
    latch();
    return;
}

uint16_t tulosta_lukua(uint16_t arvo, viesti_t* viesti, uint16_t aikaa){
    uint16_t aikaa_kulunut = 0;
    while(aikaa_kulunut <= aikaa){
        aikaa_kulunut += tulosta_arvo(arvo, viesti);
    }
    return(aikaa_kulunut);
}

/* Tulosta lukuarvo (0-9999) ruudulle, pidä merkkivalot ennallaan */
unsigned char tulosta_arvo(uint16_t arvo, viesti_t* viesti){
    // Tuhannet jos läsnä
    unsigned char aikaa = 0; // ei loputtomiin
    // Aloita tuhansista
    unsigned char dig_arvo = (unsigned char)(arvo/1000)%10;
    // Ei-nolla, eli jotain lähetettävää (ei esim. 0012 vaan 12)
    if(dig_arvo){
        viesti->kentat.digit_no = DIG_4;
        viesti->kentat.numero = SSEG_NUMERO[dig_arvo];
        laheta_viesti(viesti);
        delay(SSEG_AIKA[dig_arvo]);
        aikaa += SSEG_AIKA[dig_arvo];
        }
    // Sadat
    dig_arvo = (unsigned char)(arvo/100)%10;
    if(dig_arvo or aikaa){
        viesti->kentat.digit_no = DIG_3;
        viesti->kentat.numero = SSEG_NUMERO[dig_arvo];
        laheta_viesti(viesti);
        delay(SSEG_AIKA[dig_arvo]);
        aikaa += SSEG_AIKA[dig_arvo];
        }
    // Kympit
    dig_arvo = (unsigned char)(arvo/10)%10;
    if(dig_arvo or aikaa){
        viesti->kentat.digit_no = DIG_2;
        viesti->kentat.numero = SSEG_NUMERO[dig_arvo];
        laheta_viesti(viesti);
        delay(SSEG_AIKA[dig_arvo]);
        aikaa += SSEG_AIKA[dig_arvo];
        }
    // Ykköset aina: nolla muodossa 0 eikä tyhjänä ruutuna
    dig_arvo = (unsigned char)(arvo)%10;
    viesti->kentat.digit_no = DIG_1;
    viesti->kentat.numero = SSEG_NUMERO[dig_arvo];
    laheta_viesti(viesti);
    delay(SSEG_AIKA[dig_arvo]);
    aikaa += SSEG_AIKA[dig_arvo];
    return(aikaa);
}


/* Tulosta vain valot mutta älä mitään ruudulle */
void tulosta_valot(viesti_t* viesti){
    viesti->kentat.digit_no = DIG_NULL;
    viesti->kentat.numero = SSEG_NULL;
    laheta_viesti(viesti);
    return;
}
