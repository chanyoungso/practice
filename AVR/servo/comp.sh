avr-gcc -Os -std=c99 -DF_CPU=16000000UL -mmcu=atmega328p -c -o servo.o servo.c
avr-gcc -mmcu=atmega328p servo.o -o servo
avr-objcopy -O ihex -R .eeprom servo servo.hex
avrdude -F -V -c arduino -p ATMEGA328P -P /dev/tty.usbmodem14101 -b 115200 -U flash:w:servo.hex
