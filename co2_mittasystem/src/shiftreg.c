#include <stdint.h>
#include <avr/io.h>
#include "shiftreg.h"
#include "i2c.h"

// Funktioprototyypit
static void shiftreg_setup(void);
static void shiftreg_laheta(uint8_t arvo);
static void shiftreg_vapauta(void);
static void shiftreg_latch(void);

/* Lähetä viesti siirtorekistereille ja latchaa. */
void shiftreg_laheta_viesti(shiftreg_viesti_t* viesti_p, uint8_t latch)
{
    shiftreg_setup();
    shiftreg_laheta(viesti_p->tavut.tavut_h);
    shiftreg_laheta(viesti_p->tavut.tavut_m);
    shiftreg_laheta(viesti_p->tavut.tavut_l);
    if (latch){shiftreg_latch();}
    shiftreg_vapauta();
}

/* Aseta SPI-väylä valmiiksi siirto-operaatioita varten.
Pinnit ei riitä niin käytetään I2C-väylää datan kellotukseen.
Ei pitäisi olla väliä kun ei lähetetä aloitus- tai lopetussignaaleja
jotka I2C-väylän laitteet bongaisi.
*/
static void shiftreg_setup(void)
{
    SHIFTREG_TWI_DDR |= (1<<SHIFTREG_PIN_DO)|(1<<SHIFTREG_PIN_USCK)|(1<<SHIFTREG_LATCH_PIN);
}


/* Lähetä kahdeksan bittiä tavaraa.
Tarvii tehdä bit-bang koska USI:n SPI on niin että DO on PINB1
mutta I2C-dataväylä on PINB0...
*/
static void shiftreg_laheta(uint8_t arvo)
{
    for (uint8_t cnt=0; cnt<8; cnt++)
    {
        SHIFTREG_TWI_PORT &= ~(1<<SHIFTREG_PIN_USCK);
        if(arvo & (0x80>>cnt)){
            SHIFTREG_TWI_PORT |= (1<<SHIFTREG_PIN_DO);
        }
        else{
            SHIFTREG_TWI_PORT &= ~(1<<SHIFTREG_PIN_DO);
        }
        SHIFTREG_TWI_PORT |= (1<<SHIFTREG_PIN_USCK);
        while (!(SHIFTREG_TWI_PORT & (1<<SHIFTREG_PIN_USCK))){;}
        SHIFTREG_TWI_PORT &= ~(1<<SHIFTREG_PIN_USCK);
    }
}

/* Vapauta SPI-väylä niin sitä voidaan käyttää I2C-juttuihin. */
static void shiftreg_vapauta(void)
{
    USISR = I2C_USISR_CLR;
    USICR = 0x00;
}

/* Lähetä latch-signaali */
static void shiftreg_latch(void)
{
    SHIFTREG_LATCH_DDR |= (1<<SHIFTREG_LATCH_PIN);
    SHIFTREG_LATCH_PORT &= ~(1<<SHIFTREG_LATCH_PIN);
    SHIFTREG_LATCH_PORT |= (1<<SHIFTREG_LATCH_PIN);
    __asm__("nop");
    SHIFTREG_LATCH_PORT &= ~(1<<SHIFTREG_LATCH_PIN);
}
