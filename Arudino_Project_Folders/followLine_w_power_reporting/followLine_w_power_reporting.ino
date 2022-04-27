#include "HUSKYLENS.h"
#include "SoftwareSerial.h"
#include <ESP32Servo.h>
#include <Wire.h>
#include <Adafruit_INA219.h>
#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

// component variables
BluetoothSerial SerialBT;
HUSKYLENS huskylens;
Servo steeringServo;
Servo motorServo;
Adafruit_INA219 ina219; // component for measuring power

// pin definitions
int servoPin = 32;
int motorPin = 33;

//constants
const double Pi = 3.14159;
const int TWO_MIN = 120000;
const int HALF_SEC = 500;

// global variables
int pos = 0;
int motorSpeed = 0;
int BT_ended = 0;
int BT_started = 0;
TaskHandle_t Task1, Task2;

// power variables
float shuntvoltage = 0;
float busvoltage = 0;
float current_mA = 0;
float loadvoltage = 0;
float power_mW = 0;

// timing variables
int cur_millis = 0;
int last_power_read = 0;
int start_race_millis = 0;

// function definitions
void followLine(HUSKYLENSResult result);
void readBluetooth();
void reportPower();
void endLoop();


void powerReadingLoop(void * parameter){
  for(;;){
//  Serial.print("powerReading is running on Core: ");Serial.println( xPortGetCoreID());
    while(BT_started == 0){
      //wait here until race started with bluetooth
  //    readBluetooth();
      Serial.println(BT_started);
    }
    if(millis() > (last_power_read + HALF_SEC)){
        reportPower();
        last_power_read = millis();
      }
  }
}


void setup() {
  Serial.begin(115200);
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  // Bluetooth setup
  SerialBT.begin("PanamaCityBeach"); //Bluetooth device name
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

  //Motor Servo Setup
  motorServo.setPeriodHertz(50);
  motorServo.attach(motorPin, 500,2400);
  delay(50);
  motorServo.write(45);
  delay(1000);
  motorServo.write(50);
  delay(1000);
  motorServo.write(55);
  delay(1000);
  motorServo.write(60);
  delay(1000);
  motorServo.write(0);
  
//  xTaskCreatePinnedToCore(mainLoop, "mainLoop", 1000, NULL, 1, &Task1, 0);
  xTaskCreatePinnedToCore(powerReadingLoop,"powerLoop",1000,NULL,0,&Task2,0);
  
  //record when the program starts for a 2 minute run
  last_power_read = millis();
  start_race_millis = millis();
}

int prev_millis = 0;
HUSKYLENSResult result;
void loop() {
  while(BT_started == 0){
    //wait here until race started with bluetooth
    readBluetooth();
  }
  start_race_millis = millis();
  while(millis() < (start_race_millis + TWO_MIN)){
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
      }
        followLine(result);
      //}
       readBluetooth();
//       while(millis() < cur_millis + 5){
//          //wait approx. 15 ms
//      }
//    if(millis() > (last_power_read + HALF_SEC)){
//      reportPower();
//      last_power_read = millis();
//    }
  }
  endLoop();
}

void followLine(HUSKYLENSResult result){//Put back result
  int xDiff;
  int theta;

  if (result.command == COMMAND_RETURN_ARROW){ // check to see if result returned is an arrow following ling
    //calculations
    xDiff = (int)result.xTarget - 160;
    theta = map(xDiff, -80, 80, 170, 10);
    steeringServo.write(theta);
  }
}

void readBluetooth(){
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }
  if (SerialBT.available()) {
    int letter = SerialBT.read();
    //Serial.write(letter);

    switch(letter){
      case '6':
        motorServo.write(65);
        Serial.println("Wrote moter speed to 65");
        break;
      case '7':
        motorServo.write(70);
        Serial.println("Wrote moter speed to 70");
        break;
      case '8':
        motorServo.write(80);
        Serial.println("Wrote moter speed to 80");
        break;
      case '9':
        motorServo.write(90);
        Serial.println("Wrote moter speed to 90");
        break;
      case '0':
        motorServo.write(0);
        Serial.println("Wrote moter speed to 0");
        break;
      case 'S':
        BT_started = 1;
        BT_ended = 0;
        motorSpeed = 65;
        motorServo.write(motorSpeed);
        break;
      case 'E':
        BT_ended = 1;
        motorSpeed = 0;
        motorServo.write(motorSpeed);
        break;
    }
  }
}

void reportPower(){
  shuntvoltage = ina219.getShuntVoltage_mV();
  busvoltage = ina219.getBusVoltage_V();
  current_mA = ina219.getCurrent_mA();
  power_mW = ina219.getPower_mW();
  loadvoltage = busvoltage + (shuntvoltage / 1000);


  char powerBuffer[50];
  sprintf(powerBuffer, "%s, %s, %s, %s, %s\n",String(shuntvoltage), String(busvoltage),String(current_mA),String(power_mW),String(loadvoltage));
//  Serial.println(powerBuffer);
  const uint8_t* sendBuf = (uint8_t*)powerBuffer;
//  Serial.println(sendBuf);
  SerialBT.write(sendBuf, 50); 
}

void endLoop(){
  while(1){
    //will end here and not exit after 2 minutes
    motorServo.write(0);
    readBluetooth();
  }
}

//void mainLoop(void * parameter){
//  Serial.print("mainLoop is running on Core: ");Serial.println( xPortGetCoreID());
//}
