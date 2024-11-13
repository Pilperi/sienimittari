/*
Mittasysteemin datatyyppien händläilyfunktiot sun muut.
*/

#include <stdlib.h>
#include "co2_datatyypit.hpp"

/* Luonti- ja tuhoamisfunktiot */
viesti_t* uusi_viesti(){
    viesti_t* viesti = (viesti_t*)malloc(sizeof(viesti_t));
    viesti->tavut.tavut_l = 0;
    viesti->tavut.tavut_m = 0;
    viesti->tavut.tavut_h = 0;
    return(viesti);
}
void tuhoa_viesti(viesti_t* viesti){
    free(viesti);
    return;
}
mittatulos_t* uusi_mittatulos(){
    mittatulos_t* mittatulos = (mittatulos_t*)malloc(sizeof(mittatulos_t));
    mittatulos->mstatus.data = 0;
    mittatulos->co2 = 0;
    mittatulos->ilmankosteus = 0;
    mittatulos->lampotila = 0;
    return(mittatulos);
}
void tuhoa_mittatulos(mittatulos_t* mittatulos){
    free(mittatulos);
    return;
}
rajat_t* uudet_rajat(){
    rajat_t* rajat = (rajat_t*)malloc(sizeof(rajat_t));
    rajat->co2_raja_huono = 1000;
    rajat->co2_raja_hyva = 700;
    rajat->lampotila_raja_huono = 25;
    rajat->lampotila_raja_hyva = 20;
    rajat->ilmankosteus_raja_huono = 65;
    rajat->ilmankosteus_raja_hyva = 80;
    return(rajat);
}
void tuhoa_rajat(rajat_t* rajat){
    free(rajat);
    return;
}

/*
Muunna mittarin lukuarvot ymmärrettävään muotoon ja täytä mittatulokseen.
Muunnoslaskukaavat datasheetissä.
*/
void paivita_mittatulos(mittatulos_t* mtulos, uint8_t* lukuarvot){
    // Hiilidioksidi on suoraan ppm
    mtulos->co2 = (uint16_t)(lukuarvot[0] << 8 | lukuarvot[1]);
    // Lämpötila celsiuksina
    mtulos->lampotila = (uint16_t)(-45.0 + 175.0 * ((uint16_t)(lukuarvot[3] << 8 | lukuarvot[4])) / 0xFFFF);
    // Ilmankosteus prosenteiksi
    mtulos->ilmankosteus = (uint16_t)(100.0 * ((uint16_t)(lukuarvot[6] << 8 | lukuarvot[7])) / 0xFFFF);
}

/*
Vertaile mittatulosta mittasetupin rajoihin, ja täytä statuskentät oikein
*/
void paivita_mittastatus(mittatulos_t* mtulos, rajat_t* rajat){
    mtulos->mstatus.data = 0;
    // CO2 NOK
    if(mtulos->co2 >= rajat->co2_raja_huono){
        mtulos->mstatus.status.co2_nok = 1;
    }
    else{mtulos->mstatus.status.co2_nok = 0;}
    // CO2 OK
    if(mtulos->co2 < rajat->co2_raja_hyva){
        mtulos->mstatus.status.co2_ok = 1;
    }
    else{mtulos->mstatus.status.co2_ok = 0;}
    
    // Ilmankosteus NOK
    if(mtulos->ilmankosteus < rajat->ilmankosteus_raja_huono){
        mtulos->mstatus.status.ilmankosteus_nok = 1;
    }
    else{mtulos->mstatus.status.ilmankosteus_nok = 0;}
    // Ilmankosteus OK
    if(mtulos->ilmankosteus >= rajat->ilmankosteus_raja_hyva){
        mtulos->mstatus.status.ilmankosteus_ok = 1;
    }
    else{mtulos->mstatus.status.ilmankosteus_ok = 0;}
    
    // Lämpötila NOK
    if(mtulos->lampotila > rajat->lampotila_raja_huono){
        mtulos->mstatus.status.lampotila_nok = 1;
    }
    else{mtulos->mstatus.status.lampotila_nok = 0;}
    // Lämpötila OK
    if(mtulos->lampotila >= rajat->lampotila_raja_hyva){
        mtulos->mstatus.status.lampotila_ok = 1;
    }
    else{mtulos->mstatus.status.lampotila_ok = 0;}
}

void paivita_valot_viestiin(mittatulos_t* mtulos, viesti_t* viesti){
    viesti->kentat.statusvalot = mtulos->mstatus.data;
    return;
}
