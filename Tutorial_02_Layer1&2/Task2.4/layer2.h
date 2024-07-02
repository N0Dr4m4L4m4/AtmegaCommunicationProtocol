struct MsgService {
    uint8_t prescaler;
}msgService;

uint32_t CalcCRC32(volatile uint8_t *payload, uint8_t size);
void SendMessage(uint8_t* payload, uint8_t size);
void InitMessageServiceTX(uint8_t prescaler, volatile uint8_t *port, uint8_t *mask);
void InitMessageServiceRX();
void CheckMessage();