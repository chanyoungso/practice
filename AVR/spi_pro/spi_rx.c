#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>
#include "spi.h"
#include "usart.h"
#include "lcd.h"

#define DISP_LEN 16

enum high_low { high, low };
void disp_loop();

int main()
{
//   USART_init(b19200);
    SPI_init(spi_slave);
    LCD_init();
    disp_loop();
    return 0;
}

void disp_loop()
{  
    LCD_clear();
    LCD_str("READY>");
    char cur_row = 0;
    char cur_col = 6;
    unsigned char data = ACK;

  // LCD Loop
    while (1)
    {
        // LCD Cursor Positioning
        if (cur_col >= DISP_LEN) {
            cur_col = 0;
            if (cur_row == 0) {
                cur_row = 1;
            }
            else {
                cur_row = 0;
            }
        }
        LCD_pos(cur_row, cur_col); 

        // rx and disp
        data = SPI_char( 0x30 );
        LCD_char(data);
        cur_col++;
    }
}