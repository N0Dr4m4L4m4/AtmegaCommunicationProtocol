#include <avr/io.h>
#include "../Layer2/layer2.h"
#include "../Layer3/layer3.h"
#include "../Buffer/buffer.h"
#include "../USART/usart.h"
//#define F_CPU 12000000UL
//#include <util/delay.h>

uint8_t state = DES_ADDRESS; /*** @brief state machine initialize with destination address status**/
const uint8_t pi_adr =  9; /*** @brief Own address of the pi**/
uint8_t system_priority = PRIORITY_LOW; /*** @brief System priority level initialize with PRIORITY_LOW**/
package_t rx; /*** @brief package object for receiving data*/
/**
 * @brief This function is used to send packages from a source to a destination addressed device
 * 
 * @param dest_addr destination address of the device
 * @param src_addr source address of the device
 * @param payload payload as uint8_t array
 * @param payload_size size of the payload
 * @param priority priority of the message(LOW,MEDIUM,HIGH)
 */
void sendPackage(uint8_t* dest_addr, uint8_t* src_addr, uint8_t* payload, uint8_t payload_size, uint8_t priority)
{
    /* check if the package can be send with the current priority*/
    if(priority >= system_priority)
    {
        /* set up system priority */
        uint8_t tmp_priority = system_priority;
        system_priority = priority;
        sendMessage(dest_addr,1);
        //_delay_ms(1500);
        //checkPackage();
    
        sendMessage(src_addr,1);
        //_delay_ms(1500);
        //checkPackage();

        sendMessage(payload,payload_size);
        //_delay_ms(3500);
        //checkPackage();
        /* set back system priority */
        system_priority = tmp_priority;
    }
    
}
/**
 * @brief Checks if a message has been received, if yes:
 * the state machine changes with one package and starts with receiving the destination address
 * 
 */
void checkPackage() {
    uint8_t msg[250];
    uint8_t size = 0;
    uint8_t error;
    getMessageWithoutCrc(msg, &size, &error);
    switch(state){
        case DES_ADDRESS:
            rx.dst_addr = msg[0];
            USART_Transmit(rx.dst_addr);
            state = SRC_ADDRESS;
            break;
        case SRC_ADDRESS:
            rx.src_addr = msg[0];
            USART_Transmit(rx.src_addr);
            state = PAYLOAD;
            break;
        case PAYLOAD:
            rx.size=size;
            USART_Transmit(rx.size);
            for(int i = 0; i < size; i++)
            {
                rx.msg[i] = msg[i];
                //USART_Transmit(rx.msg[i]);
            }
            // Broadcast
            if(rx.dst_addr == 0x00)
            {
                //USART_Transmit(0xA0);
                if(rx.src_addr ==  pi_adr)
                {
                    // notify layer 4
                    //USART_Transmit(0xA1);
                }
                else
                {   // relay package
                    sendPackage(&rx.dst_addr, &rx.src_addr, rx.msg, rx.size, PRIORITY_HIGH);
                    //USART_Transmit(0xA2);
                }

            }
            else if (rx.src_addr == pi_adr)
            {
               //USART_Transmit(0xA3);
            }
            else
            {
                // relay package
                sendPackage(&rx.dst_addr, &rx.src_addr, rx.msg, rx.size, PRIORITY_HIGH);
                //USART_Transmit(0xA4);
            }

            state = DES_ADDRESS;
    }
    for(int i = 0; i < 250; i++)
    {
         msg[i] = 0;
    }
 
}