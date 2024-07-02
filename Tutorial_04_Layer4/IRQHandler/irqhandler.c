#include <avr/io.h>
#include <avr/interrupt.h>
#include "../Buffer/buffer.h"
#include "../Layer1/layer1.h"
#include "../IRQHandler/irqhandler.h"
#include "../USART/usart.h"

volatile uint8_t byteCntrTx = 0; /** @brief byteCntrTx byte counter for transmission one byte 1 - 8 */
volatile uint8_t buffCntrTx = 0; /** @brief buffCntrTx buffer counter for transmission one byte 0 - 255 */
volatile uint8_t buffCntrRx = 0; /** @brief buffCntrRx byte counter for receiving one byte 1 - 8 */
volatile uint8_t byteCntrRx = 0; /** @brief byteCntrRx buffer counter for receiving one byte 0 - 255 */

/**
 * @brief Construct a new ISR timer interrupt. Every interrupt is checking if a package is ready to send
 * 
 */
ISR(TIMER0_OVF_vect)
{  
    if(!TX_SERV.MUTEX && TX_SERV.READY)
    {
        TX_SERV.MUTEX = 1;
            /* write 1 */
            if (((TX_SERV.BUFFER[buffCntrTx] >> byteCntrTx) & 0x01) == 1)
            {
                PORTB |= 1 << PB5;
            }
            /* write 0 */
            else
            {
                PORTB &= ~(1<<PB5);
            }
            /* increment counter */
            byteCntrTx++;
            /* if one byte is frinish of the sending buffer */
            if(byteCntrTx == 8)
            {
                //USART_Transmit(TX_SERV.BUFFER[buffCntrTx]);
                /* reset the byte counter*/
                byteCntrTx = 0;
                /* are we at the end of the buffer */
                if(buffCntrTx == TX_SERV.SIZE-1)
                {
                    
                    buffCntrTx = 0;
                    TX_RESET();
                }
                /* go to the next byte of the buffer*/
                else
                {
                    buffCntrTx++;  
                }
            }
        TX_SERV.MUTEX = 0;
        }
    /* toggle pin to send clk */
    PORTB ^= 1 << PB4;   
}
/**
 * @brief Construct a new ISR port interrupt. Every edge activates an interrupt. And the port status will be read.
 * 
 */
ISR(PCINT2_vect)
{
    /* check if receiver buffer is ready */
    if(!RX_SERV.MUTEX)
    {
        /* lock buffer */
        RX_SERV.MUTEX = 1;
        /* Buffer needs to be empty*/
        if(RX_SERV.FULL_FLAG == 0)
        {
            /* is HIGH value on PD5 */
            if((PIND & (1 << PIND5))) 
            {
                /* write 1 in receive buffer on RxCounter position */
                RX_SERV.BUFFER[buffCntrRx] |= (1 << byteCntrRx);
            } 
            /* is LOW value on PD5 */
            else if(!(PIND & (1 << PIND5))) 
            {
                /* write 0 in receive buffer on RxCounter position */
            RX_SERV.BUFFER[buffCntrRx] &= ~(1 << byteCntrRx);
            } 
            /* position of buffer + 1 */
            byteCntrRx++;
            /* checks preample */
            if(buffCntrRx > 1 && RX_SERV.BUFFER[buffCntrRx] != 0x01 && RX_SERV.PREAMPLE_FLAG == 0)
            {
                buffCntrRx = 0;
                byteCntrRx = 0;
            }
        }
        /* after every byte check*/
        if (byteCntrRx == 8) 
        {
            /* check if buffer has preample inside*/
            //USART_Transmit(RX_SERV.BUFFER[buffCntrRx]);
            if(RX_SERV.BUFFER[buffCntrRx] != 0x7E && RX_SERV.PREAMPLE_FLAG == 0)
            {
                buffCntrRx = 0;
                byteCntrRx = 0;
            }
            /* if the given byte was a preample */
            else
            {
                /* set up flag */
                RX_SERV.PREAMPLE_FLAG = 1;
            }
            /* if the preample was received */
            if(RX_SERV.PREAMPLE_FLAG == 1)
            {
                byteCntrRx = 0;
                buffCntrRx++;
                /* for paylod informaion */
                switch(buffCntrRx)
                {
                    case 5:
                        /* crc32 was received*/
                        RX_SERV.CRC32_FLAG = 1;
                        break;
                    case 6:
                        /* size of payload was received*/
                        RX_SERV.SIZE_FLAG = 1;
                        RX_SERV.SIZE = RX_SERV.BUFFER[5];
                        break;
                }
                /* are we at the end of the payload ?*/
                if((buffCntrRx == RX_SERV.SIZE + 6) && RX_SERV.SIZE_FLAG == 1)
                {
                    /* set up flags for ready*/
                    RX_SERV.PAYLOAD_FLAG = 1;
                    RX_SERV.FULL_FLAG = 1;
                    /* reset buff counter */
                    buffCntrRx = 0;
                }
            }
        }
        /* unlock receiver buffer */
        RX_SERV.MUTEX = 0;
    }
}