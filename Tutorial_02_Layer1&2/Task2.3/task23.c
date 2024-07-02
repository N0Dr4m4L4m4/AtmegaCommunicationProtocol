/* 
    Author:  Robin Gerstmann 
    Mail:    robin.gerstmann@s2020.tu-chemnitz.de
    Date:    20.11.2021
    Fuction: Timer overflow test with ISR
*/
#define F_CPU 12000000UL
#include "usart.c"
#include <avr/interrupt.h>
#include <util/delay.h>

/* CPU cycles for delay fuction needed */

/* baudrate value for serial communication */
#define BAUD 9600
/* calculated UBBR value like mentioned in datasheet */
#define MYUBRR F_CPU/16/BAUD-1
/* Init for timer 0 8-BIT timer*/
void InitTimer();

int main()
{
    InitTimer();
    USART_Init(MYUBRR,0);
    while (1)
    {
        USART_Transmit((unsigned char)'M');
    }
}

void InitTimer(){
    /* OVF ISR*/
    TIMSK0 |= 1 << TOIE0;
    /* enable isr */
    sei();
    /* prescaler clk/128 and start timer*/
    TCCR0B |= 1 << CS00 | 1 << CS02;
}
/* ISR for overflow */
ISR(TIMER0_OVF_vect)
{
    USART_Transmit((unsigned char)'I');
}
