#include <avr/io.h>
/** @brief Transimssion buffer struct with flags and mutex*/
struct TX_BUF{
    volatile uint8_t BUFFER[256]; /** @brief Buffer of uint8_t with max 256 items */
    volatile uint8_t SIZE;  /** @brief size of the given payload */
    volatile uint8_t MUTEX : 1; /** @brief Mutual exclusion lock*/
    volatile uint8_t READY : 1; /** @brief Flag to notify layer 1 to send*/
}TX_SERV;
/** @brief Receiving buffer struct with flags, mutex and payload */
struct RX_BUF{
    volatile uint8_t BUFFER[256]; /** @brief Receiving buffer of 256 items*/
    volatile uint8_t SIZE;/** @brief Size of the received payload*/
    volatile uint8_t MUTEX : 1;/** @brief Mutual exclusion lock*/
    volatile uint8_t PREAMPLE_FLAG : 1;/** @brief Preample flag received*/
    volatile uint8_t CRC32_FLAG : 1;/** @brief CRC32 value received flag*/
    volatile uint8_t SIZE_FLAG : 1;/** @brief Payload size received flag*/
    volatile uint8_t PAYLOAD_FLAG : 1;/** @brief Payload received flag*/
    volatile uint8_t FULL_FLAG : 1;/** @brief Buffer full flag: Payload received*/
}RX_SERV;
