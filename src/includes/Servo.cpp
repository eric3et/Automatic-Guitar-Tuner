#include "Common.h"

#pragma region Function Declarations

//Function Declarations
void ServoInit();
void StartTuningAlgorithm(int currentString, float frequency);
void TurnMotorCW(float time);
void TurnMotorCCW(float time);
bool IsFreqWithinTuningBounds(int frequency);
void ResetTuningParameters();

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
const int Max_Motor_Time = 1000; //ms

//Adjustable Tuning Parameters
//float tolerance = 0.01;//1% tolerance
int tolerance = 2;
int prev_freq = 0;
float curr_time = 0;
float prev_time = 0;
float Ms_Per_Hz = DefaultCalibration;
int desiredFreq = 0;
int correctCounter = 0;
bool standardTuning = true;

//PID Parameters
float proportional = 0;
float integral = 0;
float error = 0;
float kp = 0.9;
float ki = 0.54;
float rate = 0.5; //placeholder
float range = 0;

#pragma endregion Variables

#pragma region Functions

void ServoInit(){
	ledcSetup(PWMChannel, PWMFreq, PWMResolution);
	ledcAttachPin(SERVO_PIN, PWMChannel);
}

	// PI Controller Implementation
	// --------------------------------
	// Pout = kp * e(t)
	// proportional = setpoint - currentvalue
	// setpoint = desiredFreq
	// currentvalue = frequency

	// Integral implementation, sum the error over time
	// ki = ?
	// pout = kp*e(t) + (ki*int)*t
	// integral = integral + error;

	// calibration = (error * calibrationrate) + defaulcalibration ?

//Erics PID SOLUTION
void StartTuningAlgorithm(int currentString, float frequency){


	if(gpio_get_level((gpio_num_t)TRIGGER_PIN) == 1) 
	{
		// if trigger is not pressed, then exit function, and reset state machine.
		tuningComplete = false; 
		calibrate = 0; 
		return;
	} 
	if(tuningComplete) 
	{
		// if tuning is flagged as complete for the selected string, then exit function, and reset state machine.
		calibrate = 0; 
		return;
	} 

	// Get desired frequency for current string
	desiredFreq = GetHzForStringNumber(currentString);

	//if current frequency is within tolerance of desired frequency 3 times in a row, mark tuning as complete.
	if(abs(desiredFreq - frequency) <= tolerance)
	{
		calibrate = 0;
		if(prev_freq == 0) prev_freq = frequency;
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
	else
	{
		correctCounter = 0;
		if(IsFreqWithinTuningBounds(frequency))
		{
			switch (calibrate)
			{
				case 0:
				Serial.println("CASE 0:");
					//initial calibration
					//We need to come up with a solution that triggers this initial setup when...
					//	1. picking a new string
					//	2. If device is set to E string for too long, it will trigger this when the user starts actively using the device

					// perhaps when the trigger button is released, the initial calibration will run again, once the trigger is held.
					
					// FUNCTIONALITY
					// this state will attempt to move the motor in the correct direction.
					// it will then switch to the secondary calibratioin state.

					ResetTuningParameters();

					calibrate = 1;
					prev_freq = frequency;
					proportional = abs(desiredFreq - frequency);
					Serial.print("subtract");
					Serial.println(desiredFreq - frequency);
					if(desiredFreq - frequency > 0)  //pitch up - ccw
					{
						TurnMotorCCW(DefaultCalibration);
					}
					else if(desiredFreq - frequency < 0) //pitch down - cw
					{
						TurnMotorCW(DefaultCalibration);
					}
					break;

				case 1:
				Serial.println("CASE 1:");
					//secondary calibration

					// FUNCTIONALITY
					// this state will compare frequency from initial calibration, and determine a calibration value, Ms_Per_Hz
					// if the frequency went the wrong way, it will switch tuner to a clockwise tighened guitar,





					//flip tuning rotation if guitar is strung other way
					//if(abs(desiredFreq-frequency > proportional)) standardTuning = !standardTuning;
					if(prev_freq - frequency == 0){
						calibrate = 0;
						return;
					}
					Ms_Per_Hz = DefaultCalibration/abs(prev_freq - frequency);

					prev_freq = frequency;
					range = abs(desiredFreq - frequency);

					if(desiredFreq - frequency > 0)  //pitch up - ccw
					{
						TurnMotorCCW(Ms_Per_Hz*range);
					}
					else if(desiredFreq - frequency < 0) //pitch down - cw
					{
						TurnMotorCW(Ms_Per_Hz*range);
					}

					calibrate = 2;
					break;

				case 2:
				Serial.println("CASE 2:");
					//continuous PI error correction
					//after initial calibration this branch of code will run until the tuner or is no longer in use, or the user selects a different string.
					// calibrate = 0 should not happen in this step, it should happen when the trigger is released, or when string select button is pressed.
					
					// FUNCTIONALITY
					// Tune the guitar using the constantly calibrated error value, recompute the error value upon each iteration.



					proportional = desiredFreq - frequency;
					if(proportional <= 0) {
						calibrate = 0;
						return;
					}
					integral = integral + proportional;
					error = (kp * proportional) + ((ki * integral));

					Ms_Per_Hz += Ms_Per_Hz*(error / proportional);
					
					prev_freq = frequency;
					
					if(desiredFreq - frequency > 0)  //pitch up - ccw
					{
						TurnMotorCCW(Ms_Per_Hz*proportional);
					}
					else if(desiredFreq - frequency < 0) //pitch down - cw
					{
						TurnMotorCW(Ms_Per_Hz*proportional);
					}
					break;

				default:
					break;
					
			}
		}
	}
}




void TurnMotorCW(float time){
	//used to invert tuning direction if necessary
	// if(!standardTuning){
	// 	TurnMotorCCW(time);
	// 	return;
	// }
	
	if (time>Max_Motor_Time) 
	{
		calibrate = 0;
		time = DefaultCalibration;
	}
	
	Serial.println("cw:");
	Serial.println(time);
	ledcWrite(PWMChannel, Servo_CW_DutyCycle);
	delay(time);
	ledcWrite(PWMChannel, Servo_Stop_DutyCycle);
	StopMotor();
	Serial.println(time);
}

void TurnMotorCCW(float time){
	//used to invert tuning direction if necessary
	// if(!standardTuning){
	// 	TurnMotorCW(time);
	// 	return;
	// }
	
	if (time>Max_Motor_Time) 
	{
		calibrate = 0;
		time = DefaultCalibration;
	}
	Serial.println("ccw:");
	Serial.println(time);
	ledcWrite(PWMChannel, Servo_CCW_DutyCycle);
	delay(time);
	ledcWrite(PWMChannel, Servo_Stop_DutyCycle);	
	StopMotor();
	Serial.println(time);
}

void StopMotor(){
	ledcWrite(PWMChannel, Servo_Stop_DutyCycle);
	delay(100);
}

bool IsFreqWithinTuningBounds(int frequency){
	return (frequency < desiredFreq * Upper_Tuning_Bound && frequency > desiredFreq * Lower_Tuning_Bound);
	
}

void ResetTuningParameters(){
	tolerance = 1;
	prev_freq = 0;
	curr_time = 0;
	prev_time = 0;
	Ms_Per_Hz = DefaultCalibration;
	correctCounter = 0;
	standardTuning = true;

	//PID Parameters
	proportional = 0;
	integral = 0;
	error = 0;
	range = 0;
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

	// void StartTuningAlgorithm(int currentString, int frequency){
	// 	if(gpio_get_level((gpio_num_t)TRIGGER_PIN) == 1) {tuningComplete = false; return;} // if trigger is not pressed, then exit function
	// 	if(tuningComplete) return; // if tuning is flagged as complete for the selected string, then exit function
	// 	desiredFreq = GetHzForStringNumber(currentString);
	// 	proportional = desiredFreq - frequency;
	// 	integral = integral + proportional;
	// 	error = (kp * proportional) + ((ki * integral));

	// 	if(error < 3)
	// 		Ms_Per_Hz = DefaultCalibration - (error * rate);
	// 	else
	// 		Ms_Per_Hz = DefaultCalibration + (error * rate);

		
	// 	if(IsFreqWithinTuningBounds(frequency)){
	// 		// while(abs(desiredFreq - frequency) >= tolerance)
	// 		// {
	// 			if(abs(error) <= tolerance)
	// 			{
	// 				Ms_Per_Hz = DefaultCalibration;
	// 				integral = 0;
	// 			}
	// 			else
	// 			{
	// 				if(error > 0)
	// 				{
	// 					curr_time = abs(error)*(Ms_Per_Hz);
	// 					if(abs(curr_time) < Max_Motor_Time) TurnMotorCCW(curr_time);
	// 					else TurnMotorCCW(Max_Motor_Time);
	// 				}
	// 				else if(error < 0)
	// 				{
	// 					curr_time = abs(error)*(Ms_Per_Hz);
	// 					if(abs(curr_time) < Max_Motor_Time) TurnMotorCW(curr_time);
	// 					else TurnMotorCW(Max_Motor_Time);
	// 				}	
	// 			}

	// 			desiredFreq = GetHzForStringNumber(currentString);
	// 			proportional = desiredFreq - frequency;
	// 			integral = integral + proportional;
	// 			error = (kp * proportional) + ((ki * integral));

	// 			if(error < 3)
	// 				Ms_Per_Hz = DefaultCalibration - (error * rate);
	// 			else
	// 				Ms_Per_Hz = DefaultCalibration + (error * rate);	
	// 		//}							
	// 	}

	// }


#pragma endregion Functions

