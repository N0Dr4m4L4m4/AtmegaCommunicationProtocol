FILENAME   = main
TIMER  	   = Layer1/timer
LAYER1	   = Layer1/layer1
LAYER2	   = Layer2/layer2
LAYER3	   = Layer3/layer3
IRQHANDLER = IRQHandler/irqhandler
CRC32	   = CRC32/crc32
USART	   = USART/usart
BUFFER	   = Buffer/buffer
DEVICE     = atmega328p
PROGRAMMER = gpio
BAUD       = 115200
COMPILE    = avr-gcc -Wall -Os -mmcu=$(DEVICE)


default: compile upload clean

compile:
	$(COMPILE) -c $(FILENAME).c -o $(FILENAME).o
	$(COMPILE) -c $(TIMER).c -o $(TIMER).o
	$(COMPILE) -c $(USART).c -o $(USART).o
	$(COMPILE) -c $(CRC32).c -o $(CRC32).o
	$(COMPILE) -c $(LAYER1).c -o $(LAYER1).o
	$(COMPILE) -c $(LAYER2).c -o $(LAYER2).o
	$(COMPILE) -c $(LAYER3).c -o $(LAYER3).o
	$(COMPILE) -c $(BUFFER).c -o $(BUFFER).o
	$(COMPILE) -c $(IRQHANDLER).c -o $(IRQHANDLER).o
	$(COMPILE) -o $(FILENAME).elf $(FILENAME).o $(TIMER).o $(USART).o $(LAYER1).o $(LAYER2).o $(LAYER3).o $(CRC32).o $(IRQHANDLER).o $(BUFFER).o
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
	rm $(LAYER3).o
	rm $(CRC32).o
	rm $(IRQHANDLER).o
	rm $(BUFFER).o
	rm $(FILENAME).elf
	rm $(FILENAME).hex