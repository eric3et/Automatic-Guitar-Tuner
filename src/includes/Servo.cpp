#include "Common.h"

//Servo Setup
const int PWMFreq = 50;//Hz
const int PWMChannel = 0;
const int PWMResolution = 10;
const int Servo_CW_DutyCycle = 102; //2ms
const int Servo_Stop_DutyCycle = 77; //1.5ms
const int Servo_CCW_DutyCycle = 51; //1ms


void TurnMotor(){
	if(digitalRead(SERVO_ACTIVATION_PIN) == LOW){
		//if trigger is release a ISR will be called that will detach the servo pin. This Servo Pin Will be attached later.

		ledcWrite(PWMChannel, Servo_CW_DutyCycle);
		delay(1650);//180d
		ledcWrite(PWMChannel, Servo_Stop_DutyCycle);
		delay(1000);//stop
		ledcWrite(PWMChannel, Servo_CCW_DutyCycle);
		delay(1650);//180d		
		ledcWrite(PWMChannel, Servo_Stop_DutyCycle);
	}
}

void ServoInit(){

	pinMode(SERVO_ACTIVATION_PIN, INPUT_PULLUP);

	ledcSetup(PWMChannel, PWMFreq, PWMResolution);
	ledcAttachPin(SERVO_PIN, PWMChannel);
}