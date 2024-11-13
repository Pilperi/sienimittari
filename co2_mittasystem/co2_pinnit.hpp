/* Pinninimet ja pinnien roolit */
#ifndef _PINNIT_NIMET
#define _PINNIT_NIMET

#include <Arduino.h>

#define FREE0       0  // VAPAA
#define LATCH       1  // Valojen latchaus
#define INTERRUPT   2  // IO-laajentimen interrupt
#define SHIFT_CLK   3  // sreg kello
#define I2C_DAT     4  // I2C data
#define I2C_CLK     5  // I2C kello
#define SHIFT_DAT   6  // sreg data
#define FREE3       7  // VAPAA
#define FREE4       8  // VAPAA
#define FREE5       9  // VAPAA

/* Alusta pinnit */
void aseta_pinnitilat(void);

#endif
