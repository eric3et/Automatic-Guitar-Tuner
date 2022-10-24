#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include <driver/i2s.h>

// I2S
#define I2S_SAMPLE_RATE (20000) // Max sampling frequency = 277.777 kHz
#define ADC_INPUT (ADC1_CHANNEL_4) //pin 32
#define I2S_DMA_BUF_LEN (1024)
#define NUM_SAMPLES (1300)
void i2sInit();


//Other Global variables
int counter = 0;
//const int NUM_SAMPLES = 1000; // How many samples
int SamplingRate = I2S_SAMPLE_RATE; //How many Timer Interrupts per second
float correlation[NUM_SAMPLES] = {0};

void ReadSamples();
// void ComputeAverage();
void ComputeCorrelation();
void ComputeFrequency();
void DisplayResults();

uint16_t buffer_i2s[I2S_DMA_BUF_LEN];
short buffer[NUM_SAMPLES] = {0};
int bufferAverage = 0;
int freq = 0;

void setup(){
  Serial.begin(115200);
  setCpuFrequencyMhz(240);

  // Initialize the I2S peripheral
  i2sInit();
}

void loop(){
    
	//Serial.println("1");
	ReadSamples();
	//Serial.println("2");
	ComputeCorrelation();
	//Serial.println("3");
	ComputeFrequency();
	//Serial.println("4");
	DisplayResults();
	
}

void ReadSamples(){
	buffer_i2s[I2S_DMA_BUF_LEN] = {0};
	counter = 0;
	bufferAverage = 0;
	size_t bytes_read;
	while(1){
		i2s_read(I2S_NUM_0, &buffer_i2s, sizeof(buffer_i2s), &bytes_read, 15);
		for(int i = 0; i < bytes_read/2; ++i){
			buffer[counter] = buffer_i2s[i] & 0x0FFF;
			bufferAverage += buffer[counter];
			counter++;
			if(counter > NUM_SAMPLES) {
				bufferAverage /= NUM_SAMPLES;
				return;
			}
		}
	}
	
	
}
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
		
	freq = (float)SamplingRate/(highestPeakIndex + 1)*2;//*2?	


	counter++;

}
void DisplayResults(){
	if(freq > 62 && freq < 450) Serial.println(freq);
	else Serial.println(".\n");
	Serial.println(freq);
	counter = 0;
}

void i2sInit(){
  i2s_config_t i2s_config = {
    .mode = (i2s_mode_t)(I2S_MODE_MASTER | I2S_MODE_RX | I2S_MODE_ADC_BUILT_IN),
    .sample_rate =  I2S_SAMPLE_RATE,              // The format of the signal using ADC_BUILT_IN
    .bits_per_sample = I2S_BITS_PER_SAMPLE_16BIT, // is fixed at 12bit, stereo, MSB
    .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,
    .communication_format = I2S_COMM_FORMAT_STAND_I2S,
    .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
    .dma_buf_count = 8,
    .dma_buf_len = I2S_DMA_BUF_LEN,
    .use_apll = false,
    .tx_desc_auto_clear = false,
    .fixed_mclk = 0
  };
  Serial.printf("Attempting to setup I2S ADC with sampling frequency %d Hz\n", I2S_SAMPLE_RATE);
  if(ESP_OK != i2s_driver_install(I2S_NUM_0, &i2s_config, 0, NULL)){
    Serial.printf("Error installing I2S. Halt!");
    while(1);
  }
  if(ESP_OK != i2s_set_adc_mode(ADC_UNIT_1, ADC_INPUT)){
    Serial.printf("Error setting up ADC. Halt!");
    while(1);
  }
  if(ESP_OK != adc1_config_channel_atten(ADC_INPUT, ADC_ATTEN_DB_11)){
    Serial.printf("Error setting up ADC attenuation. Halt!");
    while(1);
  }
  if(ESP_OK != i2s_adc_enable(I2S_NUM_0)){
    Serial.printf("Error enabling ADC. Halt!");
    while(1);
  }
  Serial.printf("I2S ADC setup ok\n");
}