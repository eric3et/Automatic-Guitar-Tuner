#include "Common.h"


void ComputeCorrelation(){
	int shift = 0;

	for(int shift = 0; shift < NUM_SAMPLES; shift++){
		correlation[shift] = 0;
		for(int i = 0; i < shift; i++) correlation[shift] += ((float)abs(buffer[i]-bufferAverage)/buffer[i]);
		for(int j = shift; j < NUM_SAMPLES-shift; j++) correlation[shift] += ((float)abs(buffer[j]-buffer[j+shift])/buffer[j]);
		correlation[shift] = 1-(correlation[shift]/NUM_SAMPLES);
	}
	counter++;
}

void ComputeFrequency(){
	int correlation_size = sizeof(correlation)/sizeof(correlation[0]);
	float highestPeakValue = 0;
	int highestPeakIndex = 0;
	bool threshold = false;
	float tempFreq = 0;

	for(int i = 0; i < correlation_size; i++){
		if(!threshold){
			if(correlation[i] < correlation[i+1] && correlation[i] <= correlation[i-1]) threshold = true;
		}else{
			if(correlation[i]>highestPeakValue){
				highestPeakIndex = i;
				highestPeakValue = correlation[i];
			}
		}
	}
	tempFreq = (float)I2S_SAMPLE_RATE/(highestPeakIndex + 1)*2;	
 
	Serial.println(tempFreq);
	int desiredFreq = GetHzForStringNumber(currentString);
	if (abs(tempFreq - desiredFreq) <= desiredFreq+(desiredFreq*0.3)) freq = tempFreq;
	else if(currentString != 0 && abs(tempFreq*2 - desiredFreq) <= desiredFreq+(desiredFreq*0.15)) freq = tempFreq*2;
	else if(currentString != 0 && abs(tempFreq*4 - desiredFreq) <= desiredFreq+(desiredFreq*0.10)) freq = tempFreq*4;
	else freq = 0;

}