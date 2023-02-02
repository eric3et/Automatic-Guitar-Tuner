#include "Common.h"


void IRAM_ATTR isr() {
	currentString++;
    if(currentString == 6) currentString = 0;

    //debounce
    unsigned long t = millis();
    while(millis()-t < 200);
}

void InterruptsInit(){
    pinMode(SELECT_STRING_BUTTON_PIN, INPUT_PULLUP);
    attachInterrupt(SELECT_STRING_BUTTON_PIN, isr, FALLING);
}


