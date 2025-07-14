#include <Arduino.h>
#include <stdlib.h>
#include <stdint.h>
#include <Wire.h>
#include "co2_datatyypit.hpp"
#include "co2_pinnit.hpp"
#include "co2_kommunikointi.hpp"
#include "co2_ioexp.hpp"
#include "co2_ui.hpp"
#include "co2_napit.hpp"
#include "co2_yksikkotestit.hpp"
#include "co2_logging.h"

// Jos sensori ei ole kiinni tai halutaan muuten vaan testailla, arvo 0
unsigned char SENSORI = 1;

// Esimerkkitulos
uint8_t ESIMERKKI_DATA_IN[9] = {0x01, 0xF4, 0x7B, 0x66, 0x67, 0xA2, 0x5E, 0xB9, 0x3C};

// SCD4x
const int16_t SCD_ADDRESS = 0x62;



/* Testimoodi, katso toimiiko kaikki */
void testi(){
    serialLog("TESTI START");
    viesti_t* viesti = uusi_viesti();
    testaa_statusvalot(viesti);
    testaa_segmentit(viesti);
    testaa_numerokentat(viesti);
    testaa_numerotulostus(viesti);
    // Valmis
    tuhoa_viesti(viesti);
    serialLog("TESTI END");
    return;
}

void setup() {
    setLogging();
    aseta_pinnitilat();
    serialLog("CO2");
    if(SENSORI){
        // I2C alustus
        Wire.begin();
        // Odotetaan sensoria
        delay(1000);
        // Periodisen mittauksen aloitus, mittapiste 5 s välein
        Wire.beginTransmission(SCD_ADDRESS);
        Wire.write(0x21);
        Wire.write(0xb1);
        if(Wire.endTransmission()){
            serialLog("EI SENSORIA");
            // Virhe laitteen kanssa, mennään debug-moodiin
            SENSORI = 0;
            testi();
        }
        else{
            // Odotetaan ensimmäistä mittausta, tehdään vaikka testi odotellessa
            serialLog("Sensori käytössä");
            testi();
        }
    }
}


void loop() {
    unsigned char painettu_nappi;
    ioexp_porttisuunta(0xFF); // kaikki sisääntuloja
    ioexp_out(0x00);
    painettu_nappi = ioexp_lue();
    /* Oletusrajat */
    rajat_t* rajat = uudet_rajat();

    viesti_t* viesti = uusi_viesti();
    
    // Tulos- ja statuswrapperi
    mittatulos_t* mittatulos = uusi_mittatulos();
    uint16_t moodi = MOODI_NORMAALI;
    
    uint8_t data_in[9], laskuri;
    // CTRL punaisena jos ei sensoria
    // ja käytetään mittatuloksena esimerkkitulosta
    if(!SENSORI){
        mittatulos->mstatus.status.ctrl_ok = 0;
        mittatulos->mstatus.status.ctrl_nok = 1;
        for(char i=0; i<9; i++){
          data_in[i] = ESIMERKKI_DATA_IN[i];
          }
    }
    uint16_t aikaa;
    while(1){
      if(SENSORI){
          serialLog("Sensori käytettävissä");
          // Pyydä mittaustulosta
          Wire.beginTransmission(SCD_ADDRESS);
          Wire.write(0xec);
          Wire.write(0x05);
          if(!Wire.endTransmission()){
              // Lue data sensorilta
              Wire.requestFrom(SCD_ADDRESS, 9);
              laskuri = 0;
              while (Wire.available()) {
                  data_in[laskuri++] = Wire.read();
              }
          }
          else{
              // Laite ei enää vastaa
              serialLog("Sensori lakkasi vastaamasta!");
              SENSORI = 0;
              mittatulos->mstatus.status.ctrl_ok = 0;
              mittatulos->mstatus.status.ctrl_nok = 1;
              for(char i=0; i<9; i++){
                data_in[i] = ESIMERKKI_DATA_IN[i];
                }
          }
      }

      // Tulokset inee
      paivita_mittatulos(mittatulos, data_in);
      paivita_mittastatus(mittatulos, rajat);
      paivita_valot_viestiin(mittatulos, viesti);

      tulosta_valot(viesti);
      aikaa = 0;
      while(aikaa < 5000){
        if(digitalRead(INTERRUPT) == LOW){
            painettu_nappi = ioexp_lue();
            moodi = tulkitse_painallus(painettu_nappi, moodi);
        }
        if(moodi & (MOODI_CO2|MOODI_KOSTEUS|MOODI_LAMPOTILA)){
          ui_mainflow(moodi, viesti, mittatulos, rajat, &aikaa);
          }
        //aikaa += tulosta_lukua(1234, viesti, 2000);
        delay(1);
        aikaa++;
    }
    }
    tuhoa_viesti(viesti);
    tuhoa_mittatulos(mittatulos);
    tuhoa_rajat(rajat);
 }
