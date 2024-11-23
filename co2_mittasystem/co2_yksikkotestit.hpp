/* Rautatason yksikkötestien headeri */

#ifndef YKSIKKOTESTIT
#define YKSIKKOTESTIT

#include <stdint.h>
#include "co2_datatyypit.hpp"
#include "co2_kommunikointi.hpp"

#define TESTIAIKA 500

void testaa_statusvalot(viesti_t*);
void testaa_numerokentat(viesti_t*);
void testaa_segmentit(viesti_t*);
void testaa_numerotulostus(viesti_t*);

#endif
