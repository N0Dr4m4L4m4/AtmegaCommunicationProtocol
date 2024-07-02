#define F_CPU 12000000UL
#include <avr/io.h>
#include "../Layer3/layer3.h"
#include "../Layer4/layer4.h"
#include "../Buffer/buffer.h"
#include "../USART/usart.h"
#include "../StateMachine/statemachine.h"
#include <util/delay.h>

uint8_t ACK = 0; /**
 * @brief is needed to identify an ACK of layer 3
 * 
 */
/**
 * @brief Inctrements the package number of the layer 4 buffer object for sending
 * 
 */
void incrementPackageNumber()
{
    if(layer4buf.pckg_number > 0xff)
    {
        layer4buf.pckg_number = 0;
    }
    else
    {
        layer4buf.pckg_number++;
    }
}
/**
 * @brief Sends message to layer 3
 * 
 * @param flags Can be used to ACK a package or sends a datagram
 * @param package_number Number of the package 
 * @param payload Message
 * @param size Size of the message
 * @param src Source address
 * @param dest Destination address
 */
void sendMessageLayer4(uint8_t flags, uint8_t package_number, uint8_t* payload, uint8_t size, uint8_t *src, uint8_t *dest){
    /* increase the buffer */
    uint8_t tmp_payload[size+2];
    /* insert layer4 informations */
    tmp_payload[0] = package_number;
    tmp_payload[1] = flags;
    /* copy payload into new paylaod*/
    for(uint8_t i = 2 ; i < size + 2;i++)
    {
        tmp_payload[i] = payload[i-2];
    }
    /* sends it to layer 3*/
    sendPackage(dest,src,tmp_payload,size+2,PRIORITY_LOW);
}
/**
 * @brief Checking for incoming messages
 * 
 * @param msg pointer of the receiving buffer
 * @param size pointer to the size of the buffer
 * @param error pointer of the error number
 */
void checkMessageLayer4(uint8_t *msg, uint8_t *size, uint8_t *error){
    /* check for incoming message */
    checkPackage(msg, size, error);
    /* look if any flag was received */
    uint8_t recv_flag = msg[1];
    /* if not error was received */
    if(!*error)
    { 
        /* send ACK when recv a message */
        if(recv_flag != FLG_DGRAM && recv_flag != FLG_ACK && !ACK)
        {
            setPrevState(getState());
            setState(STATE_SEND_ACK);
        }
    }
}
/**
 * @brief Prepares packge for layer 4
 * 
 * @param flags which flags should be used like ACK or DGRAM
 * @param payload message
 * @param size size of the message
 * @param src source address
 * @param dest destination address
 */
void sendPackageLayer4(uint8_t flags,uint8_t* payload, uint8_t size, uint8_t *src, uint8_t *dest)
{
    /* copy payload in buffer for layer 4*/
    for (uint8_t i = 0 ; i < size; i++)
    {
        layer4buf.msg[i] = payload[i];
    }
    /* copy information in buffer layer 4 and ready up sending flag*/
    layer4buf.flags = flags;
    layer4buf.size = size;
    layer4buf.dest_addr = *dest;
    layer4buf.src_addr = *src;
    layer4buf.ready = 1;
}