/* Juttelufunktiot IO laajentimelle PI4IOE5V9554 */
#include <stdint.h>
#include <attiny_i2c/attiny_i2c.h>
#include "ioexp.h"


/* Aseta porttien suunnat.

1 ulostulolle, 0 sisääntulolle.
*/
void ioexp_porttisuunta(uint8_t laiteosoite, uint8_t suunnat)
{
    i2c_setup(I2C_MOODI_SOFTAKELLO);
    i2c_aloita();
    i2c_kirjoita(IOEXP_ADDR|laiteosoite|IOEXP_WRITE);
    i2c_ack(I2C_ACK_LUE);
    i2c_kirjoita(IOEXP_CMD_CONFIG);
    i2c_ack(I2C_ACK_LUE);
    i2c_kirjoita(suunnat);
    i2c_ack(I2C_ACK_LUE);
    i2c_lopeta();
    return;
}

/* Lue porttien arvot.
Portin bitti on 1 jos ylhäällä, 0 jos alhaalla.
*/
uint8_t ioexp_lue(uint8_t laiteosoite){
    uint8_t arvo;
    i2c_setup(I2C_MOODI_SOFTAKELLO);
    i2c_aloita();
    i2c_kirjoita(IOEXP_ADDR|laiteosoite|IOEXP_WRITE);
    i2c_ack(I2C_ACK_LUE);
    i2c_kirjoita(IOEXP_CMD_READ);
    i2c_ack(I2C_ACK_LUE);
    i2c_aloita_rep();
    i2c_kirjoita(IOEXP_ADDR|laiteosoite|IOEXP_READ);
    i2c_ack(I2C_ACK_LUE);
    arvo = i2c_lue();
    i2c_lopeta();
    return(arvo);
}

/* Aseta porttien tilat. */
void ioexp_out(uint8_t laiteosoite, uint8_t tila){
    i2c_setup(I2C_MOODI_SOFTAKELLO);
    i2c_aloita();
    i2c_kirjoita(IOEXP_ADDR|laiteosoite|IOEXP_WRITE);
    i2c_ack(I2C_ACK_LUE);
    i2c_kirjoita(IOEXP_CMD_PORT_OUT);
    i2c_ack(I2C_ACK_LUE);
    i2c_kirjoita(tila);
    i2c_ack(I2C_ACK_LUE);
    i2c_lopeta();
    return;
}
