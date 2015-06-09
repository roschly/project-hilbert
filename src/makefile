SOURCES=main.c serial.c dynamixel.c dxl_hal.c
local:
	avr-gcc -O2 -mmcu=atmega2561 -std=gnu99 -I ../include/ -Wall ${SOURCES} -o ../dist/main.elf
	avr-objcopy -O ihex -R .eeprom -R .fuse -R .lock -R .signature ../dist/main.elf ../dist/main.hex
clean:
	rm -f *.o *.out *.gch *.a *.hex *.elf *.exe
