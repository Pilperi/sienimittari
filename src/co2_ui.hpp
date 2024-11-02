/* UI-puolen hommat: mitä näytetään kun nappia painetaan jne */

#ifndef _CO2_UI
#define _CO2_UI

#include "co2_datatyypit.hpp"

#define NAYTTOTAAJUUS 100
#define RAJA_ASKEL_CO2 10
#define RAJA_ASKEL_KOSTEUS 5
#define RAJA_ASKEL_LAMPOTILA 1

#define RAJA_CO2_CAP_YLA 9990
#define RAJA_CO2_CAP_ALA 0
#define RAJA_KOSTEUS_CAP_YLA 100
#define RAJA_KOSTEUS_CAP_ALA 0
#define RAJA_LAMPOTILA_CAP_YLA 30
#define RAJA_LAMPOTILA_CAP_ALA 0


/* Laitteen operaatiomoodit */
enum NappiMoodit {
    MOODI_NORMAALI   = 0,
    MOODI_MUOKKAA    = 1<<0,
    MOODI_YLARAJA    = 1<<1,
    MOODI_ALARAJA    = 1<<2,
    MOODI_LAMPOTILA  = 1<<3,
    MOODI_KOSTEUS    = 1<<4,
    MOODI_CO2        = 1<<5,
    MOODI_PLUS       = 1<<6,
    MOODI_MIINUS     = 1<<7,
    MOODI_TALLENNA   = 1<<8
};

unsigned int ui_mainflow(unsigned int, viesti_t*, mittatulos_t*, rajat_t*);

#endif