#include <stdint.h>
#include "co2_ui.hpp"
#include "co2_napit.hpp"
#include "co2_logging.hpp"
#include "co2_kommunikointi.hpp"

/* Tarkista mitkä napit painettiin alas ja mitkä vapautettiin */
void tarkista_muutokset(nappi_muutos_t* nappitila){
    unsigned char muutokset = nappitila->uusi_painallus ^ nappitila->edellinen_painallus;
    nappitila->muutokset = muutokset;
    unsigned char ylos = 0;
    unsigned char alas = 0;
    unsigned char maski;
    for(unsigned char i=0; i<8; i++){
        maski = 1<<i;
        if(muutokset & maski){
            // Painallus alas
            if(nappitila->uusi_painallus & maski){
                alas += maski;
            }
            // Vapautettu painallus
            else{
                ylos += maski;
            }
        }
    }
    nappitila->muutokset_alas = alas;
    nappitila->muutokset_ylos = ylos;
}
