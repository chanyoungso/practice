#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#ifndef F_CPU
#define F_CPU 16000000UL
#endif

// Baud Rate Prescale
#define BR_L_4800       0xCF
#define BR_L_9600       0x67
#define BR_L_19200      0x33      
#define BR_L_38400      0x19
#define BR_L_57600      0x10
#define BR_L_76800      0x0C
#define BR_L_115200     0x08

// UCSRA bit
#define RXC_BIT         0x80
#define TXC_BIT         0x40
#define UDRE_BIT        0x20
#define FE_BIT          0x10
#define DOR_BIT         0x08
#define PE_BIT          0x04
#define U2X_BIT         0x02
#define MPCM_BIT        0x01

// UCSRB bit
#define RXCIE_BIT       0x80
#define TXCIE_BIT       0x40
#define UDRIE_BIT       0x20
#define RXEN_BIT        0x10
#define TXEN_BIT        0x08
#define UCSZ2_BIT       0x04
#define RXB8_BIT        0x02
#define TXB8_BIT        0x01

// UCSRC bit
#define URSEL_BIT       0x80
#define UMSEL_BIT       0x40
#define UPM1_BIT        0x20
#define UPM0_BIT        0x10
#define USBS_BIT        0x08
#define UCSZ1_BIT       0x04
#define UCSZ0_BIT       0x02
#define UCPOL_BIT       0x01

void USART_init();
void USART_tx(uint8_t data);
uint8_t USART_rx();
void USART_tx_bin(uint8_t data);
void USART_tx_hex(uint8_t data);
void USART_tx_nib(uint8_t data);

void USART_init()
{
    // UCSR0A = 0x00;
    UCSR0B |= ( TXEN_BIT | RXEN_BIT );
    UCSR0C |= ( URSEL_BIT | UCSZ0_BIT | UCSZ1_BIT );
    UBRR0L = BR_L_9600;
    UBRR0H = 0x00;
}

void USART_tx(uint8_t data) {
    while( !( UCSR0A & UDRE_BIT ) );
    UDR0 = data;
}

void USART_tx_str(unsigned char* str) { // Beobachtung
    for (int i = 0; str[i] != 0; i++)
    {
        while (!(UCSR0A & UDRE_BIT));
        UDR0 = str[i];
    }    
    
}

uint8_t USART_rx() {
    while( !( UCSR0A & RXC_BIT ) );
    return UDR0;
}

void USART_tx_bin(uint8_t data) {
  for (int i = 0; i < 8; i++) {
    if ( (data >> (7 - i)) & 0x01 ) {
      USART_tx('1');
    }
    else {
      USART_tx('0');
    }   
  }
}

void USART_tx_hex(uint8_t data) {
  USART_tx_str("0x");
  USART_tx_nib(data >> 4);
  USART_tx_nib(data);
}

void USART_tx_nib(uint8_t data) {
  data &= 0x0F;
  if ( data < 0x0A ) {
    USART_tx( data + 0x30 );
  }
  else {
    USART_tx( data + 0x37 );
  }
}