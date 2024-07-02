#define STATE_IDLE 0
#define STATE_SEND 1
#define STATE_WAIT 2
#define STATE_SEND_ACK 3
#define STATE_CHANGE_SUCCESS 0
#define STATE_CHANGE_FAILURE 1

struct statemachine{
    uint8_t state;
    uint8_t busy : 1;
    uint8_t prev_state;
}machine;

uint8_t getState();
uint8_t getPrevState();
uint8_t setState(uint8_t state);
uint8_t setPrevState(uint8_t state);
void checkState();