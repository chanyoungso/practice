avr-gcc -Os -std=c99 -DF_CPU=16000000UL -mmcu=atmega328p -c -o dcmotor.o dcmotor.c
avr-gcc -mmcu=atmega328p dcmotor.o -o dcmotor
avr-objcopy -O ihex -R .eeprom dcmotor dcmotor.hex
avrdude -F -V -c arduino -p ATMEGA328P -P /dev/tty.usbmodem14201 -b 115200 -U flash:w:dcmotor.hex