#include <avr/io.h>
#include <util/delay.h>

#define DIFF_U 10
#define DIFF_D 10
#define DUR 200
#define D_MAX 255
#define D_MIN 150

int main() {
  DDRB = 0b11111111;
  DDRD = 0b11111111;
  PORTB = 0b00000000;
  PORTD = 0b00000000;
  
  // OC0A: PD6 ardupin06
  // OC0B: PD5 ardupin05
  // OC1A: PB1 ardupin09
  // OC1B: PB2 ardupin10
  // OC2A: PB3 ardupin11
  // OC2B: PD3 ardupin03

  TCCR0A = 0b01101001; // WGM:11(Fast), COM:10(Non inverse), CS:001(No prescale)
  TCCR0B = 0b01100001; // WGM:10(Phase Correct), COM:10(Non inverse), CS:001(No prescale)
  TCCR1A = 0b01101001; // WGM:11(Fast), COM:10(Non inverse), CS:001(No prescale)
  TCCR1B = 0b01100001; // WGM:10(Phase Correct), COM:10(Non inverse), CS:001(No prescale)
  TCCR2A = 0b01101001; // WGM:11(Fast), COM:10(Non inverse), CS:001(No prescale)
  TCCR2B = 0b01100001; // WGM:10(Phase Correct), COM:10(Non inverse), CS:001(No prescale)
  
  int duty = D_MIN;
  
  while(1) {
    for (duty = D_MIN; duty < D_MAX; duty = duty + DIFF_U) {      
      OCR1B = duty;
      _delay_ms(DUR);
    }
    for (duty = D_MAX; duty > D_MIN; duty = duty - DIFF_D)
    {      
      OCR1B = duty;
      _delay_ms(DUR);
    }    
  }
  return 0;
}

