/** @brief Using no prescaler.*/
#define NO_PRE 0
/** @brief Using prescaler with 8 of the clock frequency.*/
#define PRE_8 1
/** @brief Using prescaler with 64 of the clock frequency.*/
#define PRE_64 2
/** @brief Using prescaler with 256 of the clock frequency.*/
#define PRE_256 3
/** @brief Using prescaler with 1024 of the clock frequency.*/
#define PRE_1024 4
/** @brief Using external clock with fallen edge.*/
#define PRE_EXT_CLK_FALL 5
/** @brief Using external clock with rising edge.*/
#define PRE_EXT_CLK_RISE 6

void InitTimer_0();
void StartTimer_0(uint8_t* prescaler);
void StopTimer_0();