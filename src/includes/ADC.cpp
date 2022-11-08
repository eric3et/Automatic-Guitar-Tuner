#include "Common.h"


float ReadSamples(){
	buffer_i2s[I2S_DMA_BUF_LEN] = {0};
	counter = 0;
	bufferAverage = 0;
	size_t bytes_read;
  int max = 0;
  int min = 0;
	while(1){
		i2s_read(I2S_NUM_0, &buffer_i2s, sizeof(buffer_i2s), &bytes_read, 15);
		for(int i = 0; i < bytes_read/2; ++i){
			buffer[counter] = buffer_i2s[i] & 0x0FFF;
      if(counter == 0) {
        max = buffer[counter];
        min = buffer[counter];
      }else{
        if(buffer[counter]>max) max = buffer[counter];
        if(buffer[counter]<min) min = buffer[counter];
      }
			bufferAverage += buffer[counter];
			counter++;
			if(counter > NUM_SAMPLES) {
        bufferAverage /= NUM_SAMPLES;
        
        //Serial.println("\n>>>>>>>>>>>>>>>>>>>>\n");
        // for(int i = 0; i < NUM_SAMPLES; i++){
        //   Serial.println(buffer[i]);
        // }
        //Serial.println((float)(bufferAverage-min)/bufferAverage);

        float diff = (float)(bufferAverage-min)/bufferAverage;
        return diff;
			}
		}
	}
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