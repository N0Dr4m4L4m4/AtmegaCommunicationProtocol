#include <avr/io.h>
#include <avr/interrupt.h>

/* basic configuration of USART*/
void USART_Init(unsigned int ubrr, int isr_on)
{
    /* Turn on ISR*/
    /* Set baud rate */
    UBRR0H = (unsigned char)(ubrr>>8);
    UBRR0L = (unsigned char)ubrr;
    /* Enable receiver and transmitter */
    UCSR0B = (1<<RXEN0)|(1<<TXEN0);
    /* Set frame format: 8data, 2stop bit */
    UCSR0C = (1<<USBS0)|(3<<UCSZ00);
    if(isr_on != 0)
    {
        UCSR0B |= (1 << RXCIE0);
        sei();
    }
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
