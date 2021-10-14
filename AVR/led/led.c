#include <avr/io.h>
#include <util/delay.h>
#include "8x8dotmatrix.h"

#define BLINK_DELAY_MS 10000
#define LINE_DELAY_US 350
#define CHAR_DELAY_TIMES 50
#define LENGTH 13
#define SPA_COL 2

// Global Variables
char ver_switch[] = {0b00000001, 0b00000010, 0b00000100, 0b00001000};

// Functions
void display_char(char* cur_char);

int main() {
  // Pin Settings
  DDRD = 0b11111111;
  DDRB = 0b00001111;
  DDRC = 0b00001111;

  // Initialize Pins D0-7: cathode horizontal, B0-3 & C0-3: anode vertical
  PORTC = 0b00000000;
  PORTB = 0b00000000;

  // Generate Text Sequence
  char text[][8] = { H, A, P, P, Y, N, E, W, Y, E, A, R, EXCLA};
  char cur_char[8];
  for(int cur_row = 0; cur_row < 8; cur_row++) {
    cur_char[cur_row] = 0b11111111;
  }

  while(1) {
    for(int cur_seq = 0; cur_seq < LENGTH; cur_seq++) {
      for(int cur_col = 0; cur_col < 8; cur_col++) {
        for(int cur_row = 0; cur_row < 8; cur_row++) {
          cur_char[cur_row] =  cur_char[cur_row] << 1;
          cur_char[cur_row] |= ( (text[cur_seq][cur_row] & (0b10000000 >> cur_col)) >> (7 - cur_col) );
        }
        display_char( cur_char );
      }
      for(int cur_col = 0; cur_col < SPA_COL; cur_col++) {
        for(int cur_row = 0; cur_row < 8; cur_row++) {
          cur_char[cur_row] =  cur_char[cur_row] << 1;
          cur_char[cur_row] |= 0b00000001;
        }
        display_char( cur_char );
      }
    }
  }
}

void display_char(char* cur_char) {
  for(int cur_time = 0; cur_time < CHAR_DELAY_TIMES; cur_time++) {
    //PORTC = 0b00000000;
    int cur_line = 0;
    for(int cur_c = 0; cur_c < 4; cur_c++) {
      PORTC = ver_switch[cur_c];
      PORTD = cur_char[cur_line++];
      _delay_us(LINE_DELAY_US);
    }
    PORTC = 0b00000000;
    for(int cur_b = 0; cur_b < 4; cur_b++) {
      PORTB = ver_switch[cur_b];
      PORTD = cur_char[cur_line++];
      _delay_us(LINE_DELAY_US);
    }
    PORTB = 0b00000000;
  }
}
