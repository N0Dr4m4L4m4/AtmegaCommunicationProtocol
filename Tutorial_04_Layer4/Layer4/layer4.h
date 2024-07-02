#define FLG_DGRAM 0x02
#define FLG_ACK 0x01
#define FLG_FUTURE 0xFC
struct layer4Buffer{
    uint8_t msg[250];/** @brief buffer of the message*/
    uint8_t flags;/** @brief flags liek ACK or DGRAM */
    uint8_t pckg_number;/** @brief Package number*/
    uint8_t size;/** @brief Size of the message*/
    uint8_t dest_addr;/** @brief Destination address*/
    uint8_t src_addr; /** @brief Source address*/
    uint8_t ready : 1; /** @brief flag to identify sending wish*/
}layer4buf;
void incrementPackageNumber();
void checkMessageLayer4(uint8_t *msg, uint8_t *size, uint8_t *error);
void sendMessageLayer4(uint8_t flags, uint8_t package_number, uint8_t* payload, uint8_t size, uint8_t *src, uint8_t *dest);
void sendPackageLayer4(uint8_t flags,uint8_t* payload, uint8_t size, uint8_t *src, uint8_t *dest);