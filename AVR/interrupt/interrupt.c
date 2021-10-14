#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

ISR(INT1_vect) {
  if ( PORTB & 0b00000010 ) {
    PORTB &= 0b11111101;
  }
  else {
    PORTB |= 0b00000010;
  }  
}

ISR(INT0_vect) {
  if ( PORTB & 0b00000001 ) {
    PORTB &= 0b11111110;
  }
  else {
    PORTB |= 0b00000001;
  }  
}

int main() {

  DDRB = 0b00000011;  
  DDRD = 0b00000000;
  EICRA = 0b00000000;
  EIMSK = 0b00000011;
  sei();
  PORTB = 0b00000011;

  while(1) {

    //    

  }
  return 0;
}

