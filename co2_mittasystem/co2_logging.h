#include <Arduino.h>
#include <String.h>

// Aseta loggausparametrit
char setLogging(void);
// Lähetä logiviesti
void serialLog(const char*);
// Lähetä lukuarvollinen viesti
void serialLogPari(const char*, int, const char*, int);
