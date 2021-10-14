#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define ROT_INTERVAL_US 1000
#define DEG_PER_STEP 5.625 / 64
#define STEPS 8
#define ROT_PIN_MSK 0b00001111

/* ===========
[28BYJ-48 Pin Map]
orange: PB3
yellow: PB2
pink:   PB1
blue:   PB0

[Buttons]
CW_rotate: PB5
CCW_rotate: PB4
============= */
typedef enum { CW, CCW } rot_dir_t;
typedef enum { BY_DEG, BY_STEP} rot_mth_t;
uint8_t rot_seq[STEPS] = { 0b00000111, 0b00000011, 0b00001011, 0b00001001, 0b00001101, 0b00001100, 0b00001110, 0b00000110 };

void STEPPER_rotate(int param, rot_mth_t mth, rot_dir_t dir) {
  int num_step = 0;
  if ( mth = BY_DEG ) {
    num_step = param / DEG_PER_STEP;
  }
  else if ( mth = BY_STEP ) {
    num_step = param;
  }  

  if ( dir == CW ) {
    for (int i = 0; i < num_step; i++) { 
      PORTB = (rot_seq[i % STEPS] & ROT_PIN_MSK);
      _delay_ms(1);    
    }
  }
  else if ( dir == CCW ) {
    for (int i = 0; i < num_step; i++) { 
      PORTB = (rot_seq[STEPS - (i % STEPS) - 1] & ROT_PIN_MSK);
      _delay_ms(1);    
    }    
  }
  PORTB = 0b00000000;
}
