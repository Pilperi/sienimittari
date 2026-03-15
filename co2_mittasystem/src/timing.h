#ifndef TIMING_H
#define TIMING_H

#include <stdint.h>

#define TIMING_CS_1    ((0<<CS02)|(0<<CS01)|(1<<CS00))
#define TIMING_CS_8    ((0<<CS02)|(1<<CS01)|(0<<CS00))
#define TIMING_CS_64   ((0<<CS02)|(1<<CS01)|(1<<CS00))
#define TIMING_CS_256  ((1<<CS02)|(0<<CS01)|(0<<CS00))
#define TIMING_CS_1024 ((1<<CS02)|(0<<CS01)|(1<<CS00))

void timing_delay_set(uint8_t aikaa_paalla, uint16_t toistoja);

#endif // TIMING_H