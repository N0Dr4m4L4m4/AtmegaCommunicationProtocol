# RaspNet - EmbeddedLab Wintersemester 21/22

## Author:  Robin Gerstmann

Technologie
========================
## Programmiersprache
Für das verwendete Protokoll wurde die Programmiersprache C verwendet.

## Hardware
+ Raspberry Pi
+ Gertboard
+ Mikrocontroller: Atmega328p von Atmel (jetzt Microchip)

## Toolchain
+ Betriebsystem: UNIX basierende Systeme sind empfohlen. Unter Windows mit zusätzlichen Programmen jedoch auch möglich. 
+ Serielle Kommunikation: USART-Protokoll wird hier verwendet. 

> **Kommando:** 
> minicom ama0 -H

+ Compiler: avr-gcc cross-compiler für AVR basierende Mikrocontroller

+ avrdude: AVRDUDE nutzt ISP um den Mikrocontroller zu flashen


## Installation
+ Minicom installation in Linux (Debian, Ubuntu, Kali, Mint)
> **Kommando:** 
> sudo apt install minicom -y

+ Minicom installation in Linux (Fedora, CentOS, RHEL)
> **Kommando:**
> sudo yum install minicom -y

+ avrdude installation in Linux
> **Kommando:**
> sudo apt-get install avrdude

+ AVR tool chain installation:
> **Kommando:**
> sudo apt-get install gcc-avr binutils-avr avr-libc

## Anmerkung
+ Die Dokumentation ist allgemein in Englisch beschrieben. Nur diese Seite wurde auf deutsch geschrieben.

## Referenzlinks:
+ [ RaspNet protocol ]: [ https://osg.informatik.tu-chemnitz.de/lehre/emblab/protocol_final_4stud.pdf ]
+ [Gertboard Manual]: [ https://osg.informatik.tu-chemnitz.de/lehre/emblab/gertboard_manual_2.0.pdf ]
+ [AVR]: [ https://osg.informatik.tu-chemnitz.de/lehre/emblab/atmel.pdf ]
+ [avrdude]: [https://www.nongnu.org/avrdude/ ]
+ [avr-gcc]: [https://gcc.gnu.org/wiki/avr-gcc ]
+ [Minicom]: [https://salsa.debian.org/minicom-team/minicom5 ] 
+ [Raspberry Pi]: [https://www.raspberrypi.org/ ]