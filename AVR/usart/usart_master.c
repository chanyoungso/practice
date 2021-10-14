#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "usart.h"
#include "lcd.h"

#define LEN 18

int main() {
  char buffer[LEN] = "Yeri fighting!!\r\n";
  char cur_row = 0;
  char cur_col = 0;
  uint8_t counter = 0;
  uint8_t data = ' ';
  USART_init();
  LCD_init();  

  while(1) {

    if ( counter >= LEN ) {
      counter = 0;
    }
    
    if (cur_col >= 16) {
      cur_col = 0;
      if (cur_row == 0) {
        cur_row = 1;
      }
      else {
        cur_row = 0;
      }
    }
    LCD_pos(cur_row, cur_col);
    // USART_tx( buffer[counter]);
    // LCD_char( buffer[counter] );
    data = USART_rx();
    LCD_char( data );
    _delay_ms(10);
    cur_col++;
    counter++;
  }
  return 0;
}