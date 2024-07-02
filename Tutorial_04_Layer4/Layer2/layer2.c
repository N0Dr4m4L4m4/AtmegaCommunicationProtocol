#include <avr/io.h>
#include <avr/interrupt.h>
#include "../Layer1/timer.h"
#include "../Layer2/layer2.h"
#include "../Layer1/layer1.h"
#include "../Buffer/buffer.h"
#include "../CRC32/crc32.h"
#include "../USART/usart.h"
/**
 * @brief Check if a message was recieved, if yes:
 * message will be written in the given msg pointer when crc checksum is correct
 * 
 * @param msg pointer where the received message will be written
 * @param size pointer of the size of the message
 * @param error pointer of the error handeling
 */
void getMessageInclCrcCalc(uint8_t* msg, uint8_t* size, uint8_t* error)
{
    if(!RX_SERV.MUTEX && RX_SERV.FULL_FLAG == 1)
    {
        /* lock buffer */
        RX_SERV.MUTEX = 1;
        /* calc crc32 of payload */
        uint32_t crc =  RX_SERV.BUFFER[1] | RX_SERV.BUFFER[2] << 8 | ((uint32_t) (RX_SERV.BUFFER[3]) << 16 ) | ((uint32_t) (RX_SERV.BUFFER[4]) << 24 );
        if(CheckCRC32((uint8_t*)RX_SERV.BUFFER, RX_SERV.SIZE, crc) == true)
        {
            /* copy payload*/
            for (uint8_t i = 0; i < RX_SERV.SIZE; i++)
            {
                msg[i] = ((uint8_t) RX_SERV.BUFFER[i+6]);
            }
            /* copy values*/
            *size = (uint8_t) RX_SERV.SIZE;
            /* no error*/
            *error = 0;
        }
        else
        {
            /*if msg is not correct*/
            *error = 1;
        }
        /* reset buffer */
        RX_RESET();
        /* unlock buffer */
        RX_SERV.MUTEX = 0;
    }
    else
    {
        /* when msg is not ready*/
        *error = 2;
    }
}
/**
 * @brief Get the message without CRC calculation. Is needed for better performance. 
 * Copies data from buffer in given parameters.
 * 
 * @param msg pointer where the received message will be written
 * @param size pointer of the size of the message
 * @param error pointer of the error handeling
 * @param crc value of the crc sum 
 */
void getMessage(uint8_t* msg, uint8_t* size, uint8_t* error, uint32_t* crc)
{
    //USART_Transmit_String("\nMutex:");
    //USART_Transmit(RX_SERV.MUTEX+'0');
    //USART_Transmit_String("\nFlag:");
    
    /* if buffer ready and not used */
    if(!RX_SERV.MUTEX && (RX_SERV.FULL_FLAG == 1))
    {
        //USART_Transmit_String("\n--------\n");
        //USART_Transmit_String("Payload size:");
        //USART_Transmit_String("\nPayload:");
        /* lock buffer */
        RX_SERV.MUTEX = 1;
        /* calc crc32 of payload */
        for (uint8_t i = 0; i < RX_SERV.SIZE; i++)
        {
            /* copy buffer */
            msg[i] = ((uint8_t) RX_SERV.BUFFER[i+6]);
            //USART_Transmit(msg[i]);
        }
        //USART_Transmit_String("\n--------\n");
        /* copy values */
        *size = (uint8_t) RX_SERV.SIZE;
        *error = 0;
        *crc =  RX_SERV.BUFFER[1] | (uint32_t) RX_SERV.BUFFER[2] << 8 | ((uint32_t) (RX_SERV.BUFFER[3]) << 16 ) | ((uint32_t) (RX_SERV.BUFFER[4]) << 24 );
        /* reset receiver buffer*/
        RX_RESET();
        /* unlock buffer */
        RX_SERV.MUTEX = 0;
    }
    else
    {
        /* when msg is not ready*/
        *error = 2;
    }
    
}
/**
 * @brief Send message with a given payload and size. Calculates the CRC32 of the payload notify layer1 to starts transmission.
 * 
 * @param payload pointer of the payload
 * @param size size of the payload
 */
void sendMessage(uint8_t* payload, uint8_t size)
{
    /* is there any transmission locked*/
    if(!TX_SERV.MUTEX)
    {
        TX_SERV.MUTEX = 1;
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
        /* write payload in buffer */
        for(uint8_t i = 0; i < size; i++)
        {
            TX_SERV.BUFFER[i+6] = payload[i];
        }
        /* set up flags */
        TX_SERV.READY = 1;
        TX_SERV.MUTEX = 0;
        /* start transmission */
        StartTimer_0(&msgService.prescaler);
    }
}
/**
 * @brief Initialize receiver service to receive messages
 * 
 */
void InitMessageServiceRX()
{
    /* for input port*/
    DDRD &= ~(1 <<PD5);
    /* PIN interrupt*/
    PCICR |= (1<<PCIE2);
    PCMSK2 |= (1<<PCINT20);
    /* set up flags of rx */
    RX_SERV.FULL_FLAG = 0;
    /* activate ISR */
    sei();
}
/**
 * @brief Initialize transmitter service to send messages with a given port, mask and prescaler for clock.
 * 
 * @param prescaler NO_PRE, PRE_8, PRE_64, PRE_256, PRE_1024 
 * @param port Pointer of the port for transmission e.g PD
 * @param mask Pointer  of the mask to the port register
 */
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