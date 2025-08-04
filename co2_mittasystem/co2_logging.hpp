#ifndef CO2_LOGGING_H_
#define CO2_LOGGING_H_

#include <Arduino.h>
#include <String.h>

// Aseta loggausparametrit
char setLogging(void);
// Lähetä logiviesti
void serialLog(const char*);
// Lähetä lukuarvollinen viesti
void serialLogPari(const char*, int, const char*, int);
// Muunna lukuarvo binäärin kuvaavaksi stringiksi
void binstr(unsigned char, char*);
#endif
