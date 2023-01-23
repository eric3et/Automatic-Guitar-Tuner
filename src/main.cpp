#include "includes/Common.h"


void setup(){
  	Serial.begin(BAUD_RATE);
  	setCpuFrequencyMhz(CPU_FREQ_MHZ);
	DisplayInit();
  	i2sInit();
	ServoInit();
	InterruptsInit();
}

void loop(){
	ReadSamples();
	ComputeCorrelation();
	ComputeFrequency();
	DisplayOLED(currentString,freq);
	StartTuningAlgorithm(currentString,freq);
}
