/* I2C-toiminnallisuuksien headeri. Ohjaa lähinnä oikeaan importtiin */

#ifndef CO2_I2C_H
#define CO2_I2C_H

/* ATtiny-arkkitehtuuri */
#ifdef __AVR_ARCH__
    /* ATtiny USI I2C */
    #if __AVR_ARCH__ == 25
        #include <attiny_i2c/attiny_i2c.h>
    #endif // __AVR_ARCH__ val

/* Jokin muu arkkitehtuuri, katotaan toimiiko wire */
#else
    #include <wire.h>
#endif //__AVR_ARCH__

#endif // CO2_I2C_H
