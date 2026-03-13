/* Pohjustusfunktiot pinneille */
#include <Arduino.h>
#include "co2_pinnit.hpp"

void aseta_pinnitilat(void){
    pinMode(LATCH, OUTPUT);
    pinMode(SHIFT_CLK, OUTPUT);
    pinMode(SHIFT_DAT, OUTPUT);
    pinMode(INTERRUPT, INPUT);
    return;
}
