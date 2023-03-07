#include "Common.h"


void IRAM_ATTR StringSelectButtonISR() {
	currentString++;
    if(currentString == 6) currentString = 0;

    //debounce
    unsigned long t = millis();
    while(millis()-t < 200);
}

void IRAM_ATTR TriggerDisableISR() {
    triggerEnabled = false;
    StopMotor();

    //debounce
    unsigned long t = millis();
    while(millis()-t < 200);
}

void IRAM_ATTR TriggerEnableISR() {
    triggerEnabled = true;

    //debounce
    unsigned long t = millis();
    while(millis()-t < 200);
}

void InterruptsInit(){
    pinMode(SELECT_STRING_BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(SELECT_STRING_BUTTON_PIN, StringSelectButtonISR, FALLING);
    attachInterrupt(TRIGGER_PIN, TriggerDisableISR, FALLING);
    attachInterrupt(TRIGGER_PIN, TriggerEnableISR, RISING);
}


