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
    
	
	//Need to come up with a better solution for blocking autocorrelation computation when guitar strum is not detected on both aux and mic
	//This is better for aux, but < 0.30 doesnt work for mic
	// if(ReadSamples() < 0.30) 
	// { //if string is not being strummed
	// 	DisplayOLED(currentString,0);
	// 	//delay(250);
	// }
	// else{
		ReadSamples();
		ComputeCorrelation();
		ComputeFrequency();
		//DisplayConsole();
		DisplayOLED(currentString,freq);
	// }
}
