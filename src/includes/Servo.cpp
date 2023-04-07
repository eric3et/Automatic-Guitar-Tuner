#include "Common.h"

#pragma region Function Declarations

//Function Declarations
void ServoInit();
void StartTuningAlgorithm(int currentString, int frequency);
void TurnMotorCW(float time);
void TurnMotorCCW(float time);
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
const int DefaultCalibration = 55;
const float Upper_Tuning_Bound = 1.2;
const float Lower_Tuning_Bound = 0.8;
const int Max_Motor_Time = 300; //ms

//Adjustable Tuning Parameters
//float tolerance = 0.01;//1% tolerance
int tolerance = 1;
int prev_freq = 0;
float curr_time = 0;
float prev_time = 0;
float Ms_Per_Hz = DefaultCalibration;
int desiredFreq = 0;
int correctCounter = 0;

//PID Parameters
float proportional = 0;
float integral = 0;
float error = 0;
float kp = 0.9;
float ki = 0.54;
float rate = 0.5; //placeholder

#pragma endregion Variables

#pragma region Functions

void ServoInit(){
	ledcSetup(PWMChannel, PWMFreq, PWMResolution);
	ledcAttachPin(SERVO_PIN, PWMChannel);
}

/*
//SIMPLE SOLUTION
void StartTuningAlgorithm(int currentString, int frequency){
	if(gpio_get_level((gpio_num_t)TRIGGER_PIN) == 1) {tuningComplete = false; return;} // if trigger is not pressed, then exit function
	if(tuningComplete) return; // if tuning is flagged as complete for the selected string, then exit function
	desiredFreq = GetHzForStringNumber(currentString);
	if(IsFreqWithinTuningBounds(frequency)){
		// if(abs(desiredFreq - frequency) <= ceil((float)desiredFreq*tolerance))//pitch within tolerance = guitar tuned
		//if(desiredFreq == frequency)//tuned with no tolerance
		if(abs(desiredFreq - frequency) <= tolerance)
		{
			if(prev_freq == 0)
			{
				prev_freq = frequency;
			}
			else
			{
				correctCounter++;
				if(correctCounter > 3)
				{
					tuningComplete = true;
					prev_freq = 0;
				}

			}
		}
		 
		else if(desiredFreq - frequency > 0)  //pitch up - ccw
		{
			correctCounter = 0;
			curr_time = abs(freq - desiredFreq)*DefaultCalibration;
			if(abs(curr_time) < Max_Motor_Time) TurnMotorCCW(curr_time);
			else TurnMotorCCW(Max_Motor_Time);
		}
		else if(desiredFreq - frequency < 0) //pitch down - cw
		{
			correctCounter = 0;
			curr_time = abs(freq - desiredFreq)*DefaultCalibration;
			if(abs(curr_time) < Max_Motor_Time) TurnMotorCW(curr_time);
			else TurnMotorCW(Max_Motor_Time);
		}
	}
}
*/

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
	//ledcWrite(PWMChannel, Servo_CW_DutyCycle);
	// delay(100);
	ledcWrite(PWMChannel, Servo_Stop_DutyCycle);
	delay(100);
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

/*
	PI Controller Implementation
	--------------------------------
	Pout = kp * e(t)
	proportional = setpoint - currentvalue
	setpoint = desiredFreq
	currentvalue = frequency

	Integral implementation, sum the error over time
	ki = ?
	pout = kp*e(t) + (ki*int)*t
	integral = integral + error;

	calibration = (error * calibrationrate) + defaulcalibration ?
*/

	void StartTuningAlgorithm(int currentString, int frequency){
		if(gpio_get_level((gpio_num_t)TRIGGER_PIN) == 1) {tuningComplete = false; return;} // if trigger is not pressed, then exit function
		if(tuningComplete) return; // if tuning is flagged as complete for the selected string, then exit function
		desiredFreq = GetHzForStringNumber(currentString);
		proportional = desiredFreq - frequency;
		integral = integral + proportional;
		error = (kp * proportional) + ((ki * integral));

		if(error < 3)
			Ms_Per_Hz = DefaultCalibration - (error * rate);
		else
			Ms_Per_Hz = DefaultCalibration + (error * rate);

		
		if(IsFreqWithinTuningBounds(frequency)){
			while(abs(desiredFreq - frequency) >= tolerance)
			{
				if(error <= tolerance)
				{
					Ms_Per_Hz = DefaultCalibration;
					integral = 0;
				}
				else
				{
					if(error > 0)
					{
						curr_time = abs(error)*(Ms_Per_Hz);
						if(abs(curr_time) < Max_Motor_Time) TurnMotorCCW(curr_time);
						else TurnMotorCCW(Max_Motor_Time);
					}
					if(error < 0)
					{
						curr_time = abs(error)*(Ms_Per_Hz);
						if(abs(curr_time) < Max_Motor_Time) TurnMotorCW(curr_time);
						else TurnMotorCW(Max_Motor_Time);
					}	
				}

				desiredFreq = GetHzForStringNumber(currentString);
				proportional = desiredFreq - frequency;
				integral = integral + proportional;
				error = (kp * proportional) + ((ki * integral));

				if(error < 3)
					Ms_Per_Hz = DefaultCalibration - (error * rate);
				else
					Ms_Per_Hz = DefaultCalibration + (error * rate);	
			}							
		}

	}


#pragma endregion Functions

