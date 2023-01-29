#include "Common.h"


void DisplayOLED(int stringNum, int frequency) {
	const char notes[] = {'E','A','D','G','B','e'};
	display.clearDisplay();
	display.setCursor(0, 0);     // Start at top-left corner
	display.setTextColor(SSD1306_WHITE); // Draw white text
  
	for(int i = 0; i <= 5; i++){
		if(i == stringNum){
			display.setTextSize(4);      // Normal 1:1 pixel scale
			display.print(notes[i]);
		}else{
			display.setTextSize(1);      // Normal 1:1 pixel scale
			display.print(notes[i]);
		}
	}
	
    display.setTextSize(2);      // Normal 1:1 pixel scale
    display.setCursor(95,0);     // Start at top-left corner
    display.print("Hz");
    display.setCursor(70,18);     // Start at top-left corner
    
	if(frequency > 62 && frequency < 450) {
		display.print(frequency);
		DisplayLED(frequency, stringNum);
	}
	else{
		for(short i = 0; i < 4;i++){
			if(i==displayLoader) display.print(">");
			else display.print("-");
		}
		displayLoader++;
		if(displayLoader >= 5) displayLoader = 0;
	}
  	display.display();
}

void DisplayLED(int freq, int stringNum){
	int desiredFreq = GetHzForStringNumber(stringNum);
	int freqDiff = abs(freq - desiredFreq);

	int toleranceG = desiredFreq * 0.015;
	int toleranceGY = desiredFreq * 0.05;
	int toleranceY = desiredFreq * 0.10;
	int toleranceYR = desiredFreq * 0.15;

	TurnOffLEDS();

	if(freqDiff <= toleranceG) TurnOnLED(LED_GREEN_PIN); // Green

	else if(freq < desiredFreq){ // to low
		if(freqDiff <= toleranceGY) {TurnOnLED(LED_GREEN_PIN);TurnOnLED(LED_LOW_YELLOW_PIN);}
		else if(freqDiff <= toleranceY) {TurnOnLED(LED_LOW_YELLOW_PIN);}
		else if(freqDiff <= toleranceYR) {TurnOnLED(LED_LOW_YELLOW_PIN);TurnOnLED(LED_LOW_RED_PIN);}
		else {TurnOnLED(LED_LOW_RED_PIN);}
	}else{ // to high
		if(freqDiff <= toleranceGY) {TurnOnLED(LED_GREEN_PIN);TurnOnLED(LED_HIGH_YELLOW_PIN);}
		else if(freqDiff <= toleranceY) {TurnOnLED(LED_HIGH_YELLOW_PIN);}
		else if(freqDiff <= toleranceYR) {TurnOnLED(LED_HIGH_YELLOW_PIN);TurnOnLED(LED_HIGH_RED_PIN);}
		else {TurnOnLED(LED_HIGH_RED_PIN);}
	}
}
void TurnOffLEDS(){
	gpio_set_level((gpio_num_t)LED_HIGH_RED_PIN, LOW);
	gpio_set_level((gpio_num_t)LED_HIGH_YELLOW_PIN, LOW);
	gpio_set_level((gpio_num_t)LED_GREEN_PIN, LOW);
	gpio_set_level((gpio_num_t)LED_LOW_YELLOW_PIN, LOW);
	gpio_set_level((gpio_num_t)LED_LOW_RED_PIN, LOW);
}
void TurnOnLED(int LED_Pin){
	gpio_set_level((gpio_num_t)LED_Pin, HIGH);
}

void DisplayInit(){
  	if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    	Serial.println(F("SSD1306 allocation failed"));
    	for(;;);
  	}
}

void LEDInit(){
	gpio_reset_pin((gpio_num_t)LED_HIGH_RED_PIN);
	gpio_reset_pin((gpio_num_t)LED_HIGH_YELLOW_PIN);
	gpio_reset_pin((gpio_num_t)LED_GREEN_PIN);
	gpio_reset_pin((gpio_num_t)LED_LOW_YELLOW_PIN);
	gpio_reset_pin((gpio_num_t)LED_LOW_RED_PIN);

	gpio_set_direction((gpio_num_t)LED_HIGH_RED_PIN, GPIO_MODE_OUTPUT);
	gpio_set_direction((gpio_num_t)LED_HIGH_YELLOW_PIN, GPIO_MODE_OUTPUT);
	gpio_set_direction((gpio_num_t)LED_GREEN_PIN, GPIO_MODE_OUTPUT);
	gpio_set_direction((gpio_num_t)LED_LOW_YELLOW_PIN, GPIO_MODE_OUTPUT);
	gpio_set_direction((gpio_num_t)LED_LOW_RED_PIN, GPIO_MODE_OUTPUT);
	
	gpio_set_level((gpio_num_t)LED_HIGH_RED_PIN, LOW);
	gpio_set_level((gpio_num_t)LED_HIGH_YELLOW_PIN, LOW);
	gpio_set_level((gpio_num_t)LED_GREEN_PIN, LOW);
	gpio_set_level((gpio_num_t)LED_LOW_YELLOW_PIN, LOW);
	gpio_set_level((gpio_num_t)LED_LOW_RED_PIN, LOW);
}