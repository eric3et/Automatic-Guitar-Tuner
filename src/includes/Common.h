#ifndef COMMON_H_
#define COMMON_H_


// Libraries
#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include <ESP32Servo.h>

//PINS
#define SERVO_PIN 13
#define SELECT_STRING_BUTTON_PIN 27
#define TRIGGER_PIN 18
#define LED_LOW_RED_PIN 33
#define LED_LOW_YELLOW_PIN 25
#define LED_GREEN_PIN 26
#define LED_HIGH_YELLOW_PIN 14
#define LED_HIGH_RED_PIN 12
#define ADC_PIN 32 // I2S_MODE_ADC_BUILT_IN


//I2S & ADC Setup
#include <driver/i2s.h>
#define I2S_SAMPLE_RATE (20000)
#define ADC_INPUT (ADC1_CHANNEL_4) //pin 32
#define I2S_DMA_BUF_LEN (1024)
#define NUM_SAMPLES (1300)

//OLED Display Setup
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels
#define OLED_RESET -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
#define LOGO_HEIGHT 16
#define LOGO_WIDTH  16
extern Adafruit_SSD1306 display;
static const unsigned char PROGMEM logo_bmp[] =
{ 0b00000000, 0b11000000,
  0b00000001, 0b11000000,
  0b00000001, 0b11000000,
  0b00000011, 0b11100000,
  0b11110011, 0b11100000,
  0b11111110, 0b11111000,
  0b01111110, 0b11111111,
  0b00110011, 0b10011111,
  0b00011111, 0b11111100,
  0b00001101, 0b01110000,
  0b00011011, 0b10100000,
  0b00111111, 0b11100000,
  0b00111111, 0b11110000,
  0b01111100, 0b11110000,
  0b01110000, 0b01110000,
  0b00000000, 0b00110000 };

//Global Variables
extern volatile int currentString;
extern int counter;
extern float correlation[NUM_SAMPLES];
extern uint16_t buffer_i2s[I2S_DMA_BUF_LEN];
extern short buffer[NUM_SAMPLES];
extern int bufferAverage;
extern float freq;
extern short displayLoader;
extern const int BAUD_RATE;
extern const int CPU_FREQ_MHZ;
extern bool tuningComplete;
extern int calibrate;

//Function Declarations
void i2sInit();
void DisplayInit();
void ServoInit();
void InterruptsInit();
void LEDInit();
float ReadSamples();
void ComputeCorrelation();
void ComputeFrequency();
void DisplayOLED(int stringNum, int frequency);
void StartTuningAlgorithm(int currentString, float freq);
int GetHzForStringNumber(int stringNumber);
void DisplayLED(int frequency, int stringNum);
void TurnOffLEDS();
void TurnOnLED(int LED_Pin);
void StopMotor();


#endif