avr-gcc -Os -std=c99 -DF_CPU=16000000UL -mmcu=atmega328p -c -o usart_slave.o usart_slave.c
avr-gcc -mmcu=atmega328p usart_slave.o -o usart_slave
avr-objcopy -O ihex -R .eeprom usart_slave usart_slave.hex
avrdude -F -V -c arduino -p ATMEGA328P -P /dev/tty.usbmodem14101 -b 115200 -U flash:w:usart_slave.hex
