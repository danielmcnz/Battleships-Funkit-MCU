# File:   Makefile
# Author: C. Hall-Fernandez, D. Mcgregor
# Date:   19/10/2022
# Descr:  Makefile for 'Battleships' game

# Definitions.
CC = avr-gcc
CFLAGS = -mmcu=atmega32u2 -Os -Wall -Wstrict-prototypes -Wextra -g -I../../drivers -I../../fonts -I../../drivers/avr -I../../utils
OBJCOPY = avr-objcopy
SIZE = avr-size
DEL = rm


# Default target.
all: game.out


# Compile: create object files from C source files.

# --------------------------------------------
# Src

game.o: game.c defs.h player.h renderer.h packet.h map.h random.h ../../drivers/avr/system.h ../../fonts/font3x5_1.h ../../drivers/button.h ../../drivers/display.h ../../drivers/navswitch.h ../../fonts/font5x7_1.h ../../utils/font.h ../../utils/pacer.h ../../utils/tinygl.h ../../drivers/avr/ir_uart.h
	$(CC) -c $(CFLAGS) $< -o $@

player.o: player.c defs.h ../../drivers/avr/system.h ../../drivers/navswitch.h
	$(CC) -c $(CFLAGS) $< -o $@

renderer.o: renderer.c defs.h map.h ../../utils/tinygl.h ../../fonts/font3x5_1.h
	$(CC) -c $(CFLAGS) $< -o $@

packet.o: packet.c defs.h map.h ../../drivers/avr/ir_uart.h
	$(CC) -c $(CFLAGS) $< -o $@

map.o: map.c defs.h random.h
	$(CC) -c $(CFLAGS) $< -o $@

# --------------------------------------------


# --------------------------------------------
# Dependencies

pio.o: ../../drivers/avr/pio.c ../../drivers/avr/pio.h ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

system.o: ../../drivers/avr/system.c ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

timer.o: ../../drivers/avr/timer.c ../../drivers/avr/system.h ../../drivers/avr/timer.h
	$(CC) -c $(CFLAGS) $< -o $@

display.o: ../../drivers/display.c ../../drivers/avr/system.h ../../drivers/display.h ../../drivers/ledmat.h
	$(CC) -c $(CFLAGS) $< -o $@

navswitch.o: ../../drivers/navswitch.c ../../drivers/avr/delay.h ../../drivers/avr/pio.h ../../drivers/avr/system.h ../../drivers/navswitch.h
	$(CC) -c $(CFLAGS) $< -o $@

ledmat.o: ../../drivers/ledmat.c ../../drivers/avr/pio.h ../../drivers/avr/system.h ../../drivers/ledmat.h
	$(CC) -c $(CFLAGS) $< -o $@

font.o: ../../utils/font.c ../../drivers/avr/system.h ../../utils/font.h
	$(CC) -c $(CFLAGS) $< -o $@

pacer.o: ../../utils/pacer.c ../../drivers/avr/system.h ../../drivers/avr/timer.h ../../utils/pacer.h
	$(CC) -c $(CFLAGS) $< -o $@

tinygl.o: ../../utils/tinygl.c ../../drivers/avr/system.h ../../drivers/display.h ../../utils/font.h ../../utils/tinygl.h
	$(CC) -c $(CFLAGS) $< -o $@

button.o: ../../drivers/button.c ../../drivers/button.h ../../drivers/avr/system.h  ../../drivers/avr/pio.h
	$(CC) -c $(CFLAGS) $< -o $@

ir_uart.o: ../../drivers/avr/ir_uart.c ../../drivers/avr/ir_uart.h ../../drivers/avr/system.h ../../drivers/avr/usart1.h ../../drivers/avr/timer0.h ../../drivers/avr/pio.h ../../drivers/avr/delay.h
	$(CC) -c $(CFLAGS) $< -o $@

usart1.o: ../../drivers/avr/usart1.c ../../drivers/avr/usart1.h ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

timer0.o: ../../drivers/avr/timer0.c ../../drivers/avr/timer0.h ../../drivers/avr/system.h ../../drivers/avr/bits.h ../../drivers/avr/prescale.h
	$(CC) -c $(CFLAGS) $< -o $@

prescale.o: ../../drivers/avr/prescale.c ../../drivers/avr/prescale.h ../../drivers/avr/system.h
	$(CC) -c $(CFLAGS) $< -o $@

# --------------------------------------------



# Link: create ELF output file from object files.
game.out: game.o player.o renderer.o packet.o pio.o system.o timer.o display.o ledmat.o font.o pacer.o tinygl.o navswitch.o button.o ir_uart.o usart1.o timer0.o prescale.o map.o
	$(CC) $(CFLAGS) $^ -o $@ -lm
	$(SIZE) $@


# Target: clean project.
.PHONY: clean
clean: 
	-$(DEL) *.o *.out *.hex


# Target: program project.
.PHONY: program
program: game.out
	$(OBJCOPY) -O ihex game.out game.hex
	dfu-programmer atmega32u2 erase; dfu-programmer atmega32u2 flash game.hex; dfu-programmer atmega32u2 start


