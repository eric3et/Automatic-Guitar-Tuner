#include "Common.h"

#pragma region Function Declarations

//Function Declarations
void ServoInit();
void StartTuningAlgorithm(int currentString, int frequency);
void TurnMotorCW(float time);
void TurnMotorCCW(float time);
void StopMotor();
bool IsFreqWithinTuningBounds(int frequency);

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
const int DefaultCalibration = 50;
const float Upper_Tuning_Bound = 1.2;
const float Lower_Tuning_Bound = 0.8;
const int Max_Motor_Time = 300; //ms

//Adjustable Tuning Parameters
float tolerance = 0.02;
int prev_freq = 0;
float curr_time = 0;
float prev_time = 0;
float Ms_Per_Hz = DefaultCalibration;
int desiredFreq = 0;

#pragma endregion Variables

#pragma region Functions

void ServoInit(){
	ledcSetup(PWMChannel, PWMFreq, PWMResolution);
	ledcAttachPin(SERVO_PIN, PWMChannel);
}

//SIMPLE SOLUTION
void StartTuningAlgorithm(int currentString, int frequency){
	desiredFreq = GetHzForStringNumber(currentString);
	if(IsFreqWithinTuningBounds(frequency)){
		if(abs(desiredFreq - frequency) <= ceil((float)desiredFreq*tolerance)); //pitch within tolerance = guitar tuned
		else if(desiredFreq - frequency > 0)  //pitch up - ccw
		{
			curr_time = abs(freq - desiredFreq)*DefaultCalibration;
			if(abs(curr_time) < Max_Motor_Time) TurnMotorCCW(curr_time);
			else TurnMotorCCW(Max_Motor_Time);
		}
		else if(desiredFreq - frequency < 0) //pitch down - cw
		{
			curr_time = abs(freq - desiredFreq)*DefaultCalibration;
			if(abs(curr_time) < Max_Motor_Time) TurnMotorCW(curr_time);
			else TurnMotorCW(Max_Motor_Time);
		}
	}
}

void TurnMotorCW(float time){
	ledcWrite(PWMChannel, Servo_CW_DutyCycle);
	delay(time);
	ledcWrite(PWMChannel, Servo_Stop_DutyCycle);
	StopMotor();
	Serial.println(time);
}

void TurnMotorCCW(float time){
	ledcWrite(PWMChannel, Servo_CCW_DutyCycle);
	delay(time);
	ledcWrite(PWMChannel, Servo_Stop_DutyCycle);	
	StopMotor();
	Serial.println(time);
}

void StopMotor(){
	ledcWrite(PWMChannel, Servo_CW_DutyCycle);
	delay(100);
	ledcWrite(PWMChannel, Servo_Stop_DutyCycle);
}

bool IsFreqWithinTuningBounds(int frequency){
	return (frequency < desiredFreq * Upper_Tuning_Bound && frequency > desiredFreq * Lower_Tuning_Bound);
	
}

// SMART SOLUTION
// void StartTuningAlgorithm(int currentString, int frequency){
// 	desiredFreq = GetHzForStringNumber(currentString);
// 	Serial.println(Ms_Per_Hz);	
// 	if(IsFreqWithinTuningBounds(frequency)){
// 		if(abs(desiredFreq - frequency) <= ceil((float)desiredFreq*tolerance)) //pitch within tolerance = guitar tuned
// 		// This Step should need to pass 3 times in a row, or 3 times total? Counter?
// 		{
// 			Ms_Per_Hz = DefaultCalibration;
// 			prev_freq = 0;
// 		}
// 		else if(desiredFreq - frequency > 0)  //pitch up - ccw
// 		{
// 			if(prev_freq != 0 && frequency != prev_freq){
// 				Ms_Per_Hz = abs(curr_time - prev_time)/abs(frequency - prev_freq);
// 				curr_time = Ms_Per_Hz * abs(frequency - desiredFreq);
// 				if(abs(curr_time) < Max_Motor_Time) TurnMotorCCW(curr_time);
// 				else TurnMotorCCW(Max_Motor_Time);
// 			}
// 		}
// 		else if(desiredFreq - frequency > 0) //pitch down - cw
// 		{
// 			if(prev_freq != 0  && frequency != prev_freq){
// 				Ms_Per_Hz = abs(curr_time - prev_time)/abs(frequency - prev_freq);
// 				curr_time = Ms_Per_Hz * abs(frequency - desiredFreq);
// 				if(abs(curr_time) < Max_Motor_Time) TurnMotorCW(curr_time);
// 				else TurnMotorCW(Max_Motor_Time);
// 				prev_freq = frequency;
// 				prev_time = curr_time;
// 			}
// 		}
// 	}
// }

#pragma endregion Functions

