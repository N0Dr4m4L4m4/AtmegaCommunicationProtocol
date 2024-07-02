/**
 * @brief Struct for message service to save the predefined prescaler
 * 
 */
struct MsgService {
    uint8_t prescaler; /** @brief Prescaler value*/
}msgService;

uint32_t CalcCRC32(volatile uint8_t *payload, uint8_t size);
void getMessage(uint8_t* msg, uint8_t* size, uint8_t* error);
void getMessageWithoutCrc(uint8_t* msg, uint8_t* size, uint8_t* error);
void sendMessage(uint8_t* payload, uint8_t size);
void InitMessageServiceTX(uint8_t prescaler, volatile uint8_t *port, uint8_t *mask);
void InitMessageServiceRX();
void CheckMessage();