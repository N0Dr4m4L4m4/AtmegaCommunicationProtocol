#define DES_ADDRESS 1 /** @brief destination address received state*/
#define SRC_ADDRESS 2 /** @brief source address receiver state*/
#define PAYLOAD 3 /** @brief payload state*/
/**
 * @brief Priority level low
 * 
 */
#define PRIORITY_LOW 1
/**
 * @brief Priority level medium
 * 
 */
#define PRIORITY_MEDIUM 2
/**
 * @brief Priority level high
 * 
 */
#define PRIORITY_HIGH 5
/** @brief Type definition of package*/
typedef struct package_s package_t;
/**
 * @brief Package object for layer 3 
 * 
 */
struct package_s{
    uint8_t src_addr; /** @brief Source address*/
    uint8_t dst_addr; /** @brief Destination address*/
    uint8_t msg[250];/** @brief Payload*/
    uint8_t size; /** @brief Size of the payload*/
    uint8_t priority; /** @brief priority level (not used right now) */
}; 
/**
 * @brief Struct of packages for transmission and receiving
 * 
 */
struct package{
    struct package_t *rx;/** @brief receiver package */
    struct package_t *tx;/** @brief transmission package*/
}packages;

void checkPackage();
void sendPackage(uint8_t* dest_addr, uint8_t* src_addr, uint8_t* payload, uint8_t payload_size, uint8_t priority);