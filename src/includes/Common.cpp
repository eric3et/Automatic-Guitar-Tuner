#include "Common.h"


//Display
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Global Variables
int volatile currentString = 0;
int counter = 0;
float correlation[NUM_SAMPLES] = {0};
uint16_t buffer_i2s[I2S_DMA_BUF_LEN];
short buffer[NUM_SAMPLES] = {0};
int bufferAverage = 0;
int freq = 0;
short displayLoader = 0;
const int BAUD_RATE = 115200;
const int CPU_FREQ_MHZ = 240;
bool triggerEnabled = false;





