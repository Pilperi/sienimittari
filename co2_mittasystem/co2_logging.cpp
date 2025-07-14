#include "co2_logging.h"

unsigned char SERIAL_AVAIL = 0;

char setLogging(){
    Serial.begin(9600);
    for(char i=0; i<100; i++){
      if(Serial){
        SERIAL_AVAIL = 1;
        return(0);
        }
      delay(10);
    }
    SERIAL_AVAIL = 0;
    return(1);
}

void serialLog(const char* viesti){
    // Ei sarjaporttia käytössä
    if(!SERIAL_AVAIL){return;}
    Serial.println(viesti);
}

void serialLogPari(const char* alkuteksti, int luku1, const char* valiteksti, int luku2){
    // Ei sarjaporttia käytössä
    if(!SERIAL_AVAIL){return;}
    Serial.printf(alkuteksti);
    Serial.printf("%d %s %d\n", luku1, valiteksti, luku2);
}
