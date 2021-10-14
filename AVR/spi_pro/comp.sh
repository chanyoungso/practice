avr-gcc -Os -std=c99 -DF_CPU=16000000UL -mmcu=atmega328p -c -o spi_rx.o spi_rx.c
avr-gcc -mmcu=atmega328p spi_rx.o -o spi_rx
avr-objcopy -O ihex -R .eeprom spi_rx spi_rx.hex
avrdude -F -V -c arduino -p ATMEGA328P -P /dev/tty.usbmodem14101 -b 115200 -U flash:w:spi_rx.hex
