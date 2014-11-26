
#include "Servo.h"
// motor_ctrl.ino
#define DIR1_FORWARD	LOW
#define DIR1_BACKWARD	HIGH
#define PWM1	255
#define DIR1_PIN	9 	// This pin is for direction control, CHANGE RESPECTIVELY to your selection
#define PWM1_PIN	10	// This pin is for PWM control, CHANGE RESPECTIVELY to your selection
#define MAX_NUM_BYTES 50
void stopMotor();	//Forward declaration, this func is used to stop the motor
void setup() {
	pinMode(DIR1_PIN, OUTPUT);
	pinMode(PWM1_PIN, OUTPUT);
	stopMotor();
	Serial.begin(9600);
}

void loop() {
	char command[MAX_NUM_BYTES];
	char previousCommand='s';	// Used to detect if one command is repeated, this is used to avoid 'sluggish' in the motor
	if (Serial.available()>0){
		Serial.readBytesUntil('\n',command, MAX_NUM_BYTES);
		Serial.println(command);
		if (command[0]=='f'){
			// Turn motor forward
			if (previousCommand!='f'){
				stopMotor();
				digitalWrite(DIR1_PIN, DIR1_FORWARD);
				for (uint16_t pwm_value = 0; pwm_value<PWM1; pwm_value++){
					analogWrite(PWM1_PIN, pwm_value);
				}
			}
			previousCommand = 'f';
		}
		else if (command[0]=='b'){
			// Turn motor backward
			if (previousCommand!='b'){
				stopMotor();
				digitalWrite(DIR1_PIN, DIR1_BACKWARD);
				for (uint16_t pwm_value = 0; pwm_value<PWM1; pwm_value++){
					// analogWrite(PWM1_PIN, 255-pwm_value);
					analogWrite(PWM1_PIN, pwm_value);
				}
			}
			previousCommand='b';
		}
		else if (command[0]=='s'){
			stopMotor();
		}
	}

}


void stopMotor(){
	digitalWrite(DIR1_PIN, DIR1_FORWARD);
	digitalWrite(PWM1_PIN, LOW);
	delay(50);
}