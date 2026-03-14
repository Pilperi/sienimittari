#include <stdint.h>
#include <avr/io.h>
#include "timing.h"

uint8_t TIMER_COUNT;

void timing_delay_set(uint8_t aikaa_paalla, uint8_t aikaa_pois)
{
    TCCR0B = 0x00;
    TCNT0 = 0x00;
    OCR0A = aikaa_pois+aikaa_paalla;
    OCR0B = aikaa_paalla;
    TCCR0A = 0x00;
    TCCR0B = TIMING_CS_1024;
    TCNT0 = 0x00;
    TIMSK = (1<<OCIE0A)|(1<<OCIE0B);
}

void timing_delay_disable(void)
{
    TIMSK = 0x00;
    TCCR0B = 0x00;
}

void isr_timer0_compa(void)
{
    TIMER_COUNT--;
    if(!TIMER_COUNT)
    {
        timing_delay_disable();
    }
}