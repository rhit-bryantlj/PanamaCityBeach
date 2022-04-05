#include "HUSKYLENS.h"
#include "SoftwareSerial.h"
#include <ESP32Servo.h>
#include <Wire.h>
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
HUSKYLENS huskylens;
Servo steeringServo;
Servo motorServo;

// pin definitions
int servoPin = 32;
int motorPin = 33;

//constants
const double Pi = 3.14159;

// global variables
int pos = 0;
int motorSpeed = 0;
int cur_millis = 0;
int BT_ended = 1;

void followLine(HUSKYLENSResult result);
void readBluetooth();

void setup() {
  Serial.begin(115200);
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  // Bluetooth setup
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");

  // HuskeyLens setup
  Wire.begin();
  while (!huskylens.begin(Wire))
  {
      Serial.println(F("Begin failed!"));
      Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>I2C)"));
      Serial.println(F("2.Please recheck the connection."));
      delay(100);
  }

  // Steering Servo Setup
  steeringServo.setPeriodHertz(50);
  steeringServo.attach(servoPin, 1000, 2000);

  //Motor Servo Setup
  motorServo.setPeriodHertz(50);
  motorServo.attach(motorPin, 500,2400);
  delay(20);
  motorServo.write(45);
     delay(2000);
    motorServo.write(55);
    delay(2000);
    motorServo.write(60);
    delay(2000);
    motorServo.write(65);
    delay(2000);
    motorServo.write(68);
}

int prev_millis = 0;
HUSKYLENSResult result;
void loop() {
    cur_millis = millis();
    if (!huskylens.request()) Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
    else if(!huskylens.isLearned()) Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));
  //  else if(!huskylens.available()) Serial.println(F("No block or arrow appears on the screen!"));
    else
    {
        while (huskylens.available())
        {
            result = huskylens.read();
        } 
  //      delay(15);
    }

    //if(cur_millis - prev_millis > ){
     // prev_millis = cur_millis;
      followLine(result);
    //}
   //  readBluetooth();
//     while(millis() < cur_millis + 5){
//        //wait approx. 15 ms
//    }
}

void followLine(HUSKYLENSResult result){//Put back result
  int xDiff;
  double yDiff;
  int theta;
  int servoAngle;

  if (result.command == COMMAND_RETURN_ARROW){ // check to see if result returned is an arrow following ling
    //calculations
    xDiff = (int)result.xTarget - 160;
//    yDiff = result.yOrigin - result.yTarget;
//    theta = acos(xDiff/yDiff) * 180 / Pi;
//    int angle = theta;
//    SerialBT.write(theta);

//    theta = map(theta, 0, 180, 10, 170);
//    theta = constrain(theta, 10, 170);
//    steeringServo.write(theta);
//    if(theta > 95 && theta< 150){
//      steeringServo.write(theta + 10);
//    }
//    if(theta < 85 && theta > 30) {
//      steeringServo.write(theta - 10);
//    }else{
//      steeringServo.write(theta);
//    }
      theta = map(xDiff, -80, 80, 170, 10);
      //theta = constrain(xDiff, 0, 180);
      //steeringServo.write(theta);

//    if(/*(BT_ended != 1) &&*/ (theta > 120 || theta < 60)){
//      //motorSpeed = 55;
//      motorServo.write(65);
//      delay(50);
//      motorServo.write(70);
//      delay(50);
//    }

    char buffer[50];
    sprintf(buffer, "xTarget: %d  diff: %d  theta: %d  ", result.xTarget, xDiff, theta);
    //Serial.println(buffer);
    steeringServo.write(theta);
//    else if(BT_ended != 1){
//      motorServo.write(70);
//    }
  }
}

void readBluetooth(){
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    int letter = SerialBT.read();
    Serial.write(letter);
    if(letter == 'S'){
      BT_ended = 0;
      motorSpeed = 60;
      motorServo.write(motorSpeed);
    }
    if(letter == 'E'){
      BT_ended = 1;
      motorSpeed = 0;
      motorServo.write(motorSpeed);
    }
  }
}
