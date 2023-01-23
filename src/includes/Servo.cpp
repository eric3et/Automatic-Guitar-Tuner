#include "Common.h"


#pragma region Function Declarations

//Function Declarations
void ServoInit();
void StartTuningAlgorithm(int currentString, int freq);
void TurnMotorCW(float time);
void TurnMotorCCW(float time);
void StopMotor();
bool IsFreqWithinTuningBounds();

#pragma endregion Function Declarations

#pragma region Variables

//Servo Setup
const int PWMFreq = 50;//Hz
const int PWMChannel = 0;
const int PWMResolution = 10;
const int Servo_CW_DutyCycle = 102; //2ms
const int Servo_Stop_DutyCycle = 77; //1.5ms
const int Servo_CCW_DutyCycle = 51; //1ms

//Constant Tuning Parameters
const int DefaultCalibration = 200;
const float Upper_Tuning_Bound = 1.2;
const float Lower_Tuning_Bound = 0.8;
const int Max_Motor_Time = 3000; //ms

//Adjustable Tuning Parameters
int tolerance = 2;
int prev_freq = 0;
float Ms_Per_Hz = DefaultCalibration;
int desiredFreq = 0;

#pragma endregion Variables

#pragma region Functions

void ServoInit(){

	ledcSetup(PWMChannel, PWMFreq, PWMResolution);
	ledcAttachPin(SERVO_PIN, PWMChannel);
}

void StartTuningAlgorithm(int currentString, int freq){
	desiredFreq = GetHzForStringNumber(currentString);
	if(IsFreqWithinTuningBounds()){
		if(abs(desiredFreq - freq) < tolerance) //pitch within tolerance = guitar tuned
		// This Step should need to pass 3 times in a row, or 3 times total? Counter?
		{
			Ms_Per_Hz = DefaultCalibration;
			prev_freq = 0;
		}
		else if(desiredFreq - freq > 0)  //pitch up - ccw
		{
			if(prev_freq != 0 || freq != prev_freq){
				Ms_Per_Hz = Ms_Per_Hz/abs(freq - prev_freq);
				float time = Ms_Per_Hz * abs(freq - desiredFreq);
				if(abs(time) < Max_Motor_Time) TurnMotorCCW(time);
			}
		}
		else //pitch down - cw
		{
			if(prev_freq != 0  || freq != prev_freq){
				Ms_Per_Hz = Ms_Per_Hz/abs(freq - prev_freq);
				float time = Ms_Per_Hz * abs(freq - desiredFreq);
				if(abs(time) < Max_Motor_Time) TurnMotorCW(time);
			}
		}
		prev_freq = freq;
		
	}
}

void TurnMotorCW(float time){
	ledcWrite(PWMChannel, Servo_CW_DutyCycle);
	delay(time);
	ledcWrite(PWMChannel, Servo_Stop_DutyCycle);
	StopMotor();
}

void TurnMotorCCW(float time){
	ledcWrite(PWMChannel, Servo_CCW_DutyCycle);
	delay(time);
	ledcWrite(PWMChannel, Servo_Stop_DutyCycle);	
	StopMotor();
}

void StopMotor(){
	ledcWrite(PWMChannel, Servo_CW_DutyCycle);
	delay(100);
	ledcWrite(PWMChannel, Servo_Stop_DutyCycle);
}

bool IsFreqWithinTuningBounds(){
	return (freq < desiredFreq * Upper_Tuning_Bound && freq > desiredFreq * Lower_Tuning_Bound);
	
}

#pragma endregion Functions

