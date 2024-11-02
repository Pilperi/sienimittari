#include <Arduino.h>
#include <stdlib.h>
#include <Wire.h>
#include "co2_datatyypit.hpp"
#include "co2_pinnit.hpp"
#include "co2_kommunikointi.hpp"
#include "co2_ioexp.hpp"

#define SENSORI 0

// SCD4x
const int16_t SCD_ADDRESS = 0x62;


void setup() {
    aseta_pinnitilat();
    
    Wire.begin();
    if(SENSORI){
    // init I2C

    // wait until sensors are ready, > 1000 ms according to datasheet
    delay(1000);

    // start scd measurement in periodic mode, will update every 5 s
    Wire.beginTransmission(SCD_ADDRESS);
    Wire.write(0x21);
    Wire.write(0xb1);
    Wire.endTransmission();

    // wait for first measurement to be finished
    delay(5000);
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
    
    unsigned int moodi = MOODI_NORMAALI;
    
    uint8_t data_in[12], laskuri;
    unsigned int aikaa;
    while(1){
      if(SENSORI){
          // Pyydä mittaustulosta
          Wire.beginTransmission(SCD_ADDRESS);
          Wire.write(0xec);
          Wire.write(0x05);
          Wire.endTransmission();
  
          // Lue data sensorilta
          Wire.requestFrom(SCD_ADDRESS, 12);
          laskuri = 0;
          while (Wire.available()) {
              data_in[laskuri++] = Wire.read();
          }
          // Tulokset inee
          paivita_mittatulos(mittatulos, data_in);
          paivita_mittastatus(mittatulos, rajat);
      }
  
      aikaa = 0;
      while(aikaa < 5000){
        if(digitalRead(INTERRUPT) == LOW){
            painettu_nappi = ioexp_lue();
            moodi = tulkitse_painallus(painettu_nappi, moodi);
            moodi = ui_mainflow(moodi, viesti, mittatulos);
        }
        else{delay(1);aikaa++;}
    }
    }
    tuhoa_viesti(viesti);
    tuhoa_mittatulos(mittatulos);
    tuhoa_rajat(rajat);
 }
