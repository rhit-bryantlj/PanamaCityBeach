#include <ESP32Servo.h>
#include <analogWrite.h>

Servo steeringServo;
Servo motorServo;

int servoPin = 32;
int motorPin = 33;

int pos;
int motorSpeed;

void setup() {
  // put your setup code here, to run once:
  // Allow allocation of all timers
  Serial.begin(921600);
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  steeringServo.setPeriodHertz(50);    // standard 50 hz servo
  steeringServo.attach(servoPin, 1000, 2000); // attaches the servo on pin 18 to the servo object

  motorServo.setPeriodHertz(50); //standard 50 hz servo
  motorServo.attach(motorPin, 1000,2000);

}

void loop() {

     motorServo.write(0);
     delay(1000);

     for(motorSpeed = 50; motorSpeed <= 100; motorSpeed += 10){
      motorServo.write(motorSpeed);
      delay(250);
     }

//    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
//      // in steps of 1 degree
//      steeringServo.write(pos);    // tell servo to go to position in variable 'pos'
//      delay(15);             // waits 15ms for the servo to reach the position
//    }
//    for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
//      steeringServo.write(pos);    // tell servo to go to position in variable 'pos'
//      delay(15);             // waits 15ms for the servo to reach the position
//    }
}
