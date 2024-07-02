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
#include "Layer1/timer.h"
#include "Layer2/layer2.h"
#include "Layer3/layer3.h"
#include "USART/usart.h"

#define MYUBRR 12000000UL/16/9600-1
/* PORTS for transmission */
#define TX_PORT PB5
#define CLK_PORT PB4

int main()
{
    /* for some debug tests*/
    USART_Init(MYUBRR,0);
    /* set up mask for PORTS */
    uint8_t mask = 1 << TX_PORT | 1 << CLK_PORT; 
    /* Init messageServiceTX */
    InitMessageServiceTX(PRE_256, &DDRB, &mask);
    /* Init messageServiceRX*/
    InitMessageServiceRX();
    /* 
    uint8_t msg[] = {0x64,0x65,0x66};
    uint8_t src_addr = 0x09;
    uint8_t dst_addr = 0x01;
    uint8_t n = (sizeof(msg) / sizeof(msg[0]));
    */
    while (1)
    {
        //sendPackage(&dst_addr,&src_addr,msg,n,PRIORITY_LOW);
        checkPackage();
    }
}