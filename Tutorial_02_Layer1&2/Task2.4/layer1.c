#include <avr/io.h>
#include <avr/interrupt.h>
#include "buffer.h"
#include "layer1.h"
#include "timer.h"
#include "usart.h"

volatile int i = 0;
volatile int byte_cntr_tx = 0;
volatile int buff_cntr_tx = 0;
volatile int byte_cntr_rx = 0;
volatile int buff_cntr_rx = 0;
/* ISR for overflow */
ISR(TIMER0_OVF_vect)
{  
    if(!TX_SERV.MUTEX)
    {
        TX_SERV.MUTEX = 1;
            /* write 1 */
            if (((TX_SERV.BUFFER[buff_cntr_tx] >> byte_cntr_tx) & 0x01) == 1)
            {
                PORTB |= 1 << PB5;
            }
            /* write 0 */
            else
            {
                PORTB &= ~(1<<PB5);
            }
            /* increment counter */
            byte_cntr_tx++;
            /* if one byte is frinish of the sending buffer */
            if(byte_cntr_tx == 8)
            {
                /* reset the byte counter*/
                byte_cntr_tx = 0;
                /* are we at the end of the buffer */
                if(buff_cntr_tx == TX_SERV.SIZE-1)
                {
                    buff_cntr_tx = 0;
                    StopTimer_0();
                    TX_RESET();
                }
                /* go to the next byte of the buffer*/
                else
                {
                    buff_cntr_tx++;
                    
                }
            }
            /* toggle pin to send clk */
            PORTB ^= 1 << PB4;
        TX_SERV.MUTEX = 0;
        }
        
}
    


void TX_RESET()
{
    /* lock mutex tx*/
    TX_SERV.MUTEX = 1;

    /* reset all flags*/
    TX_SERV.SIZE = 0;
    /* reset buffer */
    for(int i = 0; i < 256; i++)
    {
        TX_SERV.BUFFER[i] = 0x00;
    }
    /* set mutex free */
    TX_SERV.MUTEX = 0;
}

void RX_RESET()
{
    /* reset all flags*/
    RX_SERV.FULL_FLAG = 0;
    RX_SERV.PREAMPLE_FLAG = 0;
    RX_SERV.CRC32_FLAG = 0;
    RX_SERV.SIZE_FLAG = 0;
    /* delete all vars*/
    RX_SERV.SIZE = 0;
    buff_cntr_rx = 0;
    byte_cntr_rx = 0;
    /* reset buffer */
    for(int i = 0; i < 256; i++)
    {
        RX_SERV.BUFFER[i] = 0x00;
    }
}

ISR(PCINT2_vect)
{
    if(!RX_SERV.MUTEX)
    {
        RX_SERV.MUTEX = 1;
        /* Buffer needs to be empty*/
        if(RX_SERV.FULL_FLAG == 0)
        {
            /* is HIGH value on PD5 */
            if((PIND & (1 << PIND5))) 
            {
                /* write 1 in receive buffer on RxCounter position */
                RX_SERV.BUFFER[buff_cntr_rx] |= (1 << byte_cntr_rx);
            } 
            /* is LOW value on PD5 */
            else if(!(PIND & (1 << PIND5))) 
            {
                /* write 0 in receive buffer on RxCounter position */
            RX_SERV.BUFFER[buff_cntr_rx] &= ~(1 << byte_cntr_rx);
            } 
        
            /* position of buffer + 1 */
            byte_cntr_rx++;
        }
            /* check if we are at the end of the buffer to avoid segmentation fault */
        if (byte_cntr_rx == 8) 
        {
                byte_cntr_rx = 0;
                buff_cntr_rx++;
                /* for paylod informaion */
                switch(buff_cntr_rx)
                {
                    case 1:
                        RX_SERV.PREAMPLE_FLAG = 1;
                        //USART_Transmit(0x01);
                        break;
                    case 5:
                        RX_SERV.CRC32_FLAG = 1;
                        //USART_Transmit(0x02);
                        break;
                    case 6:
                        RX_SERV.SIZE_FLAG = 1;
                        RX_SERV.SIZE = RX_SERV.BUFFER[5];
                        break;
                    default:
                        break;
                }
                /* are we at the end of the payload ?*/
                if((buff_cntr_rx == RX_SERV.SIZE+6) && RX_SERV.SIZE_FLAG == 1)
                {
                    RX_SERV.PAYLOAD_FLAG = 1;
                    RX_SERV.FULL_FLAG = 1;
                    USART_Transmit(0xff);
                }
        }
        RX_SERV.MUTEX = 0;
    }
}