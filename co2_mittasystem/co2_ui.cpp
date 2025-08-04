#include "co2_logging.hpp"
#include "co2_ui.hpp"
#include "co2_kommunikointi.hpp"
#include <stdint.h>

/* Tallennat rajat EEPROMille TODO*/
void tallenna_rajat(rajat_t* rajat){
    return;
}

/* Tulkitsee napinpainallukset nykymoodin valossa */
uint16_t tulkitse_painallus(nappi_muutos_t* nappitila, uint16_t moodi){
    // Peruutusnappi vie aina alkutilaan
    if(nappitila->muutokset_alas & NAPPI_CANCEL){return(MOODI_NORMAALI);}
    // Muuttuja + plus/miinus vie aina muokkaustilaan
    if(nappitila->muutokset_alas & (NAPPI_YLOS|NAPPI_ALAS)){
        switch(nappitila->uusi_painallus & (NAPPI_CO2|NAPPI_KOSTEUS|NAPPI_LAMPOTILA)){
            case NAPPI_CO2:
                moodi &= ~(MOODI_KOSTEUS|MOODI_LAMPOTILA);
                moodi |= MOODI_MUOKKAA|MOODI_CO2;
                if(nappitila->muutokset_alas & NAPPI_YLOS){
                    return moodi|MOODI_YLARAJA;
                }
                return moodi|MOODI_ALARAJA;
            case NAPPI_KOSTEUS:
                moodi &= ~(MOODI_CO2|MOODI_LAMPOTILA);
                moodi |= MOODI_MUOKKAA|MOODI_KOSTEUS;
                if(nappitila->muutokset_alas & NAPPI_YLOS){
                    return moodi|MOODI_YLARAJA;
                }
                return moodi|MOODI_ALARAJA;
            case NAPPI_LAMPOTILA:
                moodi &= ~(MOODI_CO2|MOODI_KOSTEUS);
                moodi |= MOODI_MUOKKAA|MOODI_LAMPOTILA;
                if(nappitila->muutokset_alas & NAPPI_YLOS){
                    return moodi|MOODI_YLARAJA;
                }
                return moodi|MOODI_ALARAJA;
        }
    }
    // Normaalimoodissa
    if(moodi == MOODI_NORMAALI){
        switch(nappitila->muutokset_alas){
            case NAPPI_CO2:
                serialLog("Moodiin CO2");
                return MOODI_CO2;
            case NAPPI_KOSTEUS:
                serialLog("Moodiin HUM");
                return MOODI_KOSTEUS;
            case NAPPI_LAMPOTILA:
                serialLog("Moodiin TMP");
                return MOODI_LAMPOTILA;
            default:
                serialLog("Moodiin NORMAALI");
                return MOODI_NORMAALI;
        }
    }
    // Jossain rajojen muokkausmoodeista:
    // vain OK/CANCEL, plus/miinus valideja
    if(moodi & MOODI_MUOKKAA){
        switch(nappitila->muutokset_alas){
            case NAPPI_OK:
                serialLog("Muokkaus tallenna");
                return moodi|MOODI_TALLENNA;
            case NAPPI_CANCEL:
                serialLog("Muokkaus peruuta");
                return MOODI_NORMAALI;
            case NAPPI_YLOS:
                serialLog("Muokkaus plussaa");
                return moodi|MOODI_PLUS;
            case NAPPI_ALAS:
                serialLog("Muokkaus miinusta");
                return moodi|MOODI_MIINUS;
            // Muokkausmoodista pois:
            // jos oltiin muuttujan rajamuokkauksessa, normimoodiin
            // ja muutoin kyseisen arvon näyttömoodiin
            case NAPPI_CO2:
                return moodi&MOODI_CO2 ? MOODI_NORMAALI : MOODI_CO2;
            case NAPPI_KOSTEUS:
                return moodi&MOODI_KOSTEUS ? MOODI_NORMAALI : MOODI_KOSTEUS;
            case NAPPI_LAMPOTILA:
                return moodi&MOODI_LAMPOTILA ? MOODI_NORMAALI : MOODI_LAMPOTILA;
            default:
                serialLog("Muokkaus ei-validi");
                return moodi;
        }
    }
    // Ei muokkausmoodissa mutta jossain arvonäytössä:
    // joko vaihdetaan näkymää tai palataan normaalitilaan
    if(moodi & (MOODI_CO2|MOODI_KOSTEUS|MOODI_LAMPOTILA)){
        // Togglataan moodia
        switch(nappitila->uusi_painallus){
            case NAPPI_CO2:
                if(!(moodi & MOODI_CO2)){
                    return MOODI_CO2;
                }
                return MOODI_NORMAALI;
            case NAPPI_KOSTEUS:
                if(!(moodi & MOODI_KOSTEUS)){
                    return MOODI_KOSTEUS;
                }
                return MOODI_NORMAALI;
            case NAPPI_LAMPOTILA:
                if(!(moodi & MOODI_LAMPOTILA)){
                    return MOODI_LAMPOTILA;
                }
                return MOODI_NORMAALI;
            default:
                // Ei voida vielä sanoa etteikö oltaisi menossa muokkaumoodiin
                break;
        }
    }
    // Mittatyyppimoodista muokkausmoodiin
    // pitämällä nappia pohjassa ja +/-
    if(nappitila->edellinen_painallus & NAPPI_CO2){
        switch(nappitila->muutokset_alas){
            case NAPPI_YLOS:
                serialLog("CO2 plussaa");
                return moodi|MOODI_MUOKKAA|MOODI_CO2|MOODI_YLARAJA;
            case NAPPI_ALAS:
                serialLog("CO2 miinusta");
                return moodi|MOODI_MUOKKAA|MOODI_CO2|MOODI_ALARAJA;
            default:
                serialLog("CO2 ei-validi");
                return moodi;
        }
    }
    if(nappitila->edellinen_painallus & NAPPI_KOSTEUS){
        switch(nappitila->muutokset_alas){
            case NAPPI_YLOS:
                serialLog("HUM plussaa");
                return moodi|MOODI_MUOKKAA|MOODI_KOSTEUS|MOODI_YLARAJA;
            case NAPPI_ALAS:
                serialLog("HUM miinusta");
                return moodi|MOODI_MUOKKAA|MOODI_KOSTEUS|MOODI_ALARAJA;
            default:
                serialLog("HUM ei-validi");
                return moodi;
        }
    }
    if(nappitila->edellinen_painallus & NAPPI_LAMPOTILA){
        switch(nappitila->muutokset_alas){
            case NAPPI_YLOS:
                serialLog("TMP plussaa");
                return moodi|MOODI_MUOKKAA|MOODI_LAMPOTILA|MOODI_YLARAJA;
            case NAPPI_ALAS:
                serialLog("TMP miinusta");
                return moodi|MOODI_MUOKKAA|MOODI_LAMPOTILA|MOODI_ALARAJA;
            default:
                serialLog("TMP ei-validi");
                return moodi;
        }
    }
    serialLog("ei-validi");
    return moodi;
}

/* Pääfunktio, joka tarkastaa mitä pitäisi tehdä */
uint16_t ui_mainflow(uint16_t moodi, viesti_t* viesti, mittatulos_t* mittatulos, rajat_t* rajat, uint16_t* aikaa){
    // Normaalimoodi ei tee mitään
    if(!moodi){return moodi;}
    // Rajojen muokkaus
    if(moodi & MOODI_MUOKKAA){
        moodi = muokkaa_rajoja(moodi, rajat);
    }
    uint16_t nayttoluku;
    // Näytä yläraja
    if(moodi & MOODI_YLARAJA){
        switch(moodi & (MOODI_CO2|MOODI_KOSTEUS|MOODI_LAMPOTILA)){
            case MOODI_CO2:
                nayttoluku = rajat->co2_raja_huono;
                break;
            case MOODI_KOSTEUS:
                nayttoluku = rajat->ilmankosteus_raja_hyva;
                break;
            case MOODI_LAMPOTILA:
                nayttoluku = rajat->lampotila_raja_huono;
                break;
        }
        *aikaa += tulosta_lukua(nayttoluku, viesti, NAYTTOTAAJUUS);
        return moodi;
    }
    // Näytä alaraja
    if(moodi & MOODI_ALARAJA){
        switch(moodi & (MOODI_CO2|MOODI_KOSTEUS|MOODI_LAMPOTILA)){
            case MOODI_CO2:
                nayttoluku = rajat->co2_raja_hyva;
                break;
            case MOODI_KOSTEUS:
                nayttoluku = rajat->ilmankosteus_raja_huono;
                break;
            case MOODI_LAMPOTILA:
                nayttoluku = rajat->lampotila_raja_hyva;
                break;
        }
        *aikaa += tulosta_lukua(nayttoluku, viesti, NAYTTOTAAJUUS);
        return moodi;
    }
    // Näytä lukema (ei muita moodivaihtoehtoja)
    switch(moodi & (MOODI_CO2|MOODI_KOSTEUS|MOODI_LAMPOTILA)){
        case MOODI_CO2:
            nayttoluku = mittatulos->co2;
            break;
        case MOODI_KOSTEUS:
            nayttoluku = mittatulos->ilmankosteus;
            break;
        case MOODI_LAMPOTILA:
            nayttoluku = mittatulos->lampotila;
            break;
    }
    *aikaa += tulosta_lukua(nayttoluku, viesti, NAYTTOTAAJUUS);
    return moodi;
}

// Muokkaa CO2-rajoja
void muokkaa_rajoja_co2(uint16_t moodi, rajat_t* rajat){
    // Ei muokata
    if(!(moodi & (MOODI_PLUS|MOODI_MIINUS))){return;}
    uint16_t uusi_arvo;
    // Ylärajan säätö
    if(moodi & MOODI_YLARAJA){
        if(moodi & MOODI_PLUS){
            uusi_arvo = rajat->co2_raja_huono + RAJA_ASKEL_CO2;
        }
        else{
            // (vältetään underflow)
            if(rajat->co2_raja_huono > RAJA_ASKEL_CO2){
                uusi_arvo = rajat->co2_raja_huono - RAJA_ASKEL_CO2;
            }
            else{uusi_arvo = rajat->co2_raja_huono;}
        }
        // Yläraja ei voi kasvaa loputtomiin eikä olla alle alarajan
        if(uusi_arvo < RAJA_CO2_CAP_YLA && uusi_arvo > rajat->co2_raja_hyva){
            rajat->co2_raja_huono = uusi_arvo;
        }
    }
    // Alarajan säätö
    else{
        if(moodi & MOODI_PLUS){
            // Alaraja ei voi olla suurempi kuin yläraja
            uusi_arvo = rajat->co2_raja_hyva + RAJA_ASKEL_CO2;
            if(uusi_arvo < rajat->co2_raja_huono){
                rajat->co2_raja_hyva = uusi_arvo;
            }
        }
        else{
            // Alaraja ei saa mennä miinukselle
            if(rajat->co2_raja_hyva > RAJA_ASKEL_CO2){
                uusi_arvo = rajat->co2_raja_hyva - RAJA_ASKEL_CO2;
            }
            else{uusi_arvo = rajat->co2_raja_hyva;}
            if(uusi_arvo > RAJA_CO2_CAP_ALA){
                rajat->co2_raja_hyva = uusi_arvo;
            }
        }
    }
}

// Muokkaa kosteuden rajoja
void muokkaa_rajoja_hum(uint16_t moodi, rajat_t* rajat){
    // Ei muokata
    if(!moodi & (MOODI_PLUS|MOODI_MIINUS)){return;}
    uint16_t uusi_arvo;
    // Ylärajan säätö
    if(moodi & MOODI_YLARAJA){
        if(moodi & MOODI_PLUS){
            uusi_arvo = rajat->ilmankosteus_raja_hyva + RAJA_ASKEL_KOSTEUS;
        }
        else{
            // (vältetään underflow)
            if(rajat->ilmankosteus_raja_hyva > RAJA_ASKEL_KOSTEUS){
                uusi_arvo = rajat->ilmankosteus_raja_hyva - RAJA_ASKEL_KOSTEUS;
            }
            else{uusi_arvo = rajat->ilmankosteus_raja_hyva;}
        }
        // Yläraja ei voi kasvaa loputtomiin eikä olla alle alarajan
        if(uusi_arvo < RAJA_KOSTEUS_CAP_YLA && uusi_arvo > rajat->ilmankosteus_raja_huono){
            rajat->ilmankosteus_raja_hyva = uusi_arvo;
        }
    }
    // Alarajan säätö
    else{
        if(moodi & MOODI_PLUS){
            // Alaraja ei voi olla suurempi kuin yläraja
            uusi_arvo = rajat->ilmankosteus_raja_huono + RAJA_ASKEL_KOSTEUS;
            if(uusi_arvo < rajat->ilmankosteus_raja_hyva){
                rajat->ilmankosteus_raja_huono = uusi_arvo;
            }
        }
        else{
            // Alaraja ei saa mennä miinukselle
            if(rajat->ilmankosteus_raja_huono > RAJA_ASKEL_KOSTEUS){
                uusi_arvo = rajat->ilmankosteus_raja_huono - RAJA_ASKEL_KOSTEUS;
            }
            else{uusi_arvo = rajat->ilmankosteus_raja_huono;}
            if(uusi_arvo > RAJA_KOSTEUS_CAP_ALA){
                rajat->ilmankosteus_raja_huono = uusi_arvo;
            }
        }
    }
}

// Muokkaa lämpötilan rajoja
void muokkaa_rajoja_tmp(uint16_t moodi, rajat_t* rajat){
    // Ei muokata
    if(!moodi & (MOODI_PLUS|MOODI_MIINUS)){return;}
    uint16_t uusi_arvo;
    // Ylärajan säätö
    if(moodi & MOODI_YLARAJA){
        if(moodi & MOODI_PLUS){
            uusi_arvo = rajat->lampotila_raja_huono + RAJA_ASKEL_LAMPOTILA;
        }
        else{
            // (vältetään underflow)
            if(rajat->lampotila_raja_huono > RAJA_ASKEL_LAMPOTILA){
                uusi_arvo = rajat->lampotila_raja_huono - RAJA_ASKEL_LAMPOTILA;
            }
            else{uusi_arvo = rajat->lampotila_raja_huono;}
        }
        // Yläraja ei voi kasvaa loputtomiin eikä olla alle alarajan
        if(uusi_arvo < RAJA_LAMPOTILA_CAP_YLA && uusi_arvo > rajat->lampotila_raja_hyva){
            rajat->lampotila_raja_huono = uusi_arvo;
        }
    }
    // Alarajan säätö
    else{
        if(moodi & MOODI_PLUS){
            // Alaraja ei voi olla suurempi kuin yläraja
            uusi_arvo = rajat->lampotila_raja_hyva + RAJA_ASKEL_LAMPOTILA;
            if(uusi_arvo < rajat->lampotila_raja_huono){
                rajat->lampotila_raja_hyva = uusi_arvo;
            }
        }
        else{
            // Alaraja ei saa mennä miinukselle
            if(rajat->lampotila_raja_hyva > RAJA_ASKEL_LAMPOTILA){
                uusi_arvo = rajat->lampotila_raja_hyva - RAJA_ASKEL_LAMPOTILA;
            }
            else{uusi_arvo = rajat->lampotila_raja_hyva;}
            if(uusi_arvo > RAJA_LAMPOTILA_CAP_ALA){
                rajat->lampotila_raja_hyva = uusi_arvo;
            }
        }
    }
}


// Muokkaa asetettuja rajoja
uint16_t muokkaa_rajoja(uint16_t moodi, rajat_t* rajat){
    uint16_t uusi_arvo, rajoitus;
    if(moodi & (MOODI_PLUS|MOODI_MIINUS)){
        switch(moodi & (MOODI_CO2|MOODI_KOSTEUS|MOODI_LAMPOTILA)){
            // CO2
            case MOODI_CO2:
                muokkaa_rajoja_co2(moodi, rajat);
                break;
            // HUM
            case MOODI_KOSTEUS:
                muokkaa_rajoja_hum(moodi, rajat);
                break;
            // TMP
            case MOODI_LAMPOTILA:
                muokkaa_rajoja_tmp(moodi, rajat);
                break;
        }
    // Poistetaan plus-/miinusflagit
    moodi &= ~(MOODI_PLUS|MOODI_MIINUS);
    }
    return moodi;
}
