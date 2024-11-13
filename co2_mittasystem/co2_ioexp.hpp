/*
Juttelufunktiot IO laajentimelle PI4IOE5V9554
*/
#ifndef _PI4IOE5V9554
#define _PI4IOE5V9554

#define IOEXP_ADDR 0b0111000
#define IOEXP_WRITE 0
#define IOEXP_READ  1

#define IOEXP_CMD_READ     0
#define IOEXP_CMD_PORT_OUT 1
#define IOEXP_CMD_POL_INV  2
#define IOEXP_CMD_CONFIG   3

// Porttisuunnat, 1 inputille ja 0 outputille
void ioexp_porttisuunta(unsigned char);
// Lue porttien arvot
unsigned char ioexp_lue(void);
// Aseta ulostulojen arvot
void ioexp_out(unsigned char);

#endif
