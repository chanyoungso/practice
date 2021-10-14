#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

/*==================================================
[Arduino UNO]
OC0A: PD6 ardupin06
OC0B: PD5 ardupin05
OC1A: PB1 ardupin09
OC1B: PB2 ardupin10
OC2A: PB3 ardupin11
OC2B: PD3 ardupin03

TCCR1A: COM1A1 | COM1A0 | COM1B1 | COM1B0 | R | R | WGM11 | WGM10
TCCR1B: ICNC1 | ICES1 | R | WGM13 | WGM12 | CS12 | CS 11 | CS10
==================================================*/

void PWM_init() {
  TCNT1 = 0;
  ICR1 = 4999;
  // WGM = 14, Fast PWM, non-inverting, prescale(clk / 64)
  TCCR1A = 0b10100010;
  TCCR1B = 0b00011011;
  DDRB = 0b00001110;  // PB1:OC1A, PB2:OC1B, PB3:OC2A
  DDRD = 0b01101000;  // PD3:OC2B, PD5:OC0B, PD6:OC0A
  PORTB = 0b00000000;
  PORTD = 0b10000000;
}

void PWM_set(int duty) {
    OCR1A = duty;
}