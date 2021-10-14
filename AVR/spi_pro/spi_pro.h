#include <avr/io.h>
#include <util/delay.h>
#include <stdint.h>
#include "spi.h"

#define PRO_EN_REQ      0xAC
#define PRO_EN_ACK      0x53

#define ERASE_1         0xAC
#define ERASE_2         0x80

#define READ_MASK       0xFF
#define HIGH_MASK       0x08
#define READ_PM         0x20
#define LOAD_PMP        0x40
#define WRITE_PMP       0x4C
#define READ_EEPROM     0xA0
#define WRITE_EEPROM    0xC0

// Read & Write lock bit
#define READ_LB_1       0x58
#define READ_LB_2       0x00
#define WRITE_LB_1      0xAC
#define WRITE_LB_2      0xE0
#define READ_SIGN       0x30

// Read fuse
#define READ_LF_1       0x50
#define READ_LF_2       0x00

#define READ_HF_1       0x58
#define READ_HF_2       0x08

#define READ_EF_1       0x50
#define READ_EF_2       0x08

// Write fuse
#define WRITE_LF_1      0xAC
#define WRITE_LF_2      0xA0

#define WRITE_HF_1      0xAC
#define WRITE_HF_2      0xA8

#define WRITE_EF_1      0xAC
#define WRITE_EF_2      0xA4

#define DONT_CARE       0x00

#define TWD_FLASH       5
#define TWD_FUSE        5
#define TWD_ERASE       10
#define TWD_EEPROM      10

enum fuse_type { lf, hf, ef };
enum addr_type { addr_low, addr_high };
enum sign_addr { addr_0, addr_1, addr_2 };

uint8_t SPI_com(uint8_t com0, uint8_t com1, uint8_t com2, uint8_t com3);
uint8_t SPI_erase();
uint8_t SPI_load_pmp(uint8_t addr, uint8_t data, enum addr_type hl);
uint8_t SPI_write_pmp(uint8_t addr_h, uint8_t addr_l);
uint8_t SPI_read_pm(uint8_t addr_h, uint8_t addr_l, enum addr_type hl);
uint8_t SPI_read_lb();
uint8_t SPI_read_sign(enum sign_addr);
uint8_t SPI_write_lb(uint8_t data);
uint8_t SPI_read_fuse(enum fuse_type f_type);
uint8_t SPI_write_fuse(uint8_t data, enum fuse_type f_type);
uint8_t SPI_read_EEPROM(uint8_t addr_h, uint8_t addr_l);
uint8_t SPI_write_EEPROM(uint8_t addr_h, uint8_t addr_l, uint8_t data);
void SPI_pro_init();

// Functions
uint8_t SPI_com(uint8_t com0, uint8_t com1, uint8_t com2, uint8_t com3) {
    SPI_char(com0);
    SPI_char(com1);
    SPI_char(com2);
    return SPI_char(com3);
}

uint8_t SPI_erase() {
    uint8_t buff = SPI_com(ERASE_1, ERASE_2, DONT_CARE, DONT_CARE);
    _delay_ms(TWD_ERASE);
    return buff;
}

uint8_t SPI_load_pmp(uint8_t addr_l, uint8_t data, enum addr_type hl ) {
    return SPI_com((LOAD_PMP | (hl * HIGH_MASK)), DONT_CARE, addr_l, data);
}

uint8_t SPI_write_pmp(uint8_t addr_h, uint8_t addr_l) {
    uint8_t buff = SPI_com(WRITE_PMP, addr_h, addr_l, DONT_CARE);
    _delay_ms(TWD_FLASH);
    return buff;
}

uint8_t SPI_read_pm(uint8_t addr_h, uint8_t addr_l, enum addr_type hl) {
    return SPI_com((READ_PM | (hl * HIGH_MASK)), addr_h, addr_l, READ_MASK);
}

uint8_t SPI_read_lb() {
    return SPI_com(READ_LB_1, READ_LB_2, DONT_CARE, READ_MASK);
}

uint8_t SPI_write_lb(uint8_t data) {
    uint8_t buff = SPI_com(WRITE_LB_1, WRITE_LB_2, DONT_CARE, (data |= 0xC0));
    _delay_ms(TWD_FUSE);
    return buff;
}

uint8_t SPI_read_sign(enum sign_addr sign) {
    return SPI_com(READ_SIGN, DONT_CARE, (DONT_CARE | sign), READ_MASK);

}

uint8_t SPI_read_fuse(enum fuse_type f_type ) {
    switch (f_type) {
        case lf:
            return SPI_com(READ_LF_1, READ_LF_2, DONT_CARE, READ_MASK);
            break;
        case hf:
            return SPI_com(READ_HF_1, READ_HF_2, DONT_CARE, READ_MASK);
            break;
        case ef:
            return SPI_com(READ_EF_1, READ_EF_2, DONT_CARE, READ_MASK);
            break;
        default:
            return 0;
            break;
    }   
}

uint8_t SPI_write_fuse(uint8_t data, enum fuse_type f_type) {
    uint8_t buff;
    switch (f_type) {
        case lf:
            buff = SPI_com(WRITE_LF_1, WRITE_LF_2, DONT_CARE, data);
            break;
        case hf:
            buff = SPI_com(WRITE_HF_1, WRITE_HF_2, DONT_CARE, data);
            break;
        case ef:
            buff = SPI_com(WRITE_EF_1, WRITE_EF_2, DONT_CARE, data);
            break;
        default:
            break;
    }
    _delay_ms(TWD_FUSE);
    return buff;
}

uint8_t SPI_read_EEPROM(uint8_t addr_h, uint8_t addr_l) {
    return SPI_com(READ_EEPROM, addr_h, addr_l, READ_MASK);
}

uint8_t SPI_write_EEPROM(uint8_t addr_h, uint8_t addr_l, uint8_t data) {
     uint8_t buff = SPI_com(WRITE_EEPROM, addr_h, addr_l, data);
     _delay_ms(TWD_EEPROM);
     return buff;
}

void SPI_pro_init()
{
    uint8_t data = 0x00;
    SPI_rst_in();
    SPI_rst(sig_high);
    SPI_init();

    while ( data != PRO_EN_ACK ) {
        _delay_ms(100);
        SPI_sck(sig_low); // beobachtung
        SPI_rst(sig_high);
        _delay_us(10);
        SPI_rst(sig_low);
        _delay_ms(25);
        SPI_char(PRO_EN_REQ);
        SPI_char(PRO_EN_ACK);
        data = SPI_char(DONT_CARE);
        SPI_char(DONT_CARE);
    }
}



