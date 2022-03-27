
#include <ESP32Servo.h>

Servo steeringServo;  // create servo object to control a servo
Servo motorServo;

int pos = 0;    // variable to store the servo position
int motorSpeed = 0; // var to control motorSpeed

int servoPin = 32;
int motorPin = 33;



void setup() {
  Serial.begin(115200);
  steeringServo.attach(servoPin); // attaches the servo on pin 18 to the servo object


//  motorServo.setPeriodHertz(50);
//  motorServo.attach(motorPin);
//  motorServo.write(100);
}

void loop() {
 
//  motorServo.write(80);
//  delay(2000);
  
//  motorServo.write(0);
  for (pos = 10; pos <= 170; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    steeringServo.write(pos);    // tell servo to go to position in variable 'pos'
    delay(15);             // waits 15ms for the servo to reach the position
  }
  for (pos = 170; pos >= 10; pos -= 1) { // goes from 180 degrees to 0 degrees
    steeringServo.write(pos);    // tell servo to go to position in variable 'pos'
    delay(15);             // waits 15ms for the servo to reach the position
  }
}
