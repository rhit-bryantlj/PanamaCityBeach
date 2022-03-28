
#include <ESP32Servo.h>

Servo steeringServo;  // create servo object to control a servo
Servo motorServo;

int pos = 0;    // variable to store the servo position
int motorSpeed = 0; // var to control motorSpeed

int servoPin = 32;
int motorPin = 33;



void setup() {
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  Serial.begin(115200);
  
//  steeringServo.attach(servoPin); // attaches the servo on pin 18 to the servo object

  motorServo.setPeriodHertz(50);    // standard 50 hz servo
  motorServo.attach(motorPin, 1000, 2000);
//  motorServo.write(100);
}

void loop() {

 // run motor 2 seconds then turn off
  motorServo.write(80);
  delay(2000);
  motorServo.write(0);
//  delay(2000);
  
    // Sweep servo back and forth
  for (pos = 120; pos <= 160; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    steeringServo.write(pos);    // tell servo to go to position in variable 'pos'
    delay(15);             // waits 15ms for the servo to reach the position
  }
  for (pos = 160; pos >= 20; pos -= 1) { // goes from 180 degrees to 0 degrees
    steeringServo.write(pos);    // tell servo to go to position in variable 'pos'
    delay(15);             // waits 15ms for the servo to reach the position
  }
}
