/*
 * Generic Robot Control - Interfaces Adafruit motor shield v2 to Android
 * App Inventor app via the Ardiono101's BluetoothLE
 */

#include <CurieBLE.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
//#include "utility/Adafruit_PWMServoDriver.h"

#define NUM_DC_MOTORS 4
#define NUM_STEPPER_MOTORS 2

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
BLEIntCharacteristic stepperMotorSettingsCharacteristics[NUM_STEPPER_MOTORS] = {
  BLEIntCharacteristic("19B20005-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("19B20006-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite | BLENotify),
};
BLEIntCharacteristic stepperMotorPositionCharacteristics[NUM_STEPPER_MOTORS] = {
  BLEIntCharacteristic("19B20007-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("19B20008-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite | BLENotify),
};
BLEIntCharacteristic stepperMotorSpeedCharacteristics[NUM_STEPPER_MOTORS] = {
  BLEIntCharacteristic("19B20009-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("19B2000A-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite | BLENotify),
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
Adafruit_StepperMotor * StepperMotors[NUM_STEPPER_MOTORS];


const int ledPin = 13; // pin to use for the LED


void setup() {
  //while (!Serial) ;  //uncomment to use serial moitor to debug
  Serial.begin(9600);

  Serial.println("ARDROID");

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
  for(int i=0; i<NUM_STEPPER_MOTORS; i++) {
    blePeripheral.addAttribute(stepperMotorSettingsCharacteristics[i]);
    blePeripheral.addAttribute(stepperMotorPositionCharacteristics[i]);
    blePeripheral.addAttribute(stepperMotorSpeedCharacteristics[i]);
    stepperMotorSettingsCharacteristics[i].setValue(0);
    stepperMotorPositionCharacteristics[i].setValue(0);
    stepperMotorSpeedCharacteristics[i].setValue(0);
  }
  
  // begin advertising BLE service:
  blePeripheral.begin();

  Serial.println("BLE LED Peripheral Initialized");
}


void setupMotors() {
  for(int i=0; i<NUM_DC_MOTORS; i++)
    DCMotors[i] = AFMS.getMotor(i + 1); // motor port
  for(int i=0; i<NUM_STEPPER_MOTORS; i++) {
    StepperMotors[i] = AFMS.getStepper(200, i + 1); // steps per rotation, motor port
  }

  //****Motor Shield Setup:*******
  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  
  // Set the speed to start, from 0 (off) to 255 (max speed)
  for(int i=0; i<NUM_DC_MOTORS; i++) {
    DCMotors[i]->setSpeed(0);
    DCMotors[i]->run(FORWARD);
  }
  for(int i=0; i<NUM_STEPPER_MOTORS; i++) {
    StepperMotors[i]->setSpeed(100);
  }

  Serial.println("Motors Initialized");
}



void loop() {
  // listen for BLE peripherals to connect:
  BLECentral central = blePeripheral.central();

  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central (android device): ");
    // print the central's MAC address:
    Serial.println(central.address());

    int dcMotorSpeeds[NUM_DC_MOTORS];
    for(int i = 0; i < NUM_DC_MOTORS; i++)
      dcMotorSpeeds[i] = 0;
    int stepperMotorSpeeds[NUM_STEPPER_MOTORS];
    float stepperMotorAccumulators[NUM_STEPPER_MOTORS];
    int stepperMotorTargetPositions[NUM_STEPPER_MOTORS];
    int stepperMotorCurrentPositions[NUM_STEPPER_MOTORS];
    for(int i = 0; i < NUM_STEPPER_MOTORS; i++) {
      stepperMotorSpeeds[i] = 0;
      stepperMotorAccumulators[i] = 0;
      stepperMotorTargetPositions[i] = 0;
      stepperMotorCurrentPositions[i] = 0;
    }
    bool updateDCMotors = false;

    // while the central is still connected to peripheral:
    unsigned long lastMillis = millis();
    unsigned long timeDiff = 0;
    while (central.connected()) {
      
      for(int i=0; i<NUM_DC_MOTORS; i++) {
        if(dcMotorCharacteristics[i].written()) {
          updateDCMotors = true;
          dcMotorSpeeds[i] = dcMotorCharacteristics[i].value();
        }
      }

      for(int i=0; i<NUM_STEPPER_MOTORS; i++) {
        if(stepperMotorSettingsCharacteristics[i].written()) {
          int value = stepperMotorSettingsCharacteristics[i].value();
          if(value > 0) {
            // set max speed in RPM
            StepperMotors[i]->setSpeed(value);
            stepperMotorSettingsCharacteristics[i].setValue(0);
            Serial.print("Set stepper ");
            Serial.print(i + 1);
            Serial.print(" max speed to ");
            Serial.println(value);
          } else if(value < 0) {
            // set steps per rotation
            // requires creating a new Adafruit_StepperMotor object
            StepperMotors[i] = AFMS.getStepper(-value, i + 1);
            stepperMotorSettingsCharacteristics[i].setValue(0);
            Serial.print("Set stepper ");
            Serial.print(i + 1);
            Serial.print(" steps per rotation to ");
            Serial.println(-value);
          }
        }

        if(stepperMotorPositionCharacteristics[i].written()) {
          stepperMotorTargetPositions[i] = stepperMotorPositionCharacteristics[i].value();
        }

        if(stepperMotorSpeedCharacteristics[i].written()) {
          stepperMotorSpeeds[i] = stepperMotorSpeedCharacteristics[i].value();
        }
      }
      
      if (dcMotorSpeeds[0] > 255) {   //handle special case of stop command (slider1>255)
        Serial.println("Recieved all-off command");
        for(int i=0; i<NUM_DC_MOTORS; i++) {
          dcMotorSpeeds[i] = 0;
          dcMotorCharacteristics[i].setValue(0);
        }
        for(int i=0; i<NUM_STEPPER_MOTORS; i++) {
          stepperMotorSpeeds[i] = 0;
          stepperMotorSpeedCharacteristics[i].setValue(0);
          stepperMotorTargetPositions[i] = stepperMotorCurrentPositions[i];
          stepperMotorPositionCharacteristics[i].setValue(stepperMotorTargetPositions[i]);
        }
      }

      if (updateDCMotors) {
        for(int i=0; i<NUM_DC_MOTORS; i++) {
          if(dcMotorSpeeds[i] > 255)
            dcMotorSpeeds[i] = 255;
          else if(dcMotorSpeeds[i] < -255)
            dcMotorSpeeds[i] = -255;
          setDCMotorSpeed(DCMotors[i], dcMotorSpeeds[i]);
        }

        updateDCMotors = false;
      }

      for(int i=0; i<NUM_STEPPER_MOTORS; i++) {
        // position mode
        if(stepperMotorTargetPositions[i] != stepperMotorCurrentPositions[i]) {
          moveStepperMotor(StepperMotors[i], stepperMotorTargetPositions[i] - stepperMotorCurrentPositions[i]);
          stepperMotorCurrentPositions[i] = stepperMotorTargetPositions[i];
        }

        // speed mode
        stepperMotorAccumulators[i] += (float)stepperMotorSpeeds[i] * (float)timeDiff / 1000.0;
        float movement = stepperMotorAccumulators[i];
        if(movement > 1.0) {
          int steps = (int)movement;
          stepperMotorAccumulators[i] -= steps;
          moveStepperMotor(StepperMotors[i], steps);
        }
        if(movement < 1.0) {
          int steps = -(int)(-movement);
          stepperMotorAccumulators[i] -= steps;
          moveStepperMotor(StepperMotors[i], steps);
        }
      }

      timeDiff = millis() - lastMillis;
      if(timeDiff > 200)
        timeDiff = 200; // too high values cause problems
      lastMillis = millis();
    } // while  connected

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  } //if central
  
}  //end loop


// special function to handle negative speeds correctly
void setDCMotorSpeed(Adafruit_DCMotor * motor, int speed) {
  if(speed == 0) {
    motor->run(RELEASE);
  } else if(speed < 0) {
    motor->run(BACKWARD);
    motor->setSpeed(-speed);
  } else {
    motor->run(FORWARD);
    motor->setSpeed(speed);
  }
}

// special function to handle negative and zero values correctly
void moveStepperMotor(Adafruit_StepperMotor * motor, int steps) {
  if(steps == 0) {
    ; // do nothing
  } else if(steps < 0) {
    motor->step(-steps, BACKWARD, SINGLE);
  } else {
    motor->step(steps, FORWARD, SINGLE);
  }
}

