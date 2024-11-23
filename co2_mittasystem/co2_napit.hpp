/*
Nappien operointi
*/

#ifndef _NAPIT
#define _NAPIT

#include <stdint.h>

#define NAPPI_YLOS      1<<0
#define NAPPI_ALAS      1<<1
#define NAPPI_CTRL      1<<2
#define NAPPI_LAMPOTILA 1<<3
#define NAPPI_KOSTEUS   1<<4
#define NAPPI_CO2       1<<5
#define NAPPI_OK        1<<6
#define NAPPI_CANCEL    1<<7

/* Tulkitse miten operointimoodin pitäisi muuttua kun nappia painetaan */
uint16_t tulkitse_painallus(unsigned char, uint16_t);

#endif
