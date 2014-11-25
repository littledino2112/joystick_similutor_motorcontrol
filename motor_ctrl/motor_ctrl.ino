
#include "Servo.h"
// motor_ctrl.ino
#define DIR1_FORWARD	LOW
#define DIR1_BACKWARD	HIGH
#define PWM1	125
#define DIR1_PIN	9 	// This pin is for direction control, CHANGE RESPECTIVELY to your selection
#define PWM1_PIN	10	// This pin is for PWM control, CHANGE RESPECTIVELY to your selection
void stopMotor();	//Forward declaration, this func is used to stop the motor
void setup() {
	pinMode(DIR1_PIN, OUTPUT);
	pinMode(PWM1_PIN, OUTPUT);
	stopMotor();
	Serial.begin(9600);
}

void loop() {
	char command;
	if (Serial.available()>0){
		command = Serial.read();
		if (command=='1'){
			// Turn motor forward
			stopMotor();
			digitalWrite(DIR1_PIN, DIR1_FORWARD);
			analogWrite(PWM1_PIN, PWM1);
		}
		else if (command=='2'){
			// Turn motor backward
			stopMotor();
			digitalWrite(DIR1_PIN, DIR1_BACKWARD);
			// analogWrite(PWM1_PIN, 255-PWM1);
			analogWrite(PWM1_PIN, PWM1);
		}
		else if (command=='0'){
			stopMotor();
		}
	}

}


void stopMotor(){
	digitalWrite(DIR1_PIN, DIR1_FORWARD);
	digitalWrite(PWM1_PIN, LOW);
	delay(50);
}