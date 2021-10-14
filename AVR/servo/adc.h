#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/*================================================================
[Arduino UNO]
ADMUX: REFS1 | REFS0 | ADLAR | MUX4~0
ADCSRA: ADEN | ADSC | ADATE | ADIF | ADIE | ADPS2 | ADPS1 | ADPS0
ADCSRB: R | ACME | R | R | R | ADTS2 | ADTS1 | ADTS0
==================================================================*/

void ADC_init() {
    DDRC = 0b00000000;    
    ADCSRA = 0b10000111; 
    ADCSRB = 0b00000000;
    ADMUX = 0b01000000;
}

void ADC_set_channel(uint8_t channel) {
    channel &= 0b00001111;
    ADMUX |= channel;
}

void ADC_start() {
    ADCSRA |= 0b01000000; // ADSC
}

void ADC_stop() {
    ADCSRA &= ~(0b01000000);
}

void ADC_quit() {
    ADCSRA = 0b00000000;
}

uint32_t ADC_read() { 
    ADC_start();
    while(!(ADCSRA & 0b00010000));
    while((ADCSRA & 0b01000000));
    _delay_us(1);
    ADCSRA |= 0b00010000; // set ADIF
    return ADC;
}