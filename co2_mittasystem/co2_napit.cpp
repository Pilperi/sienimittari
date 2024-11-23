#include <stdint.h>
#include "co2_ui.hpp"
#include "co2_napit.hpp"

/* Tulkitsee napinpainallukset nykymoodin valossa */
uint16_t tulkitse_painallus(unsigned char painallus, uint16_t moodi){
    // Yksittäispainallukset: vaihdetaan tilaa (päälle tai pois)
    switch(painallus){
        case NAPPI_CO2:
            moodi ^= MOODI_CO2;
            moodi &= ~(MOODI_KOSTEUS|MOODI_LAMPOTILA);
            moodi &= ~(MOODI_MUOKKAA|MOODI_PLUS|MOODI_MIINUS);
            return(moodi);
        case NAPPI_KOSTEUS:
            moodi ^= MOODI_KOSTEUS;
            moodi &= ~(MOODI_CO2|MOODI_LAMPOTILA);
            moodi &= ~(MOODI_MUOKKAA|MOODI_PLUS|MOODI_MIINUS);
            return(moodi);
        case NAPPI_LAMPOTILA:
            moodi ^= MOODI_LAMPOTILA;
            moodi &= ~(MOODI_CO2|MOODI_KOSTEUS);
            moodi &= ~(MOODI_MUOKKAA|MOODI_PLUS|MOODI_MIINUS);
            return(moodi);
        // Pelkät plussa- ja miinuspainallukset tekee mitään
        // vain jos ollaan jo jonkin arvon muokkausmoodissa
        case NAPPI_YLOS:
            if(moodi & MOODI_MUOKKAA){
                moodi |= MOODI_PLUS;
            }
            return(moodi);
        case NAPPI_ALAS:
            if(moodi & MOODI_MUOKKAA){
                moodi |= MOODI_MIINUS;
            }
            return(moodi);
        default:
            break;
    }
    // Monta nappia samaan aikaan
    // Plussa ja miinus vain jos toinen nappi joku arvonappi
    if(painallus&NAPPI_YLOS){
        // Ei oltu muokkausmoodissa: siirrytään
        if(painallus&(NAPPI_CO2|NAPPI_KOSTEUS|NAPPI_LAMPOTILA)){
            moodi |= (MOODI_MUOKKAA|MOODI_YLARAJA);
        }
        // Oltiin: lisätään
        else if(moodi&(MOODI_CO2|MOODI_KOSTEUS|MOODI_LAMPOTILA)){
            moodi |= MOODI_MIINUS;
        }
    }
    else if(painallus&NAPPI_ALAS){
        if(painallus&(NAPPI_CO2|NAPPI_KOSTEUS|NAPPI_LAMPOTILA)){
            moodi |= (MOODI_MUOKKAA|MOODI_ALARAJA);
        }
        else if(moodi&(MOODI_CO2|MOODI_KOSTEUS|MOODI_LAMPOTILA)){
            moodi |= MOODI_MIINUS;
        }
    }
    // Arvopainallukset jonkun toisen kanssa samaan aikaan:
    // laitetaan moodi päälle
    if(painallus & NAPPI_CO2){
        // Vaihdetaan CO2-tilaa (päälle tai pois)
        moodi |= MOODI_CO2;
    }
    // Painallus ilmankosteus
    else if(painallus & NAPPI_KOSTEUS){
        // Vaihdetaan kosteustilaa (päälle tai pois)
        moodi |= MOODI_KOSTEUS;
    }
    // Painallus lämpötila
    else if(painallus == NAPPI_LAMPOTILA){
        // Vaihdetaan lämpö-tilaa (päälle tai pois)
        moodi |= MOODI_LAMPOTILA;
    }
    return(moodi);
}
