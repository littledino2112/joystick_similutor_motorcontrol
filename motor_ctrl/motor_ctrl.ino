
#include "Servo.h"
// motor_ctrl.ino
Servo myservo;
uint8_t resolution=0;
char temp[10];
void setup() {
	// myservo.attach(9,1000,2000);
	// myservo.write(0);
	Serial.begin(9600);
}

void loop() {
	// switch (myservo.read()) {
	//      case 0:
	//      	myservo.write(90);
	//        // do something
	//        break;
	//      case 90:
	//      	myservo.write(180);
	//        // do something
	//        break;
	//      case 180:
	//      	myservo.write(0);
	//         break;
	//  } 

}

void serialEvent(){
	char temp = Serial.read();
	Serial.println(temp);
}

