#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>
#include "spi_pro.h"
#include "usart.h"
#include "lcd.h"

#define DISP_LEN    16
#define EXIT_NUM    8
#define EXIT_MARK   0xFF
#define HEADER_SIZE  5
#define END_DELAY   1500
#define PAGE_SIZE   256
#define MAX_PAGES   6

typedef struct {
    uint16_t addr;
    uint16_t page_size;
    uint8_t* data;   
} page_t;

enum high_low { high, low };
void EEPROM_check(uint16_t addr);
void EEPROM_write(page_t* pg);
void flash_check(uint16_t addr);
void flash_write(page_t* pg);
uint8_t get_byte(uint16_t wyde, enum high_low hl);
uint16_t set_wyde(uint8_t byte_h, uint8_t byte_l);
void rx_page(page_t* pg);
void chip_check();
void disp_loop();

int main()
{
  uint8_t hd_buff[HEADER_SIZE];
  uint16_t num_page = 0;
  page_t* pages[MAX_PAGES];

  USART_init(b19200);
  LCD_init();
  SPI_pro_init();
  
  while(1) {
    LCD_clear();
    LCD_str("READY>");
    // disp_loop();

    // ===== Receiving Mode Byte =====
    hd_buff[0] = USART_rx();

    // ===== Mode Selection =====
    switch ( hd_buff[0] ) {    
      case 'w': // 0x77
        // ===== Receiving Initial Header ====
        for (int cur_b = 1; cur_b < HEADER_SIZE; cur_b++ ){
          hd_buff[cur_b] = USART_rx();
        }
        
        // ===== Preparing ====
        LCD_clear();
        LCD_str("Data receiving...");
        // rx_data(hd_buff); //
        num_page = set_wyde(hd_buff[1], hd_buff[2]);
        for (int cur_pg = 0; cur_pg < num_page; cur_pg++) {          
          pages[cur_pg] = (page_t*) malloc( sizeof(page_t) );          
        }

        // ===== Data Receiving Loop =====
        // SPI_erase();
        for (int cur_pg = 0; cur_pg < num_page; cur_pg++) {
          rx_page(pages[cur_pg]);
        }

        // ===== Data Writing Loop =====
        LCD_clear();
        LCD_str("Data uploading...");
        for (int cur_pg = 0; cur_pg < num_page; cur_pg++) {
          EEPROM_write( pages[cur_pg] );
        }
        LCD_clear();
        LCD_str( "Upload complete." );
        _delay_ms(END_DELAY);
        break;

      case 'W': // 0x57 
        // ===== Receiving Initial Header ====
        for (int cur_b = 1; cur_b < HEADER_SIZE; cur_b++ ){
          hd_buff[cur_b] = USART_rx();
        }
        
        // ===== Preparing ====
        LCD_clear();
        LCD_str("Data receiving...");
        num_page = set_wyde(hd_buff[1], hd_buff[2]);
        for (int cur_pg = 0; cur_pg < num_page; cur_pg++) {          
          pages[cur_pg] = (page_t*) malloc( sizeof(page_t) );          
        }

        // ===== Data Receiving Loop =====
        // SPI_erase();
        for (int cur_pg = 0; cur_pg < num_page; cur_pg++) {
          rx_page(pages[cur_pg]);
        }

        // ===== Data Writing Loop =====
        SPI_erase();
        LCD_clear();
        LCD_str("Data uploading...");
        for (int cur_pg = 0; cur_pg < num_page; cur_pg++) {
          flash_write( pages[cur_pg] );
        }
        LCD_clear();
        LCD_str( "Upload complete." );
        _delay_ms(END_DELAY);
        break;        
      case 'n':
        for (int i = 1; i < HEADER_SIZE; i++ ){
          USART_rx(); // dump
        }
        LCD_clear();
        LCD_str("Upload aborted.");
        _delay_ms(END_DELAY);
        break;
      case 'e':
        SPI_erase();
        LCD_clear();
        LCD_str("Data erased.");
        USART_tx_str("> Data erased.\n\r");
        _delay_ms(END_DELAY);
        break;
      case 'r':
        LCD_clear();
        LCD_str("EEPROM check...");
        EEPROM_check( 0x0000 );
        EEPROM_check( 0x0100 );
        EEPROM_check( 0x0200 );
        _delay_ms(END_DELAY);
        break;
      case 'f':
        LCD_clear();
        LCD_str("Flash check...");
        flash_check( 0x0000 );
        flash_check( 0x0100 );
        flash_check( 0x0200 );
        _delay_ms(END_DELAY);
        break;
      default: 
        LCD_clear();
        LCD_str("Program end.");
        return -1;
        break;
    };  
  }
  
  return 0;
}


void flash_check(uint16_t addr) {
  uint8_t addr_h = get_byte(addr, high);
  uint8_t addr_l = get_byte(addr, low);
  USART_tx_str("> Check flash\n\r");
  USART_tx_hex(addr_h);
  USART_tx('.');
  USART_tx_hex(addr_l);
  USART_tx_str(": \n\r");
  for (int cur_b = 0; cur_b <= 0xFF; cur_b++)
  {
    USART_tx_hex(SPI_read_pm(addr_h, addr_l + cur_b, high));
    USART_tx('.');
    USART_tx_hex(SPI_read_pm(addr_h, addr_l + cur_b, low));     
    USART_tx(' ');
    if ( (cur_b + 1) % 0x10 == 0x00) {
      USART_tx_str("\n\r");
    }
  }
  USART_tx_str("\n\r");
}


void flash_write(page_t* pg)
{
  uint8_t addr_h = get_byte(pg->addr, high);
  uint8_t addr_l = get_byte(pg->addr, low);
  for (int cur_b = 0; cur_b < pg->page_size; cur_b += 2) {
    if ( (pg->addr) == 0xFFFF ) {
      return;
    }
    SPI_load_pmp(addr_l, *(pg->data + cur_b), addr_low);
    SPI_load_pmp(addr_l, *(pg->data + cur_b + 1), addr_high);
    addr_l++;
  }
  SPI_write_pmp( addr_h, get_byte(pg->addr, low) );
}


void EEPROM_check(uint16_t addr) {
  uint8_t addr_h = get_byte(addr, high);
  uint8_t addr_l = get_byte(addr, low);
  USART_tx_str("> Check EEPROM\n\r");
  USART_tx_hex( addr_h );
  USART_tx('.');
  USART_tx_hex( addr_l );
  USART_tx_str(": \n\r");
  for (int cur_b = 0; cur_b <= 0xFF; cur_b++) {
    USART_tx_hex( SPI_read_EEPROM( addr_h, addr_l + cur_b) );
    USART_tx(' ');
    if ((cur_b + 1) % 0x10 == 0x00) {
      USART_tx_str("\n\r");
    }
  }
}

void EEPROM_write(page_t* pg) {
  uint8_t addr_h = get_byte(pg->addr, high);
  uint8_t addr_l = get_byte(pg->addr, low);
  for (int cur_b = 0; cur_b < pg->page_size; cur_b++) {
    if ( (pg->addr) == 0xFFFF ) {
      return;
    }
    SPI_write_EEPROM( addr_h, addr_l + cur_b, *(pg->data + cur_b) );
  }
}

void chip_check() {
  USART_tx_str("lock bit: ");
  USART_tx_bin(SPI_read_lb());
  USART_tx_str("\n\r");
  USART_tx_str("high fuse: ");
  USART_tx_bin(SPI_read_fuse(hf));
  USART_tx_str("\n\r");
  USART_tx_str("low fuse: ");
  USART_tx_bin(SPI_read_fuse(lf));
  USART_tx_str("\n\r");
  USART_tx_str("extended fuse: ");
  USART_tx_bin(SPI_read_fuse(ef));
  USART_tx_str("\n\r");
}

void disp_loop()
{
  LCD_clear();
  char cur_row = 0;
  char cur_col = 0;
  unsigned char data = 0x00;

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
    data = USART_rx();
    USART_tx( data );
    LCD_char(data);
    cur_col++;
  }
}

uint8_t get_byte(uint16_t wyde, enum high_low hl) {
    uint8_t new_byte = 0x00;
    if ( hl == high ) {
        wyde >>= 8;
        new_byte = wyde; 
    }
    else if ( hl == low ) {
        new_byte = wyde;
    }    
    return new_byte;
}

uint16_t set_wyde(uint8_t byte_h, uint8_t byte_l) {
  uint16_t wyde = 0x0000;
  wyde = byte_h;
  wyde <<= 8;
  wyde |= byte_l;
  return wyde;
}

void rx_page(page_t* pg) {
  uint8_t hd_buff[HEADER_SIZE];
  for (int cur_b = 0; cur_b < HEADER_SIZE; cur_b++) {
      hd_buff[cur_b] = USART_rx();
  }
  pg->page_size = set_wyde(hd_buff[1], hd_buff[2]);
  pg->addr = set_wyde(hd_buff[3], hd_buff[4]);
  pg->data = USART_rx_str( pg->page_size );
}