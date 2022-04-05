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
  motorServo.attach(motorPin, 1000,2000);
  delay(20);
  motorServo.write(0);
}

void loop() {
    cur_millis = millis();
    if (!huskylens.request()) Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
    else if(!huskylens.isLearned()) Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));
  //  else if(!huskylens.available()) Serial.println(F("No block or arrow appears on the screen!"));
    else
    {
        while (huskylens.available())
        {
            HUSKYLENSResult result = huskylens.read();
            followLine(result);
        }
  //      delay(15);
    }
     readBluetooth();
     while(millis() < cur_millis + 10){
        //wait approx. 15 ms
    }
}

void followLine(HUSKYLENSResult result){//Put back result
  double xDiff;
  double yDiff;
  int theta;
  int servoAngle;

  if (result.command == COMMAND_RETURN_ARROW){ // check to see if result returned is an arrow following ling
    //calculations
    xDiff = result.xTarget - result.xOrigin;
    yDiff = result.yOrigin - result.yTarget;
    theta = acos(xDiff/yDiff) * 180 / Pi;
    SerialBT.write(theta);

    if(theta > 95 && theta< 150){
      steeringServo.write(theta + 10);
    }
    if(theta < 85 && theta > 30) {
      steeringServo.write(theta - 10);
    }else{
      steeringServo.write(theta);
    }

    if((BT_ended != 1) && (theta > 100 || theta < 75)){
      motorSpeed = 55;
      motorServo.write(55);
    } 
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
