
#include "Servo.h"
// motor_ctrl.ino
#define DIR_FORWARD	LOW
#define DIR_BACKWARD HIGH
#define PWM1	255
#define PWM2	255
#define DIR1_PIN	9 	// This pin is for direction control, CHANGE RESPECTIVELY to your selection
#define PWM1_PIN	10	// This pin is for PWM control, CHANGE RESPECTIVELY to your selection
#define DIR2_PIN	12
#define PWM2_PIN	11
#define MAX_NUM_BYTES 50
void stopMotor(uint8_t);	//Forward declaration, this func is used to stop the motor
void setup() {
	pinMode(DIR1_PIN, OUTPUT);
	pinMode(PWM1_PIN, OUTPUT);
	pinMode(DIR2_PIN, OUTPUT);
	pinMode(PWM2_PIN, OUTPUT);
	stopMotor(1);
	stopMotor(2);
	Serial.begin(9600);
}

void loop() {
	char command[MAX_NUM_BYTES];
	static char previousCommand1='s';	// Used to detect if one command is repeated, this is used to avoid 'sluggish' in the motor
	static char previousCommand2='s';	// Used to detect if one command is repeated, this is used to avoid 'sluggish' in the motor
	if (Serial.available()>0){
		Serial.readBytesUntil('\n',command, MAX_NUM_BYTES);
		Serial.println(command);
		switch (command[0]) {	// Check rotation's direction of axis 0
		    case 'f':
				if (previousCommand1!='f'){
					stopMotor(1);
					digitalWrite(DIR1_PIN, DIR_FORWARD);
					for (uint16_t pwm_value = 0; pwm_value<PWM1; pwm_value++){
						analogWrite(PWM1_PIN, pwm_value);
					}
				}
				previousCommand1 = 'f';      // do something
			    break;
		    case 'b':
		      // do something
				if (previousCommand1!='b'){
					stopMotor(1);
					digitalWrite(DIR1_PIN, DIR_BACKWARD);
					for (uint16_t pwm_value = 0; pwm_value<PWM1; pwm_value++){
						analogWrite(PWM1_PIN, 255-pwm_value);
						// analogWrite(PWM1_PIN, pwm_value);
					}
				}
				previousCommand1='b';		        
				break;
		    default:	// default mode is stop motor 
		    	stopMotor(1);
		    	previousCommand1='s';
		      // do something
		}
		switch (command[2]) {	// Check rotation's direction of axis 0
		    case 'f':
				if (previousCommand2!='f'){
					stopMotor(2);
					digitalWrite(DIR2_PIN, DIR_FORWARD);
					for (uint16_t pwm_value = 0; pwm_value<PWM2; pwm_value++){
						analogWrite(PWM2_PIN, pwm_value);
					}
				}
				previousCommand2 = 'f';      // do something
			    break;
		    case 'b':
		      // do something
				if (previousCommand2!='b'){
					stopMotor(2);
					digitalWrite(DIR2_PIN, DIR_BACKWARD);
					for (uint16_t pwm_value = 0; pwm_value<PWM2; pwm_value++){
						analogWrite(PWM2_PIN, 255-pwm_value);
						// analogWrite(PWM1_PIN, pwm_value);
					}
				}
				previousCommand2='b';		        
				break;
		    default:	// default mode is stop motor 
		    	stopMotor(2);
		    	previousCommand2='s';
		      // do something
		}
	}
}


void stopMotor(uint8_t motor_number){
	switch (motor_number) {
	    case 1:
			digitalWrite(DIR1_PIN, LOW);
			digitalWrite(PWM1_PIN, LOW);
			delay(10);	      // do something
	      	break;
	    case 2:
		    digitalWrite(DIR2_PIN, LOW);
			digitalWrite(PWM2_PIN, LOW);
			delay(10);	      // do something do something
	        break;
	}
	
}