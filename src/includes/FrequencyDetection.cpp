#include "Common.h"


void ComputeCorrelation(){
	int shift = 0;

	for(int shift = 0; shift < NUM_SAMPLES; shift++){
		correlation[shift] = 0;
		for(int i = 0; i < shift; i++){
			correlation[shift] += ((float)abs(buffer[i]-bufferAverage)/buffer[i]);
		}
		for(int j = shift; j < NUM_SAMPLES-shift; j++){
			correlation[shift] += ((float)abs(buffer[j]-buffer[j+shift])/buffer[j]);
		}
		correlation[shift] = 1-(correlation[shift]/NUM_SAMPLES);
	}
	counter++;

	//Display Correlation Data
	// for(short i = 0; i < NUM_SAMPLES; i++){
	//     Serial.printf("%lf\n",correlation[i]);
	// }
}
void ComputeFrequency(){
	int correlation_size = sizeof(correlation)/sizeof(correlation[0]);
	float highestPeakValue = 0;
	int highestPeakIndex = 0;
	bool threshold = false;

	for(int i = 0; i < correlation_size; i++){
		if(!threshold){
			if(correlation[i] < correlation[i+1] && correlation[i] <= correlation[i-1]){
				threshold = true;
			} 
		}else{
			if(correlation[i]>highestPeakValue){
				highestPeakIndex = i;
				highestPeakValue = correlation[i];
			}
		}
	}
		
	freq = (float)I2S_SAMPLE_RATE/(highestPeakIndex + 1)*2;	

}