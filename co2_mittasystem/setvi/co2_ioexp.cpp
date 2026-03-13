/* Juttelufunktiot IO laajentimelle PI4IOE5V9554 */
#include <stdint.h>
#include "co2_ioexp.hpp"
#include "co2_i2c.h"


void ioexp_porttisuunta(uint8_t laiteosoite, uint8_t suunnat){
    i2c_setup(I2C_MOODI_SOFTAKELLO);
    i2c_aloita();
    i2c_kirjoita(laiteosoite|IOEXP_WRITE);
    i2c_ack(I2C_LUE_ACK);
    i2c_kirjoita(IOEXP_CMD_CONFIG);
    i2c_ack(I2C_LUE_ACK);
    i2c_kirjoita(suunnat);
    i2c_ack(I2C_LUE_ACK);
    i2c_lopeta();
    return;
}

uint8_t ioexp_lue(uint8_t laiteosoite){
    uint8_t arvo;
    i2c_setup(I2C_MOODI_SOFTAKELLO);
    i2c_aloita();
    i2c_kirjoita(laiteosoite|IOEXP_WRITE);
    i2c_ack(I2C_LUE_ACK);
    i2c_kirjoita(IOEXP_CMD_READ);
    i2c_ack(I2C_LUE_ACK);
    i2c_aloita();
    i2c_kirjoita(laiteosoite|IOEXP_READ);
    i2c_ack(I2C_LUE_ACK);
    arvo = i2c_lue();
    i2c_lopeta();
    return(arvo);
}

void ioexp_out(uint8_t laiteosoite, uint8_t tila){
    i2c_setup(I2C_MOODI_SOFTAKELLO);
    i2c_aloita();
    i2c_kirjoita(laiteosoite|IOEXP_WRITE);
    i2c_ack(I2C_LUE_ACK);
    i2c_kirjoita(IOEXP_CMD_PORT_OUT);
    i2c_ack(I2C_LUE_ACK);
    i2c_kirjoita(tila);
    i2c_ack(I2C_LUE_ACK);
    i2c_lopeta();
    return;
}
