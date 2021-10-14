#include <avr/io.h>
#include <util/delay.h>

#define SPECIAL

// Normal Setting: Control (C0-3), Data(D0-7)
#ifndef SPECIAL
#define DATA_DIR DDRD
#define DATABUS PORTD
#define CONTROL_DIR DDRC
#define CONTROLBUS PORTC
#define RS_BIT              0b00000001
#define RW_BIT              0b00000010
#define E_BIT               0b00000100
#endif

// Special Setting: Control (RS:D3, RW:D2, E:C4), Data(PC0-3,PD4-7)
#ifdef SPECIAL
#define DATA_DIR_H DDRD
#define DATA_DIR_L DDRC
#define DATABUS_H PORTD
#define DATABUS_L PORTC
#define RS_DIR DDRD
#define RS_BUS PORTD
#define RS_BIT 0b00001000
#define RW_DIR DDRD
#define RW_BUS PORTD
#define RW_BIT 0b00000100
#define E_DIR DDRC
#define E_BUS PORTC
#define E_BIT 0b00010000
#endif

#define MAX_LINES           2
#define MAX_CHARS           16

//===== Timing Control =====
#define BOOTUP_MS           15
#define ENABLE_US           20
#define WRITEDATA_US        46
#define COMMAND_US          42
#define RESET_MS            5
#define RESET_ONE_MS        1
#define SET4BITMODE_MS      5
#define CLEAR_MS            2
#define HOME_MS             2
#define INIT_MS             50
#define POWERON_MS          20

//===== Instructions =====
#define COM_CLEAR_DISPLAY   0x01
#define COM_RETURN_HOME     0x02

//===== Entry Mode Set =====
#define COM_SET_ENTRY       0x04
#define COM_ENTRY_DECREASE  0x00
#define COM_ENTRY_INCREASE  0x02
#define COM_ENTRY_NOSHIFT   0x00
#define COM_ENTRY_SHIFT     0x01

//===== Display ON/OFF Control =====
#define COM_SET_DISPLAY     0x08
#define COM_DISPLAY_OFF     0x00
#define COM_DISPLAY_ON      0x04
#define COM_CURSOR_OFF      0x00
#define COM_CURSOR_ON       0x02
#define COM_BLINKING_OFF    0x00
#define COM_BLINKING_ON     0x01

//===== Cursor or Display shift =====
#define COM_SET_SHIFT       0x10
#define COM_CURSOR_MOVE     0x00
#define COM_DISPLAY_SHIFT   0x08
#define COM_SHIFT_LEFT      0x00
#define COM_SHIFT_RIGHT     0x04

//===== Function Set =====
#define COM_SET_FUNCTION    0x20
#define COM_FUNC_4BIT       0x00
#define COM_FUNC_8BIT       0x10
#define COM_FUNC_1L         0x00
#define COM_FUNC_2L         0x08
#define COM_FUNC_5x7        0x00
#define COM_FUNC_5x10       0x04

//===== Set CGRAM Address =====
#define COM_SET_CGADR       0x40

//===== Set DDRAM Address =====
#define COM_SET_DDADR       0x80

#define COM_LINE_01  0x80
#define COM_LINE_02  0xC0

void LCD_init();
uint8_t LCD_func();
uint8_t LCD_disp();
void LCD_datapath();
void LCD_clear();
void LCD_home();
void LCD_en();
void LCD_pos(char row, char col);
void LCD_com(unsigned char command);
void LCD_char(unsigned char data);
void LCD_data(unsigned char data);
void LCD_str(char* str);
// void LCD_str2(char* str);

void LCD_init() {
    uint8_t func = LCD_func();
    uint8_t disp = LCD_disp();
    _delay_ms(INIT_MS);
    LCD_datapath();
    RS_DIR |= RS_BIT;
    RW_DIR |= RW_BIT;
    E_DIR |= E_BIT;
    _delay_ms(POWERON_MS);    
    LCD_com(func);
    LCD_com(disp);
    LCD_clear();
    LCD_home();
}

uint8_t LCD_func() {
    return COM_SET_FUNCTION | COM_FUNC_8BIT | COM_FUNC_2L | COM_FUNC_5x7;
}

uint8_t LCD_disp() {
    return COM_SET_DISPLAY | COM_DISPLAY_ON | COM_CURSOR_ON;
}

#ifndef SPECIAL
void LCD_datapath() {
    DATA_DIR = 0xFF;
}
#endif

#ifdef SPECIAL
void LCD_datapath() {
    DATA_DIR_H |= 0xF0; // special
    DATA_DIR_L |= 0x0F; // special
}
#endif

void LCD_clear() {
    LCD_com(COM_CLEAR_DISPLAY);
    _delay_ms(CLEAR_MS);
}

void LCD_home() {
    LCD_com(COM_LINE_01);
    _delay_ms(HOME_MS);
};

void LCD_pos(char row, char col) {
    char pos;
    switch (row) {
        case 0:
            pos = COM_LINE_01;
            break;
        case 1:
            pos = COM_LINE_02;
            break;
        default:
            return;
            break;
    }   
    if (col < MAX_CHARS) {
        pos = pos + col;
        LCD_com(pos);
    }
}

#ifndef SPECIAL
void LCD_data(unsigned char data) {
    DATABUS = data;
}
#endif

#ifdef SPECIAL
void LCD_data(unsigned char data) {
    uint8_t nib;
    nib = ((data & 0xF0) >> 4);
    for (int i = 0; i < 4; i++) {
        if ( (nib >> i) & 0x01 ) {
            DATABUS_H |= (0x01 << (i + 4) );
        }
        else {
            DATABUS_H &= ~(0x01 << (i + 4));
        }                             
    }

    nib = (data & 0x0F);
    for (int i = 0; i < 4; i++) {
        if ((nib >> i) & 0x01) {
            DATABUS_L |= (0x01 << i);
        }
        else {
            DATABUS_L &= ~(0x01 << i);
        }
    }
}
#endif

void LCD_com(unsigned char command) {
    LCD_data( command );
    RS_BUS &= ~RS_BIT;
    RW_BUS &= ~RW_BIT;
    LCD_en();
}

void LCD_char(unsigned char data) {
    LCD_data( data );
    RS_BUS |= RS_BIT;
    RW_BUS &= ~RW_BIT;
    LCD_en();
}

void LCD_str(char* str) {
    for (int i = 0; str[i] != 0; i++) {
        LCD_char(str[i]);
    }
}

void LCD_en() {
    E_BUS |= E_BIT;
    _delay_us(1);
    E_BUS &= ~E_BIT;
    _delay_ms(1);
}
