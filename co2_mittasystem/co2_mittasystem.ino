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
#include "co2_logging.hpp"

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
    serialLog("CO2 mittasysteemi");
    // I2C alustus
    Wire.begin();
    if(SENSORI){
        // Odotetaan sensoria
        delay(1000);
        // Periodisen mittauksen aloitus, mittapiste 5 s välein
        Wire.beginTransmission(SCD_ADDRESS);
        Wire.write(0x21);
        Wire.write(0xb1);
        if(Wire.endTransmission() >= 4){
            serialLog("EI SENSORIA");
            // Virhe laitteen kanssa, mennään debug-moodiin
            SENSORI = 0;
            testi();
        }
        else{
            // Odotetaan ensimmäistä mittausta, tehdään vaikka testi odotellessa
            serialLog("Sensori käytössä");
            testi();
            while(!onko_dataa()){delay(10);}
        }
    }
}

uint16_t onko_dataa(){
    uint16_t vastaus = 0;
    unsigned char laskuri;
    Wire.beginTransmission(SCD_ADDRESS);
    Wire.write(0xe4);
    Wire.write(0xb8);
    Wire.endTransmission();
    Wire.requestFrom(SCD_ADDRESS, 3);
    laskuri = 0;
    while(Wire.available()){
      // (sivuutetaan checksum)
      if(laskuri < 2){
        vastaus |= Wire.read();
        vastaus = vastaus << 8;
      }
      laskuri++;
    }
    // Jos ei valmis, pohjimmaiset 11 bittiä nollia
    vastaus &= ~(1<<12);
    return(vastaus);
}

void loop() {
    unsigned char uusi_painallus, edellinen_painallus, muutokset_alas, muutokset_ylos;
    nappi_muutos_t nappitila = {
        .uusi_painallus=0,
        .edellinen_painallus=0,
        .muutokset_alas=0,
        .muutokset_ylos=0,
        .muutokset=0
        };
    ioexp_porttisuunta(0xFF); // kaikki sisääntuloja
    ioexp_out(0x00);
    /* Oletusrajat */
    rajat_t* rajat = uudet_rajat();

    viesti_t* viesti = uusi_viesti();
    
    // Tulos- ja statuswrapperi
    mittatulos_t* mittatulos = uusi_mittatulos();
    uint16_t moodi = MOODI_NORMAALI;
    
    uint8_t data_in[9], laskuri;

    uint16_t aikaa = 0;
    char lukustr[9];

    if(!SENSORI){
        mittatulos->mstatus.status.ctrl_ok = 0;
        mittatulos->mstatus.status.ctrl_nok = 1;
        for(char i=0; i<9; i++){
            data_in[i] = ESIMERKKI_DATA_IN[i];
        }
    }
    else{
        mittatulos->mstatus.status.ctrl_ok = 1;
        mittatulos->mstatus.status.ctrl_nok = 0;
    }
    while(1){
      // Jotain nappia painettu (interrupti olisi parempi)
      if(digitalRead(INTERRUPT) == LOW){
        nappitila.uusi_painallus = ioexp_lue();
        tarkista_muutokset(&nappitila);
        moodi = tulkitse_painallus(&nappitila, moodi);
        nappitila.edellinen_painallus = nappitila.uusi_painallus;
      }
      if(moodi){
        moodi = ui_mainflow(moodi, viesti, mittatulos, rajat, &aikaa);
        if(moodi & MOODI_MUOKKAA){
            paivita_mittastatus(mittatulos, rajat);
            paivita_valot_viestiin(mittatulos, viesti);
            tulosta_valot(viesti);
        }
        }
      delay(1);
      aikaa++;
      // Tulokset inee kun uutta dataa tarjolla
      if(aikaa > 5000){
        if(SENSORI){
            while(!onko_dataa()){delay(5);}
            // Pyydä mittaustulosta
            Wire.beginTransmission(SCD_ADDRESS);
            Wire.write(0xec);
            Wire.write(0x05);
            Wire.endTransmission();

            // Lue data sensorilta
            Wire.requestFrom(SCD_ADDRESS, 9);
            laskuri = 0;
            while (Wire.available()){
              data_in[laskuri++] = Wire.read();
            }
        }
        paivita_mittatulos(mittatulos, data_in);
        paivita_mittastatus(mittatulos, rajat);
        paivita_valot_viestiin(mittatulos, viesti);
        tulosta_valot(viesti);
        aikaa = 0;
        }
    }
    tuhoa_viesti(viesti);
    tuhoa_mittatulos(mittatulos);
    tuhoa_rajat(rajat);
 }
