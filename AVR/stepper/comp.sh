avr-gcc -Os -std=c99 -DF_CPU=16000000UL -mmcu=atmega328p -c -o stepper.o stepper.c
avr-gcc -mmcu=atmega328p stepper.o -o stepper
avr-objcopy -O ihex -R .eeprom stepper stepper.hex
avrdude -F -V -c arduino -p ATMEGA328P -P /dev/tty.usbmodem14101 -b 115200 -U flash:w:stepper.hex
