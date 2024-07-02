/* 
    Author:  Robin Gerstmann 
    Mail:    robin.gerstmann@s2020.tu-chemnitz.de
    Date:    21.11.2021
    Fuction: Protocol Layer-I
*/

#define F_CPU 12000000UL

#include <avr/io.h>
#include <util/delay.h>
/* include my own files from other c files*/
#include "timer.h"
#include "layer2.h"

/* PORTS for transmission */
#define TX_PORT PB5
#define CLK_PORT PB4

int main()
{
    /* set up mask for PORTS */
    uint8_t mask = 1 << TX_PORT | 1 << CLK_PORT; 
    /* Init messageServiceTX */
    InitMessageServiceTX(PRE_1024, &DDRB, &mask);
    /* Init messageServiceRX*/
    InitMessageServiceRX();
    /* send some values every 5 seconds*/
    uint8_t msg[] = {0x74,0x65,0x73,0x74};
    uint8_t n = (sizeof(msg) / sizeof(msg[0]));
    while (1)
    {
        SendMessage(msg,n);
        CheckMessage();
    }
}