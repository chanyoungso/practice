#include <avr/io.h>
#include <util/delay.h>

#define ACK 0x7E

#define SPI_DIR     DDRB
#define SPI_BUS     PORTB
#define RST_DIR     DDRD
#define RST_BUS     PORTD
#define RST_PIN     0x08 // special

#define MOSI_BIT    0x08
#define MISO_BIT    0x10
#define SCK_BIT     0x20
#define SS_BIT      0x14

#define SPIF_BIT    0x80
#define SPE_BIT     0x40
#define MSTR_BIT    0x10
#define SPR0_BIT    0x01

#define STR_DELAY_US 1000

typedef enum { spi_sig_low, spi_sig_high } spi_sig_type;
typedef enum { spi_master, spi_slave } spi_comm_type;

uint8_t SPI_char(uint8_t data);
uint8_t SPI_str(unsigned char *str);
void SPI_init();
void SPI_rst_in();
void SPI_stop();
void SPI_sck(spi_sig_type clk);
void SPI_rst(spi_sig_type sig);

void SPI_init(spi_comm_type type)
{
    if ( !type ) {
        SPI_DIR = (MOSI_BIT | SCK_BIT | SS_BIT);
        SPCR = (SPE_BIT | MSTR_BIT | SPR0_BIT);
        SPI_BUS &= ~SS_BIT; // Beobachtung
    }

    else {
        SPI_BUS = MISO_BIT;
        SPCR = SPE_BIT;
    }
}

void SPI_stop() {
    SPI_DIR &= ~(MOSI_BIT | SCK_BIT | SS_BIT);
    SPCR = 0x00;
    SPI_BUS = 0x00;
}

void SPI_rst_in() {
    RST_DIR |= RST_PIN;
}

void SPI_sck(spi_sig_type clk) {
    if ( clk ) {
        SPI_BUS |= SCK_BIT; // Beobachtung
    }
    else {
        SPI_BUS &= ~SCK_BIT; // Beobachtung
    }
}

void SPI_rst(spi_sig_type sig) {
    if (sig) {
        RST_BUS |= RST_PIN;
        SPI_BUS |= SS_BIT;
    }
    else {
        RST_BUS &= ~RST_PIN;
        SPI_BUS &= ~SS_BIT;
    }
}

uint8_t SPI_char(uint8_t data)
{
    SPDR = data;
    while (!(SPSR & SPIF_BIT)); // SPIF 1
    return SPDR;
}

uint8_t SPI_str(unsigned char* str)
{
    uint8_t data;
    for (int i = 0; str[i] != 0; i++)
    {
        data = SPI_char(str[i]);
        _delay_us(STR_DELAY_US);
    }
    return data;
}

// uint8_t SPI_str2(uint8_t* str) {
//     uint8_t data;
//     while (*str != '\0') {
//         data = SPI_char( *(str++));
//         _delay_us(STR_DELAY_US);
//     }
//     return data;
// }
