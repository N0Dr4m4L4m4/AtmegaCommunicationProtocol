#include <avr/io.h>
#include "../USART/usart.h"
#include "../Layer3/layer3.h"
#include "../Layer4/layer4.h"
#include "../Buffer/buffer.h"
#include "statemachine.h"

uint8_t msg[250];
uint8_t size;
uint8_t error;
uint8_t *ack;
uint64_t counter = 0;
uint8_t tries = 5;

/**
 * @brief Get the State of the state machine
 * 
 * @return uint8_t state
 */
uint8_t getState()
{
    return machine.state;
}
/**
 * @brief Get the previous state of the state machine
 * 
 * @return uint8_t previous state
 */
uint8_t getPrevState()
{
    return machine.prev_state;
}
/**
 * @brief Set new state to state machine if possible (not locked)
 * 
 * @param state 
 * @return uint8_t STATE_CHANGE_SUCCESS or STATE_CHANGE_FAILURE
 */
uint8_t setState(uint8_t state)
{
    if(machine.busy) return STATE_CHANGE_FAILURE;
    machine.state = state;
    return STATE_CHANGE_SUCCESS;
}

/**
 * @brief Set prev state to state machine if possible (not locked)
 * 
 * @param state 
 * @return uint8_t STATE_CHANGE_SUCCESS or STATE_CHANGE_FAILURE
 */
uint8_t setPrevState(uint8_t state)
{
    if(machine.busy) return STATE_CHANGE_FAILURE;
    machine.prev_state = state;
    return STATE_CHANGE_SUCCESS;
}

/**
 * @brief Checks for new messages or sending wish
 * 
 */
void checkState()
{
    /* anything received ?*/
    checkMessageLayer4(msg,&size,&error);
    switch(machine.state){
        case STATE_IDLE:
            /* sending wish */
            if(layer4buf.ready == 1)
            {
                /* change state if possible */
               if(setState(STATE_SEND) == STATE_CHANGE_SUCCESS)
               {
                   /* lock state machine */
                   machine.busy = 1;
                   /* reset sending wish flag */
                   layer4buf.ready = 0;
               }
               else
               {
                   USART_Transmit_String("Machine is not able to send right now. Try it later again.\n");
               }
            }
            break;
        case STATE_SEND:
            USART_Transmit_String("Try to send message...\n");
            /* send message */
            sendMessageLayer4(layer4buf.flags, layer4buf.pckg_number, layer4buf.msg, layer4buf.size, &layer4buf.src_addr, &layer4buf.dest_addr);
            /* reset lock */
            machine.busy = 0;
            /* do not wait for ACK -> datagram*/
            if(layer4buf.flags == FLG_DGRAM)
            {
                setState(STATE_IDLE);    
            }
            /* wait for ACK*/
            else
            {
                USART_Transmit_String("Waiting for ACK...\n");
                setState(STATE_WAIT);   
            }
            break;
        case STATE_WAIT:
            /* any message received*/
            if(!error)
            {
                /* is the flasg an ack */
                if(msg[1] == 0x01)
                {
                    USART_Transmit_String("Message successfully send.\n");
                    /* package number + 1*/
                    incrementPackageNumber();
                    /* reset state to idle*/
                    setState(STATE_IDLE);  
                }
                else
                {
                    /* timer variable + 1*/
                    counter++;
                }
            }
            else{
                /* check counter */
                if(counter == 100000)
                {
                    /* reset counter */
                    counter = 0;
                    /* try to send message again*/
                    if(tries > 0)
                    {
                        USART_Transmit_String("Try to send message again\n");
                        setState(STATE_SEND);
                        tries--;
                    }
                    /* no more tries*/
                    else
                    {
                        USART_Transmit_String("No ACK received. Stop sending\n");
                        tries = 5;
                        setState(STATE_IDLE);
                    }
                }
                else
                {
                    /* counter + 1 */
                    counter++;
                }
            }
            break;
        case STATE_SEND_ACK:
            /* received any message */
            if(!error)
            {
                USART_Transmit_String("Message received: ");
                /* print out message to console */
                for(uint8_t i = 4 ; i < size+4; i++)
                {
                    USART_Transmit(msg[i]);
                }
            }
            USART_Transmit_String("\n");
            /* send ACK to sender */
            sendMessageLayer4(FLG_ACK,layer4buf.pckg_number,ack,1,&pi_adr,&rx.src_addr);
            setState(getPrevState());
            break;
        default:
            /* may not occur but its more save*/
            USART_Transmit_String("Unknown state. Set back to idle state.\n");
            setState(STATE_IDLE);
    }

}