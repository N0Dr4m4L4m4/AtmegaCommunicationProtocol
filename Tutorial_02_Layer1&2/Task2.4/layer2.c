#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "usart.h"
#include "layer2.h"
#include "buffer.h"
#include "layer1.h"
#define MYUBRR 12000000UL/16/9600-1

uint32_t CalcCRC32(volatile uint8_t *payload, uint8_t size)
{
    uint32_t CRC32MASK = 0x4C11DB7;
    uint32_t crc = 0; 
    for(int i = 0; i < size; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            if (((crc >> 31) & 1) != ((payload[i]>>(8-j-1))&1))
                crc = (crc << 1) ^ CRC32MASK;
            else
                crc = (crc << 1);
        }
    }
    return crc;
}

void SendMessage(uint8_t* payload, uint8_t size)
{
    /* is there any transmission locked*/
    if(!TX_SERV.MUTEX)
    {
        /* calculate the CRC32 checksum */
        uint32_t crc32 = CalcCRC32(payload,size);
        TX_SERV.SIZE = size + 6;
        /* preample */
        TX_SERV.BUFFER[0] = 0x7E;
        /* CRC32 in 4 bytes */
        for(uint8_t i = 0; i < 4; i++)
        {
            uint8_t OneByteCrc32 = (uint8_t)((crc32 >> (24-(i*8)))& 0xFF);
            TX_SERV.BUFFER[i+1] = OneByteCrc32;
        }
        /* write size in buffer */
        TX_SERV.BUFFER[5] = size;
        /* write pyload in buffer */
        for(uint8_t i = 0; i < size; i++)
        {
            TX_SERV.BUFFER[i+6] = payload[i];
        }
        /* some tests */
        for(uint8_t i = 0; i < TX_SERV.SIZE; i++)
        {
            //USART_Transmit(TX_SERV.BUFFER_TX[i]);
        }
        /* start transmission */
        StartTimer_0(&msgService.prescaler);
    }
}

void CheckMessage()
{
    /* is receive buffer locked */
    if(!RX_SERV.MUTEX)
    {
        /* if buffer ready/ full */
        if (RX_SERV.FULL_FLAG == 1)
        {
            /* lock receive buffer*/
            RX_SERV.MUTEX = 1;
            /* var for payload*/
            volatile uint8_t msg_payload[250];
            /* construct the crc in 32 bit lenght */
            uint32_t msg_crc32 =  RX_SERV.BUFFER[1] | RX_SERV.BUFFER[2] << 8 | ((uint32_t) (RX_SERV.BUFFER[3]) << 16 ) | ((uint32_t) (RX_SERV.BUFFER[4]) << 24 );
            /* copy payload for crc32 */
            for (uint8_t i = 0; i < RX_SERV.SIZE; i++)
            {
                msg_payload[i] = RX_SERV.BUFFER[i+6];
            }
            /* calculate CRC32 */
            uint32_t rv_crc32 = CalcCRC32(msg_payload,RX_SERV.SIZE);
            uint32_t CRC32 = 0;
            /* MSB to LSB */
            for(uint8_t i = 0; i < 4; i++)
            {
                CRC32 |= (uint32_t)((rv_crc32 >> (24-(i*8)))& 0xFF) << (i*8);
            }
            /* compare recv and calc CRC */
            if(CRC32 == msg_crc32)
            {
                /* some debug output */
                USART_Transmit(0xAA);
                /* store payload somewhere*/
                /* not mentioned yet */
            }
            /* reset the rx buffer and flags*/
            RX_RESET();
            /* set mutex free for rx */
            RX_SERV.MUTEX = 0;
        }
    }
}

void GetMessage(uint8_t* var_array)
{
    
}

void InitMessageServiceRX()
{
    /* for input port*/
    DDRD &= ~(1 <<PD5);
    /* for some debug tests*/
    USART_Init(MYUBRR,0);
    /* PIN interrupt*/
    PCICR |= (1<<PCIE2);
    PCMSK2 |= (1<<PCINT20);
    /* set up flags of rx */
    RX_SERV.FULL_FLAG = 0;
    /* activate ISR */
    sei();
}

void InitMessageServiceTX(uint8_t prescaler, volatile uint8_t *port, uint8_t* mask)
{
    /* initialize TX port with mask */
    *port |= *mask;
    /* initialize timer 0 */
    InitTimer_0();
    /* set up prescaler*/
    msgService.prescaler = prescaler;
    /* set up flags of tx */
    TX_SERV.MUTEX = 0;
}