#include <avr/io.h>
#include <avr/interrupt.h>
#include "../Buffer/buffer.h"
#include "../Layer1/layer1.h"
#include "../Layer1/timer.h"
#include "../IRQHandler/irqhandler.h"
/**
 * @brief Resets transmission buffer 
 * 
 */
void TX_RESET()
{
    /* lock mutex tx*/
    TX_SERV.MUTEX = 1;
    StopTimer_0();
    /* reset all flags*/
    TX_SERV.SIZE = 0;
    /* reset ready flag*/
    TX_SERV.READY = 0;
    /* reset buffer */
    for(int i = 0; i < 256; i++)
    {
        TX_SERV.BUFFER[i] = 0x00;
    }
    /* set mutex free */
    TX_SERV.MUTEX = 0;
}
/**
 * @brief Resets receiving buffer
 * 
 */
void RX_RESET()
{
    RX_SERV.MUTEX = 1;
    /* reset all flags*/
    RX_SERV.FULL_FLAG = 0;
    RX_SERV.PREAMPLE_FLAG = 0;
    RX_SERV.CRC32_FLAG = 0;
    RX_SERV.SIZE_FLAG = 0;
    /* delete all vars*/
    RX_SERV.SIZE = 0;
    
    /* reset buffer */
    for(int i = 0; i < 256; i++)
    {
        RX_SERV.BUFFER[i] = 0x00;
    }
    RX_SERV.MUTEX = 0;
}

