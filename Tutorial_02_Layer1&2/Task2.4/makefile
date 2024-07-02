FILENAME   = task24
TIMER  	   = timer
LAYER1	   = layer1
LAYER2	   = layer2
USART	   = usart
DEVICE     = atmega328p
PROGRAMMER = gpio
BAUD       = 115200
COMPILE    = avr-gcc -Wall -Os -mmcu=$(DEVICE)


default: compile upload clean

compile:
	$(COMPILE) -c $(FILENAME).c -o $(FILENAME).o
	$(COMPILE) -c $(TIMER).c -o $(TIMER).o
	$(COMPILE) -c $(USART).c -o $(USART).o
	$(COMPILE) -c $(LAYER1).c -o $(LAYER1).o
	$(COMPILE) -c $(LAYER2).c -o $(LAYER2).o
	$(COMPILE) -o $(FILENAME).elf $(FILENAME).o $(TIMER).o $(USART).o $(LAYER1).o $(LAYER2).o
	avr-objcopy -j .text -j .data -O ihex $(FILENAME).elf $(FILENAME).hex 
	avr-size --format=avr --mcu=$(DEVICE) $(FILENAME).elf

upload:
	avrdude -v -p $(DEVICE) -c $(PROGRAMMER) -P $(PORT) -b $(BAUD) -U flash:w:$(FILENAME).hex:i

clean:
	rm $(FILENAME).o
	rm $(TIMER).o
	rm $(USART).o
	rm $(LAYER1).o
	rm $(LAYER2).o
	rm $(FILENAME).elf
	rm $(FILENAME).hex