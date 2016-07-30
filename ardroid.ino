/*
 * Generic Robot Control - Interfaces Adafruit motor shield v2 to Android
 * App Inventor app via the Ardiono101's BluetoothLE
 */

#include <CurieBLE.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
//#include "utility/Adafruit_PWMServoDriver.h"

#define NUM_DC_MOTORS 4

//
//   BLE Services Setup:
//
BLEPeripheral blePeripheral;  // BLE Peripheral Device (the board you're programming)
BLEService RobotService("19B20000-E8F2-537E-4F6C-D104768A1214"); // BLE robot Service
                       
// BLE slider Characteristics - custom 128-bit UUID, read and writable by central
BLEIntCharacteristic dcMotorCharacteristics[NUM_DC_MOTORS] = {
  BLEIntCharacteristic("19B20001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("19B20002-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("19B20003-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("19B20004-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite | BLENotify),
};


//
//   Motor Shield Setup:
//
// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61); 

// Motor ports start at Port 1 at index 0
Adafruit_DCMotor * DCMotors[NUM_DC_MOTORS];


const int ledPin = 13; // pin to use for the LED


void setup() {
  //while (!Serial) ;  //uncomment to use serial moitor to debug
  Serial.begin(9600);

  // set LED pin to output mode
  pinMode(ledPin, OUTPUT);

  setupBLE();

  setupMotors();
}


void setupBLE() {
  //****BLE Setup:*******
  // set advertised local name and service UUID:
  blePeripheral.setLocalName("ARDROID");
  blePeripheral.setAdvertisedServiceUuid(RobotService.uuid());

  // add service and characteristic:
  blePeripheral.addAttribute(RobotService);
  
  for(int i=0; i<NUM_DC_MOTORS; i++) {
    blePeripheral.addAttribute(dcMotorCharacteristics[i]);
    //set initial value
    dcMotorCharacteristics[i].setValue(0);
  }

  // begin advertising BLE service:
  blePeripheral.begin();

  Serial.println("BLE LED Peripheral Initialized");
}


void setupMotors() {
  for(int i=0; i<NUM_DC_MOTORS; i++)
    DCMotors[i] = AFMS.getMotor(i + 1); // motor port

  //****Motor Shield Setup:*******
  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  
  // Set the speed to start, from 0 (off) to 255 (max speed)
  for(int i=0; i<NUM_DC_MOTORS; i++) {
    DCMotors[i]->setSpeed(0);
    DCMotors[i]->run(FORWARD);
  }
}



void loop() {
  // listen for BLE peripherals to connect:
  BLECentral central = blePeripheral.central();
  static int motorSpeeds[NUM_DC_MOTORS] = {0,0,0,0};
  bool handlenewval = false;

  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central (android device): ");
    // print the central's MAC address:
    Serial.println(central.address());

    // while the central is still connected to peripheral:
    while (central.connected()) {

      for(int i=0; i<NUM_DC_MOTORS; i++) {
        if(dcMotorCharacteristics[i].written()) {
          handlenewval = true;
          motorSpeeds[i] = dcMotorCharacteristics[i].value();
        }
      }
      
      if (motorSpeeds[0] > 255) {   //handle special case of stop command (slider1>255)
        Serial.println("Recieved all-off command");
        for(int i=0; i<NUM_DC_MOTORS; i++) {
          motorSpeeds[i] = 0;
          dcMotorCharacteristics[i].setValue(0);
        }
      }

      if (handlenewval) {
        for(int i=0; i<NUM_DC_MOTORS; i++) {
          if(motorSpeeds[i] > 255)
            motorSpeeds[i] = 255;
          else if(motorSpeeds[i] < -255)
            motorSpeeds[i] = -255;
          setDCMotorSpeed(DCMotors[i], motorSpeeds[i]);
        }

        handlenewval = false;
      }  //if handle new val
      
    } // while  connected

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  } //if central
  
}  //end loop


// special function to handle negative speeds correctly
void setDCMotorSpeed(Adafruit_DCMotor * motor, int speed) {
  if(speed < 0) {
    motor->run(BACKWARD);
    motor->setSpeed(-speed);
  } else {
    motor->run(FORWARD);
    motor->setSpeed(speed);
  }
}

