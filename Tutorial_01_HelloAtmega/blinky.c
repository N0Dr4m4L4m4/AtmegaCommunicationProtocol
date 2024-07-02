/* 
    Author:  Robin Gerstmann 
    Mail:    robin.gerstmann@s2020.tu-chemnitz.de
    Date:    04.11.2021
    Fuction: Turn off and on LED by changeable delay value
*/

/* CPU cycles for delay fuction needed */
#define F_CPU 12000000UL
/* baudrate value for serial communication */
#define BAUD 9600
/* calculated UBBR value like mentioned in datasheet */
#define MYUBRR F_CPU/16/BAUD-1

/* includes */
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

/* preconfigured delay */
/* variables used in ISR need to be volatile */ 
volatile uint8_t delay = 100;

/* functions for USART serial communication */
void USART_Init(unsigned int ubrr);
void USART_Transmit(unsigned char data);
void USART_Transmit_String(char *s);

/* workaround because of const value expecting _delay_ms function */
void delay_ms(uint8_t milliseconds);

int main()
{
    /* init usart serial communication */
    USART_Init(MYUBRR);
    /* Set ports as output */
    DDRD = (1<<PD4) | (1<<PD5) | (1<<PD6) | (1<<PD7);
    /* Set outputs to ports on PB regsiter */
    DDRB = (1<<PB0) | (1<<PB1) | (1<<PB2) | (1<<PB3) | (1<<PB4) | (1<<PB5);
    /* loop */
	while(1)
	{
        /* for each LED set on PORTD register */
        for(int i = 4; i < 8; i++)
        {
            /* LED 3 - 7 */
            PORTD |= (1 << i);
            /* Send debug infos */
            USART_Transmit_String("LED ");
            USART_Transmit((char) i + '0');
            USART_Transmit_String(": ON \n");
            /* delay fuction */
            delay_ms(delay);
        }
        /* for each other LED on PORTB register */
        for(int i = 0; i < 7; i++)
        {
            /* LED 8 - 12 */
            PORTB |= (1 << i);
            /* Send debug infos */
            USART_Transmit_String("LED ");
            USART_Transmit((char) i + '0');
            USART_Transmit_String(": ON \n");
            /* delay fuction */
            delay_ms(delay);
        }
        /* wait 1s */
        _delay_ms(1000);
        /* reset all ports */
        USART_Transmit_String("ALL LED OFF\n");
        PORTD = 0x00;
        PORTB = 0x00;
    }
}

/* basic configuration of USART*/
void USART_Init(unsigned int ubrr)
{
    /* Turn on ISR*/
    /* Set baud rate */
    UBRR0H = (unsigned char)(ubrr>>8);
    UBRR0L = (unsigned char)ubrr;
    /* Enable receiver and transmitter */
    UCSR0B = (1<<RXEN0)|(1<<TXEN0)|(1 << RXCIE0);;
    /* Set frame format: 8data, 2stop bit */
    UCSR0C = (1<<USBS0)|(3<<UCSZ00);
    /* Turn on ISR routine */
    sei();
}

/* sending one char */
void USART_Transmit(unsigned char data)
{
    /* Wait for empty transmit buffer */
    while (!(UCSR0A & (1<<UDRE0)))
    ;
    /* Put data into buffer, sends the data */
    UDR0 = data;
}

/* help fuction for transmitting strings*/
void USART_Transmit_String(char *s)
{
	while (*s)
	{   /* send string */
		USART_Transmit(*s);
		s++;
	}
}

/* extra delay fuction to change delay dynamically */
void delay_ms(uint8_t milliseconds)
{
    while(milliseconds > 0)
    {
        _delay_ms(1);
        milliseconds--;
    }
    return;
}

/* ISR routine if 1 byte was received*/
ISR(USART_RX_vect)
{
    /* save received value in delay variable*/
    delay = UDR0;
    /* some debug output */
    USART_Transmit_String("Delay updated\n");
}


