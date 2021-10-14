#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "stepper.h"


ISR(INT1_vect) {
    STEPPER_rotate(120, BY_STEP, CW);
}

ISR(INT0_vect) {
    STEPPER_rotate(120, BY_STEP, CCW);
}

int main() {
  DDRB = 0b00001111;
  DDRD = 0b00000000;
  EICRA = 0b00000000;
  EIMSK = 0b00000011;
  sei();
  while(1) {
    
    
  }  
}