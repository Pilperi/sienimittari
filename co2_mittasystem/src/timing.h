#ifndef TIMING_H
#define TIMING_H

#include <stdint.h>

#define TIMING_CS_1024 (((1<<CS02)|(0<<CS01)|(1<<CS00)) << CS00)

void timing_delay_set(uint8_t aikaa_paalla, uint8_t aikaa_pois);
void timing_delay_disable(void);

#endif // TIMING_H