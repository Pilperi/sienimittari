#include "co2_ui.hpp"
#include "co2_kommunikointi.hpp"
#include <stdint.h>


/* Pääfunktio, joka tarkastaa mitä pitäisi tehdä */
unsigned int ui_mainflow(unsigned int moodi, viesti_t* viesti, mittatulos_t* mittatulos, rajat_t* rajat){
    unsigned int aikaa = 0;
    // Normaalimoodissa lähetetään vain statusvalot
    if(moodi==MOODI_NORMAALI){
        aikaa += tulosta_valot(viesti);
        return(moodi);
    }
    // Lukeman näyttö
    if(!(moodi & (MOODI_YLARAJA|MOODI_ALARAJA))){
        if(moodi & MOODI_CO2){
            aikaa += tulosta_lukua(mittatulos->co2, viesti, NAYTTOTAAJUUS);
            }
        else if(moodi & MOODI_KOSTEUS){
            aikaa += tulosta_lukua(mittatulos->ilmankosteus, viesti, NAYTTOTAAJUUS);
            }
        else if(moodi & MOODI_LAMPOTILA){
            aikaa += tulosta_lukua(mittatulos->lampotila, viesti, NAYTTOTAAJUUS);
            }
    }
    // Rajojen muokkaaminen, katsotaan mitä rajaa ja minne päin
    if(moodi & MOODI_MUOKKAA){
        if(moodi & MOODI_CO2){
            moodi = muokkaa_co2_rajoja(moodi, rajat);
            }
        }
        if(moodi & MOODI_KOSTEUS){
            moodi = muokkaa_kosteus_rajoja(moodi, rajat);
        }
        if(moodi & MOODI_LAMPOTILA){
            moodi = muokkaa_lampotila_rajoja(moodi, rajat);
        }
    return(moodi);
}

/* Muokkaa CO2 rajoja, pidä huolta ettei mennä capista yli tai ali */
unsigned int muokkaa_co2_rajoja(unsigned int moodi, rajat_t* rajat){
    // Muokattavana yläraja
    if(moodi & MOODI_YLARAJA){
        uint16_t uusi_raja = rajat->co2_raja_huono;
        // Plus- vai miinusnappi
        if(moodi & MOODI_PLUS){
            uusi_raja += RAJA_ASKEL_CO2;
        }
        else{
            uusi_raja -= RAJA_ASKEL_CO2;
        }
        // Ok jos ei mennä ylärajana capista tai alarajasta ohi
        if((uusi_raja <= RAJA_CO2_CAP_YLA) & (uusi_raja >= rajat->co2_raja_hyva)){
            rajat->co2_raja_huono = uusi_raja;
        }
        moodi &= ~(MOODI_PLUS|MOODI_MIINUS); // flagit veks
    }
    // Vastaavasti alarajalle
    else if(moodi & MOODI_ALARAJA){
        uint16_t uusi_raja = rajat->co2_raja_hyva;
        // Plus- vai miinusnappi
        if(moodi & MOODI_PLUS){
            uusi_raja += RAJA_ASKEL_CO2;
        }
        else{
            uusi_raja -= RAJA_ASKEL_CO2;
        }
        // Ok jos ei mennä ylärajana capista tai alarajasta ohi
        if((uusi_raja >= RAJA_CO2_CAP_ALA) & (uusi_raja >= rajat->co2_raja_huono)){
            rajat->co2_raja_hyva = uusi_raja;
        }
        moodi &= ~(MOODI_PLUS|MOODI_MIINUS); // flagit veks
    }
    return(moodi);
}

/* Muokkaa kosteuden rajoja, pidä huolta ettei mennä capista yli tai ali */
unsigned int muokkaa_rajoja(unsigned int moodi, rajat_t* rajat){
    uint16_t uusi_raja;
    uint16_t* vanha_raja_pt;
    uint16_t raja_yla;
    uint16_t raja_ala;
    // Haetaan oikeat rajapisteet
    // CO2, isompi (yläraja) huonompi
    if(moodi & MOODI_CO2){
        if(moodi & MOODI_YLARAJA){
            vanha_raja_pt = &(rajat->co2_raja_huono);
            uusi_raja = rajat->co2_raja_huono;
            raja_yla = RAJA_CO2_CAP_YLA;
            raja_ala = rajat->co2_raja_hyva;
        }
        else{
            vanha_raja_pt = &(rajat->co2_raja_hyva);
            uusi_raja = rajat->co2_raja_hyva;
            raja_yla = rajat->co2_raja_huono;
            raja_ala = RAJA_CO2_CAP_ALA;
        }
        if(moodi & MOODI_PLUS){uusi_raja += RAJA_ASKEL_CO2;}
        else{uusi_raja -= RAJA_ASKEL_CO2;}
    }
    // Ilmankosteus, enempi parempi
    else if(moodi & MOODI_KOSTEUS){
        if(moodi & MOODI_YLARAJA){
            vanha_raja_pt = &(rajat->ilmankosteus_raja_hyva);
            uusi_raja = rajat->ilmankosteus_raja_hyva;
            raja_yla = RAJA_KOSTEUS_CAP_YLA;
            raja_ala = rajat->ilmankosteus_raja_huono;
        }
        else{
            vanha_raja_pt = &(rajat->ilmankosteus_raja_huono);
            uusi_raja = rajat->ilmankosteus_raja_huono;
            raja_yla = rajat->ilmankosteus_raja_hyva;
            raja_ala = RAJA_KOSTEUS_CAP_ALA;
        }
        if(moodi & MOODI_PLUS){uusi_raja += RAJA_ASKEL_KOSTEUS;}
        else{uusi_raja -= RAJA_ASKEL_KOSTEUS;}
    }
    // Lämpötila, kuumat olot isompi riski
    else{
        if(moodi & MOODI_YLARAJA){
            vanha_raja_pt = &(rajat->lampotila_raja_huono);
            uusi_raja = rajat->lampotila_raja_huono;
            raja_yla = RAJA_LAMPOTILA_CAP_YLA;
            raja_ala = rajat->lampotila_raja_hyva;
        }
        else{
            vanha_raja_pt = &(rajat->lampotila_raja_hyva);
            uusi_raja = rajat->lampotila_raja_hyva;
            raja_yla = rajat->lampotila_raja_huono;
            raja_ala = RAJA_LAMPOTILA_CAP_ALA;
        }
        if(moodi & MOODI_PLUS){uusi_raja += RAJA_ASKEL_LAMPOTILA;}
        else{uusi_raja -= RAJA_ASKEL_LAMPOTILA;}
    }
    // Tarkista mentiinkö jommista kummista rajoista ohitse vai onko ok
    if((uusi_raja <= raja_yla) & (uusi_raja >= raja_ala)){
        *vanha_raja_pt = uusi_raja;
    }
    // Putsaa flagit merkiksi että tehtiin mitä piti tehdä
    moodi &= ~(MOODI_PLUS|MOODI_MIINUS);
    return(moodi);
}
