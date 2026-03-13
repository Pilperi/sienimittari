/* Rautatason yksikkötestit */

#include "co2_yksikkotestit.hpp"

/* Testaa että kaikki statusvalot toimii */
void testaa_statusvalot(viesti_t* viesti){
    for(char valo=1; valo<4; valo++){
      for(char i=0; i<4; i++){
          viesti->kentat.statusvalot = valo<<i*2;
          laheta_viesti(viesti);
          latch();
          delay(TESTIAIKA);
      }
    }
    // Kaikki oranssina
    viesti->kentat.statusvalot = 0xFF;
    laheta_viesti(viesti);
    latch();
    delay(TESTIAIKA);
    // Kaikki punaisena
    viesti->kentat.statusvalot = ~0xAA;
    laheta_viesti(viesti);
    latch();
    delay(TESTIAIKA);
    // Kaikki vihreänä
    viesti->kentat.statusvalot = 0xAA;
    laheta_viesti(viesti);
    latch();
    delay(TESTIAIKA);
    return;
}

/* Testaa että numerot tulostuvat oikein eri digitteihin */
void testaa_numerokentat(viesti_t* viesti){
    /* Numeroiden tulostus läpi, katso ettei valot muutu */
    viesti->kentat.statusvalot = 0x00;
    viesti->kentat.numero = SSEG_NUMERO[1];
    viesti->kentat.digit_no = DIG_1;
    laheta_viesti(viesti);
    delay(TESTIAIKA);
    viesti->kentat.numero = SSEG_NUMERO[2];
    viesti->kentat.digit_no = DIG_2;
    laheta_viesti(viesti);
    delay(TESTIAIKA);
    viesti->kentat.numero = SSEG_NUMERO[3];
    viesti->kentat.digit_no = DIG_3;
    laheta_viesti(viesti);
    delay(TESTIAIKA);
    viesti->kentat.numero = SSEG_NUMERO[4];
    viesti->kentat.digit_no = DIG_4;
    laheta_viesti(viesti);
    delay(TESTIAIKA);
    return;
}

/* Testaa että kukin segmentti toimii */
void testaa_segmentit(viesti_t* viesti){
    /* Segmentit järjestyksessä */
    viesti->kentat.digit_no = DIG_1|DIG_2|DIG_3|DIG_4;
    viesti->tavut.tavut_m = 0x00;
    laheta_viesti(viesti);
    delay(TESTIAIKA);
    viesti->tavut.tavut_m = 0xFF;
    laheta_viesti(viesti);
    delay(TESTIAIKA);
    viesti->tavut.tavut_m = 0x00;
    laheta_viesti(viesti);
    delay(TESTIAIKA);
    viesti->tavut.tavut_m = ~SSEG_A;
    laheta_viesti(viesti);
    delay(TESTIAIKA);
    viesti->tavut.tavut_m = ~SSEG_B;
    laheta_viesti(viesti);
    delay(TESTIAIKA);
    viesti->tavut.tavut_m = ~SSEG_C;
    laheta_viesti(viesti);
    delay(TESTIAIKA);
    viesti->tavut.tavut_m = ~SSEG_D;
    laheta_viesti(viesti);
    delay(TESTIAIKA);
    viesti->tavut.tavut_m = ~SSEG_E;
    laheta_viesti(viesti);
    delay(TESTIAIKA);
    viesti->tavut.tavut_m = ~SSEG_F;
    laheta_viesti(viesti);
    delay(TESTIAIKA);
    viesti->tavut.tavut_m = ~SSEG_G;
    laheta_viesti(viesti);
    delay(TESTIAIKA);
    viesti->tavut.tavut_m = ~SSEG_DP;
    laheta_viesti(viesti);
    delay(TESTIAIKA);
    return;
}

/* Testaa että numeroarvot tulostuu ruudulle oikein */
void testaa_numerotulostus(viesti_t* viesti){
    /* Katso ettei valot muutu kun printtaillaan lukuja */
    viesti->kentat.statusvalot = 0x00;
    uint16_t aikaa;
    for(uint16_t i=1; i<=1000; i*=10){
        aikaa = 0;
        while(aikaa < TESTIAIKA){
            aikaa += tulosta_arvo(i, viesti);
        }
        viesti->kentat.statusvalot = ~(viesti->kentat.statusvalot);
    }
    tyhjaa_ruutu(viesti);
    return;
}
