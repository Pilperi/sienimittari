#include "co2_ui.hpp"
#include "co2_kommunikointi.hpp"
#include <stdint.h>


/* Pääfunktio, joka tarkastaa mitä pitäisi tehdä */
uint16_t ui_mainflow(uint16_t moodi, viesti_t* viesti, mittatulos_t* mittatulos, rajat_t* rajat, uint16_t* aikaa){
    // Normaalimoodissa lähetetään vain statusvalot
    if(moodi==MOODI_NORMAALI){
        tulosta_valot(viesti);
        return(moodi);
    }
    // Lukeman näyttö
    if(!(moodi & (MOODI_YLARAJA|MOODI_ALARAJA))){
        if(moodi & MOODI_CO2){
            *aikaa += tulosta_lukua(mittatulos->co2, viesti, NAYTTOTAAJUUS);
            }
        else if(moodi & MOODI_KOSTEUS){
            *aikaa += tulosta_lukua(mittatulos->ilmankosteus, viesti, NAYTTOTAAJUUS);
            }
        else if(moodi & MOODI_LAMPOTILA){
            *aikaa += tulosta_lukua(mittatulos->lampotila, viesti, NAYTTOTAAJUUS);
            }
    }
    // Rajojen muokkaaminen, katsotaan mitä rajaa ja minne päin
    if(moodi & MOODI_MUOKKAA){
        if(moodi & MOODI_CO2){
            moodi = muokkaa_rajoja(moodi, rajat);
            }
        }
        if(moodi & MOODI_KOSTEUS){
            moodi = muokkaa_rajoja(moodi, rajat);
        }
        if(moodi & MOODI_LAMPOTILA){
            moodi = muokkaa_rajoja(moodi, rajat);
        }
    return(moodi);
}

/* Muokkaa hiilidioksidin rajoja */
void muokkaa_rajoja_co2(uint16_t moodi, rajat_t* rajat){
    uint16_t uusi_raja;
    uint16_t raja_yla;
    uint16_t raja_ala;
    // Haetaan oikeat rajapisteet
    // CO2, isompi (yläraja) huonompi
    if(moodi & MOODI_YLARAJA){
        uusi_raja = rajat->co2_raja_huono;
        raja_yla = RAJA_CO2_CAP_YLA;
        raja_ala = rajat->co2_raja_hyva;
    }
    else{
        uusi_raja = rajat->co2_raja_hyva;
        raja_yla = rajat->co2_raja_huono;
        raja_ala = RAJA_CO2_CAP_ALA;
    }
    if(moodi & MOODI_PLUS){uusi_raja += RAJA_ASKEL_CO2;}
    else{uusi_raja -= RAJA_ASKEL_CO2;}
    // Tarkista mentiinkö jommista kummista rajoista ohitse vai onko ok
    if((uusi_raja <= raja_yla) & (uusi_raja >= raja_ala)){
        if(moodi & MOODI_YLARAJA){
            rajat->co2_raja_huono = uusi_raja;
        }
        else{
            rajat->co2_raja_hyva = uusi_raja;
        }
    }
}

/* Muokkaa kosteuden rajoja */
void muokkaa_rajoja_kosteus(uint16_t moodi, rajat_t* rajat){
    uint16_t uusi_raja;
    uint16_t raja_yla;
    uint16_t raja_ala;
    if(moodi & MOODI_YLARAJA){
        uusi_raja = rajat->ilmankosteus_raja_hyva;
        raja_yla = RAJA_KOSTEUS_CAP_YLA;
        raja_ala = rajat->ilmankosteus_raja_huono;
    }
    else{
        uusi_raja = rajat->ilmankosteus_raja_huono;
        raja_yla = rajat->ilmankosteus_raja_hyva;
        raja_ala = RAJA_KOSTEUS_CAP_ALA;
    }
    if(moodi & MOODI_PLUS){uusi_raja += RAJA_ASKEL_KOSTEUS;}
    else{uusi_raja -= RAJA_ASKEL_KOSTEUS;}
    // Tarkista mentiinkö jommista kummista rajoista ohitse vai onko ok
    if((uusi_raja <= raja_yla) & (uusi_raja >= raja_ala)){
        if(moodi & MOODI_YLARAJA){
            rajat->ilmankosteus_raja_hyva = uusi_raja;
        }
        else{
            rajat->ilmankosteus_raja_huono = uusi_raja;
        }
    }
}

/* Muokkaa lämpötilan rajoja */
void muokkaa_rajoja_lampotila(uint16_t moodi, rajat_t* rajat){
    uint16_t uusi_raja;
    uint16_t raja_yla;
    uint16_t raja_ala;
    if(moodi & MOODI_YLARAJA){
        uusi_raja = rajat->lampotila_raja_huono;
        raja_yla = RAJA_LAMPOTILA_CAP_YLA;
        raja_ala = rajat->lampotila_raja_hyva;
    }
    else{
        uusi_raja = rajat->lampotila_raja_hyva;
        raja_yla = rajat->lampotila_raja_huono;
        raja_ala = RAJA_LAMPOTILA_CAP_ALA;
    }
    if(moodi & MOODI_PLUS){uusi_raja += RAJA_ASKEL_LAMPOTILA;}
    else{uusi_raja -= RAJA_ASKEL_LAMPOTILA;}
    // Tarkista mentiinkö jommista kummista rajoista ohitse vai onko ok
    if((uusi_raja <= raja_yla) & (uusi_raja >= raja_ala)){
        if(moodi & MOODI_YLARAJA){
            rajat->lampotila_raja_huono = uusi_raja;
        }
        else{
            rajat->lampotila_raja_hyva = uusi_raja;
        }
    }
}

/* Muokkaa rajoja. Ohjaa oikeaan säätöfunktioon. */
uint16_t muokkaa_rajoja(uint16_t moodi, rajat_t* rajat){
    if(moodi & MOODI_CO2){
        muokkaa_rajoja_co2(moodi, rajat);
    }
    // Ilmankosteus, enempi parempi
    else if(moodi & MOODI_KOSTEUS){
        muokkaa_rajoja_kosteus(moodi, rajat);
    }
    // Lämpötila, kuumat olot isompi riski
    else{
        muokkaa_rajoja_lampotila(moodi, rajat);
    }
    // Putsaa flagit merkiksi että tehtiin mitä piti tehdä
    moodi &= ~(MOODI_PLUS|MOODI_MIINUS);
    return(moodi);
}
