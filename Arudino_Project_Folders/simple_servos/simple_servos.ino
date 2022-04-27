#include <ESP32Servo.h>
#include <Wire.h>
#include <Adafruit_INA219.h>

Servo steeringServo;  // create servo object to control a servo
Servo motorServo;

Adafruit_INA219 ina219; // component for measuring power

int pos = 0;    // variable to store the servo position
int motorSpeed = 0; // var to control motorSpeed
float shuntvoltage = 0;
float busvoltage = 0;
float current_mA = 0;
float loadvoltage = 0;
float power_mW = 0;

int servoPin = 32;
int motorPin = 33;

void setup() {
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  Serial.begin(115200);
  Serial.println("sup");
  while(!Serial) {
    // will pause Zero, Leonardo, etc until serial console opens
    delay(1);
  }

  // Initialize the INA219.
  // By default the initialization will use the largest range (32V, 2A).  However
  // you can call a setCalibration function to change this range (see comments).
  if (! ina219.begin()) {
    Serial.println("Failed to find INA219 chip");
    while (1) { delay(10); }
  }
  
  

  // Steering Servo Setup
  steeringServo.setPeriodHertz(50);
  steeringServo.attach(servoPin, 1000, 2000);
 // attaches the servo on pin 18 to the servo object

  motorServo.setPeriodHertz(50);    // standard 50 hz servo
  motorServo.attach(motorPin, 500, 2400);
  delay(500);
  motorSpeed = 45;
}

void loop() {
  if (Serial.available()> 0) {
    //int k = Serial.parseInt();
    //if(k >0){
      //motorSpeed = Serial.parseInt();
      pos = Serial.parseInt();
      Serial.read();
    //}
  }
  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();
  loadvoltage = busvoltage + (shuntvoltage / 1000);
  
  Serial.print("Bus Voltage:   "); Serial.print(busvoltage); Serial.println(" V");
  Serial.print("Shunt Voltage: "); Serial.print(shuntvoltage); Serial.println(" mV");
  Serial.print("Load Voltage:  "); Serial.print(loadvoltage); Serial.println(" V");
  Serial.print("Current:       "); Serial.print(current_mA); Serial.println(" mA");
  Serial.print("Power:         "); Serial.print(power_mW); Serial.println(" mW");
  Serial.print("Angle:         "); Serial.print(pos); Serial.println(" degrees");
  Serial.println("");

//  if(motorSpeed < 110){
//    motorSpeed +=5;
//  }
  motorServo.write(motorSpeed);
  steeringServo.write(pos);
   
  delay(2000);



 // run motor 2 seconds then turn off
//  motorServo.write(80);
//  delay(2000);
//  motorServo.write(0);
//  delay(2000);

 
  
    // Sweep servo back and forth
//  for (pos = 0; pos <= 170; pos += 1) { // goes from 0 degrees to 180 degrees
//    // in steps of 1 degree
        // tell servo to go to position in variable 'pos'
//    delay(15);             // waits 15ms for the servo to reach the position
//  }
//  for (pos = 170; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
//    steeringServo.write(pos);    // tell servo to go to position in variable 'pos'
//    delay(15);             // waits 15ms for the servo to reach the position
//  }
}
