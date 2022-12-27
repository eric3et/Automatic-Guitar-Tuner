#include "Common.h"

//Servo Setup
const int PWMFreq = 50;//Hz
const int PWMChannel = 0;
const int PWMResolution = 10;
const int Servo_CW_DutyCycle = 102; //2ms
const int Servo_Stop_DutyCycle = 77; //1.5ms
const int Servo_CCW_DutyCycle = 51; //1ms
const int DefaultCalibration = 200;
const float Upper_Tuning_Bound = 1.2;
const float Lower_Tuning_Bound = 0.8;

int tolerance = 2;
int prev_freq = 0;
float Ms_Per_Hz = DefaultCalibration;

void TurnMotor(int currentString, int freq){
	int desiredFreq = GetHzForStringNumber(currentString);

	//Serial.println(freq);
	//Serial.println(desiredFreq);

	if(freq < desiredFreq * Upper_Tuning_Bound && freq > desiredFreq * Lower_Tuning_Bound){
		if(abs(desiredFreq - freq) < tolerance) 
		{
			Ms_Per_Hz = DefaultCalibration;
			prev_freq = 0;
		}
		else if(desiredFreq - freq > 0) //pitch up - ccw
		{
			if(prev_freq != 0 || freq != prev_freq){
				Ms_Per_Hz = Ms_Per_Hz/abs(freq - prev_freq);
				float time = Ms_Per_Hz * abs(freq - desiredFreq)*0.001;
				//Serial.println(time);
				if(abs(time) < 3)
				{
					ledcWrite(PWMChannel, Servo_CCW_DutyCycle);
					delay(Ms_Per_Hz * abs(freq - desiredFreq));//180d	
					ledcWrite(PWMChannel, Servo_Stop_DutyCycle);	
				}

			}
			ledcWrite(PWMChannel, Servo_CCW_DutyCycle);
			delay(DefaultCalibration);//180d	
			ledcWrite(PWMChannel, Servo_Stop_DutyCycle);	
		}
		else //pitch down - cw
		{
			if(prev_freq != 0  || freq != prev_freq){
				Ms_Per_Hz = Ms_Per_Hz/abs(freq - prev_freq);
				float time = Ms_Per_Hz * abs(freq - desiredFreq)*0.001;
				//Serial.println(time);
				if(abs(time) < 3)
				{
					ledcWrite(PWMChannel, Servo_CW_DutyCycle);
					delay(Ms_Per_Hz * abs(freq - desiredFreq));//180d	
					ledcWrite(PWMChannel, Servo_Stop_DutyCycle);
				}
			}
			ledcWrite(PWMChannel, Servo_CW_DutyCycle);
			delay(DefaultCalibration);//180d
			ledcWrite(PWMChannel, Servo_Stop_DutyCycle);
			

		}
		prev_freq = freq;
		
	}

	
	




	// ledcWrite(PWMChannel, Servo_CW_DutyCycle);
	// delay(1650);//180d
	// ledcWrite(PWMChannel, Servo_Stop_DutyCycle);
	// delay(1000);//stop
	// ledcWrite(PWMChannel, Servo_CCW_DutyCycle);
	// delay(1650);//180d		
	// ledcWrite(PWMChannel, Servo_Stop_DutyCycle);
}

void ServoInit(){

	ledcSetup(PWMChannel, PWMFreq, PWMResolution);
	ledcAttachPin(SERVO_PIN, PWMChannel);
}

