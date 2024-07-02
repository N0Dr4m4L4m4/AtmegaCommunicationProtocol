/* 
    Author:  Robin Gerstmann 
    Mail:    robin.gerstmann@s2020.tu-chemnitz.de
    Date:    21.11.2021
    Fuction: Protocol Layer-I | Timer set up to transmit bits of buffer every clock edge 
*/
#include <avr/io.h>
#include <avr/interrupt.h>
#include "buffer.h"
#include "timer.h"
#include "layer1.h"

void InitTimer_0()
{
    /* OVF ISR*/
    TIMSK0 |= 1 << TOIE0;
    /* enable isr */
    sei();
}

void StartTimer_0(uint8_t* prescaler)
{
    switch (*prescaler){
        /* no prescaling */
        case NO_PRE:
            TCCR0B |= 1 << CS00;
            break;
        /* prescaling clk/8 */
        case PRE_8:
            TCCR0B |= 1 << CS01;
            break;
        /* prescaling clk/64 */
        case PRE_64:
            TCCR0B |= 1 << CS01 | 1 << CS00;
            break;
        /* prescaling clk/256 */
        case PRE_256:
            TCCR0B |= 1 << CS02;
            break;
        /* prescaling clk/1024 */
        case PRE_1024:
            TCCR0B |= 1 << CS00 | 1 << CS02;
            break;
        /* External clock source on T0 pin. Clock on falling edge */
        case PRE_EXT_CLK_FALL:
            TCCR0B |= 1 << CS01 | 1 << CS02;
            break;
        /* External clock source on T0 pin. Clock on rising edge */
        case PRE_EXT_CLK_RISE:
            TCCR0B |= 1 << CS00 |1 << CS01 | 1 << CS02;
            break;
        /* nothing happens if no prescaling is set up*/
        default:
            return;
    }
    return;
}
void StopTimer_0()
{
    /* reset timer register to turn off timer 0*/
    TCCR0B = 0x00;
}
