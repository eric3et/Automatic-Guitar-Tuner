#include "includes/Common.h"


void setup(){
  	Serial.begin(115200);
  	setCpuFrequencyMhz(240);
	DisplayInit();
  	i2sInit();
	ServoInit();
	InterruptsInit();
}

void loop(){
    
	
	ReadSamples();
	ComputeCorrelation();
	ComputeFrequency();
	//DisplayConsole();
	DisplayOLED(currentString,freq);
	TurnMotor(currentString,freq);
	
	// if(ReadSamples() < 0.05) 
	// { //if string is not being strummed
	// 	DisplayOLED(currentString,0);
	// 	//delay(250);
	// }
	// else{
	// 	ReadSamples();
	// 	ComputeCorrelation();
	// 	ComputeFrequency();
	// 	//DisplayConsole();
	// 	DisplayOLED(currentString,freq);
	// 	TurnMotor(currentString,freq);
	// }
}
