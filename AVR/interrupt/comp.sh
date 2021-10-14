avr-gcc -Os -std=c99 -DF_CPU=16000000UL -mmcu=atmega328p -c -o interrupt.o interrupt.c
avr-gcc -mmcu=atmega328p interrupt.o -o interrupt
avr-objcopy -O ihex -R .eeprom interrupt interrupt.hex
avrdude -F -V -c arduino -p ATMEGA328P -P /dev/tty.usbmodem14201 -b 115200 -U flash:w:interrupt.hex