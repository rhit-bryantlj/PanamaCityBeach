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

// pin definitions
int servoPin = 32;

//constants
const double Pi = 3.14159;

// global variables
int pos = 0;

void printAngleCalculations(HUSKYLENSResult result);

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
}

void loop() {
  if (!huskylens.request()) Serial.println(F("Fail to request data from HUSKYLENS, recheck the connection!"));
  else if(!huskylens.isLearned()) Serial.println(F("Nothing learned, press learn button on HUSKYLENS to learn one!"));
  else if(!huskylens.available()) Serial.println(F("No block or arrow appears on the screen!"));
  else
  {
//      Serial.println(F("###########"));
      while (huskylens.available())
      {
          HUSKYLENSResult result = huskylens.read();
          printAngleCalculations(result);
      }    
      delay(2000);
  }

//  printAngleCalculations(); //put back result
//  delay(5000);
  
}

void printAngleCalculations(HUSKYLENSResult result){//Put back result
//  int test_xO = 96;
//  int test_xT = 72;
//  int test_yO = 138;
//  int test_yT = 88;
  double xDiff;
  double yDiff;
  int theta;
  int servoAngle;

  //calculations
  xDiff = result.xTarget - result.xOrigin;
  yDiff = result.yOrigin - result.yTarget;
  theta = acos(xDiff/yDiff) * 180 / Pi;
  Serial.println(String()+F("xDiff: ") + xDiff + F(" yDiff: ") + yDiff + F(" theta: ") + theta);
  steeringServo.write(theta);
  
//  if (result.command == COMMAND_RETURN_ARROW){
//      Serial.println(String()+F("Arrow:xOrigin=")+result.xOrigin+F(",yOrigin=")+result.yOrigin+F(",xTarget=")+result.xTarget+F(",yTarget=")+result.yTarget+F(",ID=")+result.ID);
//  }
//  else{
//      Serial.println("Object unknown!");
//  }
}
