#include <stdint.h>
#include "co2_ui.hpp"
#include "co2_napit.hpp"

/* Tulkitsee napinpainallukset nykymoodin valossa */
uint16_t tulkitse_painallus(unsigned char painallus, uint16_t moodi){
    // Plus ja miinus samaan aikaan asettaa aina takaisin normaalitilaan
    if((painallus & NAPPI_ALAS) & NAPPI_YLOS){
        moodi = MOODI_NORMAALI;
        return(moodi);
    }
    // Painallus ylös
    else if(painallus & NAPPI_YLOS){
        // Oltiin muokkausmoodissa: kerrotaan että arvoa nostettava
        if(moodi & MOODI_MUOKKAA){
            moodi |= MOODI_PLUS;
        }
        // Ei oltu: mennään ylärajan muokkausmoodiin
        // mutta vain jos kerrottu mitä muokataan
        else if(moodi&(NAPPI_CO2|NAPPI_KOSTEUS|NAPPI_LAMPOTILA)){
            moodi |= MOODI_YLARAJA;
            moodi |= MOODI_MUOKKAA;
        }
    }
    // Painallus alas
    else if(painallus & NAPPI_ALAS){
        // Oltiin muokkausmoodissa: kerrotaan että arvoa laskettava
        if(moodi & MOODI_MUOKKAA){
            moodi |= MOODI_MIINUS;
        }
        // Ei oltu: mennään alarajan muokkausmoodiin
        // mutta vain jos kerrottu mitä muokataan
        else if(moodi&(NAPPI_CO2|NAPPI_KOSTEUS|NAPPI_LAMPOTILA)){
            moodi |= MOODI_ALARAJA;
            moodi |= MOODI_MUOKKAA;
        }
    }
    // Painallus CO2
    else if(painallus & NAPPI_CO2){
        // Ei oltu CO2-moodissa vielä: mennään
        if(!(moodi & MOODI_CO2)){
            moodi |= MOODI_CO2;
        }
        // Oltiin jo ja painettiin ilman plussaa tai miinusta:
        // tallennetaan arvo ja poistutaan muokkausmoodista
        else if(!(painallus & (NAPPI_YLOS|NAPPI_ALAS))){
            moodi |= MOODI_TALLENNA;
            moodi &= ~MOODI_MUOKKAA;
        }
    }
    // Painallus ilmankosteus
    else if(painallus & NAPPI_KOSTEUS){
        // Ei oltu kosteusmoodissa vielä: mennään
        if(!(moodi & MOODI_KOSTEUS)){
            moodi |= MOODI_KOSTEUS;
        }
        // Oltiin jo ja painettiin ilman plussaa tai miinusta:
        // tallennetaan arvo ja poistutaan muokkausmoodista
        else if(!(painallus & (NAPPI_YLOS|NAPPI_ALAS))){
            moodi |= MOODI_TALLENNA;
            moodi &= ~MOODI_MUOKKAA;
        }
    }
    // Painallus lämpötila
    else if(painallus & NAPPI_LAMPOTILA){
        // Ei oltu kosteusmoodissa vielä: mennään
        if(!(moodi & MOODI_LAMPOTILA)){
            moodi |= MOODI_LAMPOTILA;
        }
        // Oltiin jo ja painettiin ilman plussaa tai miinusta:
        // tallennetaan arvo ja poistutaan muokkausmoodista
        else if(!(painallus & (NAPPI_YLOS|NAPPI_ALAS))){
            moodi |= MOODI_TALLENNA;
            moodi &= ~MOODI_MUOKKAA;
        }
    }
    return(moodi);
}
