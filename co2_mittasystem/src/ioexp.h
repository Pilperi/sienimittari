/*
Juttelufunktiot IO laajentimelle PI4IOE5V9554
*/
#ifndef IOEXP_H
#define IOEXP_H

#include <stdint.h>

#define IOEXP_ADDR  0b01110000
#define IOEXP_WRITE 0<<0
#define IOEXP_READ  1<<0

#define IOEXP_CMD_READ     0
#define IOEXP_CMD_PORT_OUT 1
#define IOEXP_CMD_POL_INV  2
#define IOEXP_CMD_CONFIG   3

// Porttisuunnat, 1 inputille ja 0 outputille
void ioexp_porttisuunta(uint8_t laiteosoite, uint8_t suunnat);

// Lue porttien arvot
uint8_t ioexp_lue(uint8_t laiteosoite);

// Aseta ulostulojen arvot
void ioexp_out(uint8_t laiteosoite, uint8_t tila);

#endif // IOEXP_H
