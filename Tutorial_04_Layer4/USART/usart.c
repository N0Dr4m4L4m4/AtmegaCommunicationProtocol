#include <avr/io.h>
#include <avr/interrupt.h>
/**
 * @brief Is used to initialize the USART interface for serial communication
 * 
 * @param ubrr calculated UBRR value (see ATmega doc for more information)
 * @param isr_on Should receiver interrupt service routine be enabled
 */
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
/**
 * @brief Send one character via the serial interface
 * 
 * @param data Character e.g in hex, octal or binary
 */
void USART_Transmit(unsigned char data)
{
    /* Wait for empty transmit buffer */
    while (!(UCSR0A & (1<<UDRE0)))
    ;
    /* Put data into buffer, sends the data */
    UDR0 = data;
}
/**
 * @brief  Sends more than one character via the serial interface
 * 
 * @param s String of characters 
 */
void USART_Transmit_String(char *s)
{
	while (*s)
	{   /* send string */
		USART_Transmit(*s);
		s++;
	}
}
