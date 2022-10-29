#include "includes/Common.h"


void setup(){
  	Serial.begin(115200);
  	setCpuFrequencyMhz(240);
	DisplayInit();
  	i2sInit();
	ServoInit();
}

void loop(){
    
	ReadSamples();
	ComputeCorrelation();
	ComputeFrequency();
	//DisplayConsole();
	DisplayOLED(2,freq);
}
