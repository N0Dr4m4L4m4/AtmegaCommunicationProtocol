#define NO_PRE 0
#define PRE_8 1
#define PRE_64 2
#define PRE_256 3
#define PRE_1024 4
#define PRE_EXT_CLK_FALL 5
#define PRE_EXT_CLK_RISE 6

void InitTimer_0();
void StartTimer_0(uint8_t* prescaler);
void StopTimer_0();