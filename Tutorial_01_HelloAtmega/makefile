TARGET=blinky
MCU=atmega328p
SOURCES=blinky

# execute hex and clean
all: hex clean flash

# rule 1
hex: $(TARGET).hex
# rule 2
# convert .bin into hex based file
$(TARGET).hex: $(TARGET).bin
	avr-objcopy -j .text -j .data -O ihex $(SOURCES).bin $(SOURCES).hex
# --- rule 3 --- 
# -O 	optimazitaion
# -Wall	advanced error msg
# -g 	Debug-Infos
# -o	name of output
$(TARGET).bin:
	avr-gcc -Wall -g -Os -mmcu=$(MCU) -o $(SOURCES).bin $(SOURCES).c
# upload to avr chip
# -c programmer
# -p which chip
flash:
	avrdude -c gpio -p m328p -U flash:w:$(TARGET).hex
# clean 
clean:
	rm -f *.bin 