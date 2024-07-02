struct TX_BUF{
    volatile uint8_t BUFFER[256];
    volatile uint8_t SIZE;
    volatile uint8_t MUTEX : 1;
}TX_SERV;

struct RX_BUF{
    volatile uint8_t BUFFER[256];
    volatile uint8_t SIZE;
    volatile uint8_t MUTEX : 1;
    volatile uint8_t PREAMPLE_FLAG : 1;
    volatile uint8_t CRC32_FLAG : 1;
    volatile uint8_t SIZE_FLAG : 1;
    volatile uint8_t PAYLOAD_FLAG : 1;
    volatile uint8_t FULL_FLAG : 1;
}RX_SERV;

