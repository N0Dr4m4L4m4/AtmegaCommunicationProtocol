/** @brief Predefined true.*/
#define true 1
/** @brief Predefined false.*/
#define false 0

int CheckCRC32(uint8_t* payload, uint8_t size, uint32_t crc);
uint32_t CalcCRC32(volatile uint8_t *payload, uint8_t size);