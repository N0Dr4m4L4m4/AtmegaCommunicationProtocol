#include <avr/io.h>
#include "../Layer2/layer2.h"
#include "../Layer1/layer1.h"
#include "../Buffer/buffer.h"
#include "../CRC32/crc32.h"

/**
 * @brief Calculates the CRC32 checksum of the payload with size max. 250 and
 * compares it with the recieved one
 * 
 * @param payload Payload as array
 * @param size  Size of the given payload
 * @return int true 1 (equal) | false 0 (not equal)
 */
int CheckCRC32(uint8_t* payload, uint8_t size)
{
    /* var for payload*/
    uint8_t msg_payload[250];
    /* construct the crc in 32 bit lenght */
    uint32_t msg_crc32 =  payload[1] | payload[2] << 8 | ((uint32_t) (payload[3]) << 16 ) | ((uint32_t) (payload[4]) << 24 );
    /* copy payload for crc32 */
    for (uint8_t i = 0; i < size; i++)
    {
        msg_payload[i] = payload[i+6];
    }
    /* calculate CRC32 */
    uint32_t rv_crc32 = CalcCRC32(msg_payload,size);
    uint32_t CRC32 = 0;
    /* MSB to LSB */
    for(uint8_t i = 0; i < 4; i++)
    {
        CRC32 |= (uint32_t)((rv_crc32 >> (24-(i*8)))& 0xFF) << (i*8);
    }
    /* compare crc32 values*/ 
    if(CRC32 != msg_crc32)
    {
        return false;
    }
            
    return true;

}
/**
 * @brief Calculate the CRC32 of a given payload
 * 
 * @param payload Payload as uint8_t array
 * @param size Size of the payload
 * @return uint32_t crc32 checksum
 */
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