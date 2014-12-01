
#include "Servo.h"
// motor_ctrl.ino
#define DIR_FORWARD	LOW
#define DIR_BACKWARD HIGH
#define DIR1_PIN	9 	// This pin is for direction control, CHANGE RESPECTIVELY to your selection
#define PWM1_PIN	10	// This pin is for PWM control, CHANGE RESPECTIVELY to your selection
#define DIR2_PIN	12
#define PWM2_PIN	11
#define DIR3_PIN	7
#define	PWM3_PIN	6
#define MAX_NUM_BYTES 50
uint16_t PWM1 = 255;
uint16_t PWM2 = 255;
uint16_t PWM3 = 255;
void stopMotor(uint8_t);	//Forward declaration, this func is used to stop the motor
void setup() {
	// All pins to be configured in OUTPUT mode
	pinMode(DIR1_PIN, OUTPUT);	
	pinMode(PWM1_PIN, OUTPUT);
	pinMode(DIR2_PIN, OUTPUT);
	pinMode(PWM2_PIN, OUTPUT);
	pinMode(DIR3_PIN, OUTPUT);
	pinMode(PWM3_PIN, OUTPUT);
	stopMotor(1);
	stopMotor(2);
	stopMotor(3);
	Serial.begin(9600);
}

void loop() {
	char command[MAX_NUM_BYTES];
	static char previousCommand1='s';	// Used to detect if one command is repeated, this is used to avoid 'sluggish' in the motor
	static char previousCommand2='s';	// Used to detect if one command is repeated, this is used to avoid 'sluggish' in the motor
	static char previousCommand3='s';	// Used to detect if one command is repeated, this is used to avoid 'sluggish' in the motor
	if (Serial.available()>0){
		uint8_t number_bytes=0;
		number_bytes=Serial.readBytesUntil('\n',command, MAX_NUM_BYTES);
		Serial.println(command);

		/* This portion of the code is used to parse the motor speed configed in Python GUI app 
		 * Data is passed via serial port under this format [number],[number],[number], 3 number according to 3 axes' speed respectively
		 * (e.g. 255,255,255,). Since transmitted data is string, so it needs to be converted to number */

		if (command[0]>'0' & command[0]<'9'){
			// data is passed under format [number],[number],[number],
			uint8_t value = 0;
			uint8_t count = 0;
			bool repeat1=false;
			bool repeat2=false;
			bool repeat3=false;
			for (uint8_t i=0; i<number_bytes-1; i++){
				if (command[i]==','){
					count++;
				}
				else {
					value = value*10 + (command[i]-'0');
				}
				// Passing value to pwm according to [count](number of semicolon detected)
				if (count==1) {
					if (!repeat1){
						PWM1=value;
						value=0;
						repeat1=true;
					}
				}
				else if (count==2) {
					if (!repeat2){
						PWM2=value;
						value=0;
						repeat2=true;
					}
				}
				else if (count==3) {
					if (!repeat3){
						PWM3=value;
						value=0;
						repeat3=true;
					}
				}
			}
			
		}

		/* Motor control part: First direction of motor is parsed from transmitted data via serial command
		 * The transitted data is under this format [direction], [direction], [direction] 
		 * where [direction] is 'f'(forward), 'b'(backward) or 's'(stop) 
		 * previousCommandx is used to check if one direction is already selected previously. 
		 * Without this, the motor movement becomes 'choppy' */
		else {
			switch (command[0]) {	// Check rotation's direction of axis 0
			    case 'f':
					if (previousCommand1!='f'){
						stopMotor(1);
						digitalWrite(DIR1_PIN, DIR_FORWARD);
						for (uint16_t pwm_value = 0; pwm_value<PWM1; pwm_value++){
							analogWrite(PWM1_PIN, pwm_value);
						}
					}
					previousCommand1 = 'f';      //  
				    break;
			    case 'b':
			      // do something
					if (previousCommand1!='b'){
						stopMotor(1);
						digitalWrite(DIR1_PIN, DIR_BACKWARD);
						for (uint16_t pwm_value = 0; pwm_value<PWM1; pwm_value++){
							// analogWrite(PWM1_PIN, 255-pwm_value);
							analogWrite(PWM1_PIN, pwm_value);
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
							// analogWrite(PWM2_PIN, 255-pwm_value);
							analogWrite(PWM2_PIN, pwm_value);
						}
					}
					previousCommand2='b';		        
					break;
			    default:	// default mode is stop motor 
			    	stopMotor(2);
			    	previousCommand2='s';
			      // do something
			}

			switch (command[4]) {	// Check rotation's direction of axis 0
			    case 'f':
					if (previousCommand3!='f'){
						stopMotor(3);
						digitalWrite(DIR3_PIN, DIR_FORWARD);
						for (uint16_t pwm_value = 0; pwm_value<PWM3; pwm_value++){
							analogWrite(PWM3_PIN, pwm_value);
						}
					}
					previousCommand3 = 'f';      // do something
				    break;
			    case 'b':
			      // do something
					if (previousCommand3!='b'){
						stopMotor(3);
						digitalWrite(DIR3_PIN, DIR_BACKWARD);
						for (uint16_t pwm_value = 0; pwm_value<PWM3; pwm_value++){
							// analogWrite(PWM3_PIN, 255-pwm_value);
							analogWrite(PWM3_PIN, pwm_value);
						}
					}
					previousCommand3='b';		        
					break;
			    default:	// default mode is stop motor 
			    	stopMotor(3);
			    	previousCommand3='s';
			      // do something
			}
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
	    case 3:
		    digitalWrite(DIR3_PIN, LOW);
			digitalWrite(PWM3_PIN, LOW);
			delay(10);	      // do something do something
	        break;   
	}
	
}