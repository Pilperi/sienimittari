#include <stdint.h>
#include <avr/io.h>
#include "timing.h"

uint16_t TOISTOJA;
uint16_t TOISTOJA_REF;

/* Asetetaan ajastusinterruptien parametrit. */
void timing_delay_set(uint8_t aikaa_paalla, uint16_t toistoja)
{
    TCCR0B = 0x00;
    TCNT0 = 0x00;
    OCR0A = aikaa_paalla;
    TCCR0A = 0x00;
    TCCR0B = TIMING_CS_1024;
    TCNT0 = 0x00;
    TIMSK = (1<<OCIE0A);
    TOISTOJA = toistoja;
    TOISTOJA_REF = TOISTOJA;
}

/* Laitetaan ajastusinteruptit pois päältä. */
void timing_delay_unset(void)
{
    TIMSK = 0x00;
    TCCR0B = 0x00;
}
