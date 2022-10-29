#include "Common.h"

//Servo Setup
#define SERVO_PIN 13
#define SERVO_ACTIVATION_PIN 27
const int PWMFreq = 50; /* 50 Hz */
const int PWMChannel = 0;
const int PWMResolution = 10;
const int Servo_CW_DutyCycle = 102;
const int Servo_Stop_DutyCycle = 77;
const int Servo_CCW_DutyCycle = 51;


void TurnMotor(){
	if(digitalRead(SERVO_ACTIVATION_PIN) == LOW){
		ledcWrite(PWMChannel, Servo_CW_DutyCycle);//Spin Servo CW for 2 seconds
		delay(1650);//180d
		ledcWrite(PWMChannel, Servo_Stop_DutyCycle); //Stops Servo for 2 seconds
		delay(1000);
		ledcWrite(PWMChannel, Servo_CCW_DutyCycle);//Spin Servo CCW for 2 seconds
		delay(1650);//180d		
		ledcWrite(PWMChannel, Servo_Stop_DutyCycle); //Stops Servo for 2 seconds
		delay(1000);
	}
}

void ServoInit(){

	pinMode(SERVO_ACTIVATION_PIN, INPUT_PULLUP);

	ledcSetup(PWMChannel, PWMFreq, PWMResolution);
	/* Attach the LED PWM Channel to the GPIO Pin */
	ledcAttachPin(SERVO_PIN, PWMChannel);
}