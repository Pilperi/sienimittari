/*
Nappien operointi
*/

#ifndef _NAPIT
#define _NAPIT

#define NAPPI_CO2       1<<0
#define NAPPI_KOSTEUS   1<<1
#define NAPPI_LAMPOTILA 1<<2
#define NAPPI_YLOS      1<<3
#define NAPPI_ALAS      1<<4

/* Tulkitse miten operointimoodin pitäisi muuttua kun nappia painetaan */
unsigned int tulkitse_painallus(unsigned char, unsigned int);

#endif