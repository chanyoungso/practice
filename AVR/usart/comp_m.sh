avr-gcc -Os -std=c99 -DF_CPU=16000000UL -mmcu=atmega328p -c -o usart_master.o usart_master.c
avr-gcc -mmcu=atmega328p usart_master.o -o usart_master
avr-objcopy -O ihex -R .eeprom usart_master usart_master.hex
avrdude -F -V -c arduino -p ATMEGA328P -P /dev/tty.usbmodem14101 -b 115200 -U flash:w:usart_master.hex
