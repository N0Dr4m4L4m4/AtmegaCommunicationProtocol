#include <avr/io.h>
#include "../Layer2/layer2.h"
#include "../Layer3/layer3.h"
#include "../CRC32/crc32.h"
#include "../Buffer/buffer.h"
#include "../USART/usart.h"

uint8_t system_priority = PRIORITY_LOW; /*** @brief System priority level initialize with PRIORITY_LOW**/
uint8_t pi_adr =  PI_ADR;/*** @brief Own address of the pi**/;
 /**
 * @brief This function is used to send packages from a source to a destination addressed device
 * 
 * @param dest_addr destination address of the device
 * @param src_addr source address of the device
 * @param payload payload as uint8_t array
 * @param payload_size size of the payload
 * @param priority priority of the message(LOW,MEDIUM,HIGH)
 */
void sendPackage(uint8_t* dest_addr, uint8_t* src_addr, uint8_t* payload, uint8_t size, uint8_t priority)
{
    /* check if the package can be send with the current priority*/
    if(priority >= system_priority)
    {
        /* set up system priority */
        uint8_t tmp_priority = system_priority;
        system_priority = priority;
        /* increase buffer */
        uint8_t tmp_payload[size+2];
        /* flag & id*/
        tmp_payload[0] = payload[0];
        tmp_payload[1] = payload[1];
        /* address */
        tmp_payload[2] = *dest_addr;
        tmp_payload[3] = *src_addr; 

        for(uint8_t i = 4; i < size+2; i++)
        {
            tmp_payload[i] = payload[i-2];
        }
        /* send message */
        sendMessage(tmp_payload,size+2); 
        /* set back system priority */
        system_priority = tmp_priority;
    }
}
/**
 * @brief Checks if a message has been received
  */
void checkPackage(uint8_t *msg, uint8_t *size, uint8_t *error) {
    uint32_t crc;
    getMessage(msg, size, error,&crc); 
    if(!*error)
    {
        /* copy addresses */
        rx.dst_addr = msg[2];
        rx.src_addr = msg[3];
        /* copy size of msg */
        rx.size = *size;
        /* relay decision */
        if(rx.dst_addr == 0x00)
        {
            /* received broadcast */
            if(rx.src_addr ==  pi_adr)
            {
                ACK = 1;
            }
            else
            {   
                // relay package
                sendPackage(&rx.dst_addr, &rx.src_addr, rx.msg, rx.size, PRIORITY_HIGH);
                // package is not important for layer 4
                *error  = 4;
            }
        }
        /* am i the receiver of the message*/
        else if(rx.dst_addr ==  pi_adr)
        {
            /* check its crc32 */
            if(CheckCRC32(msg, *size, crc) == false)
            {
                /* if its wrong -> error*/
                *error  = 4;
            }
           
        }
        else
         {
            // relay package
            sendPackage(&rx.dst_addr, &rx.src_addr, rx.msg, rx.size, PRIORITY_HIGH);
            // package is not important for layer 4
            *error  = 4;
        }
    } 
    /* no message */
    else
    {
        *error = 1;
    }
}