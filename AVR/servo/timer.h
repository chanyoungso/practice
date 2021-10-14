#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

/*==================================================
[Arduino UNO]
TCCR0A: COM0A1 | COM0A0 | COM0B1 | COM0B0 | R | R | WGM01 | WGM00
TCCR1B: FOC0A | FOC0B | R | R | WGM02 | CS0[2:0]
TIMSK0: R | R | R | R | R | OCIEB | OCIEA | TOIE
==================================================*/

void TIMER_init() {
    TCNT0 = 0;
    OCR0A = 99;
    TCCR0A = 0b01000010;
}

void TIMER_start() {
    TCCR0B |= 0b00000010;
}

void TIMER_stop() {
    TCCR0B &=~(0b00000010);
    TIMSK0 &=~(0b00000010);
}