SOURCES=main.c
local:
	avr-gcc -O2 -mmcu=atmega2561 -std=gnu99 -Wall main.c -o main.elf
	avr-objcopy -O ihex -R .eeprom -R .fuse -R .lock -R .signature main.elf main.hex
clean:
	rm -f *.o *.out *.gch *.a *.hex *.elf *.exe