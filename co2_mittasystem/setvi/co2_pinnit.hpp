/* Pinninimet ja pinnien roolit */
#ifndef _PINNIT_NIMET
#define _PINNIT_NIMET

#include <Arduino.h>

#define FREE0       0  // NC
#define FREE1       1  // NC
#define FREE2       2  // NC
#define FREE3       3  // NC
#define I2C_DAT     4  // I2C data
#define I2C_CLK     5  // I2C kello
#define LATCH       6  // Valojen latch
#define FREE4       7  // NC
#define SHIFT_CLK   8  // sreg kello
#define INTERRUPT   9  // IO-laajentimen interrupt
#define SHIFT_DAT   10 // sreg data


/* Alusta pinnit */
void aseta_pinnitilat(void);

#endif
