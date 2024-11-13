/* Juttelufunktiot IO laajentimelle PI4IOE5V9554 */
#include "co2_ioexp.hpp"
#include <Wire.h>


void ioexp_porttisuunta(unsigned char suunnat){
    Wire.beginTransmission(IOEXP_ADDR);
    Wire.write(IOEXP_CMD_CONFIG);
    Wire.write(suunnat);
    Wire.endTransmission();
    return;
}

unsigned char ioexp_lue(){
    Wire.beginTransmission(IOEXP_ADDR);
    Wire.write(IOEXP_CMD_READ);
    Wire.endTransmission(0);
    Wire.requestFrom(IOEXP_ADDR, 1);
    unsigned char arvo = Wire.read();
    Wire.endTransmission();
    return(arvo);
}

void ioexp_out(unsigned char tila){
    Wire.beginTransmission(IOEXP_ADDR);
    Wire.write(IOEXP_CMD_PORT_OUT);
    Wire.write(tila);
    Wire.endTransmission();
    return;
}
