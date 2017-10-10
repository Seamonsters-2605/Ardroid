/*
 * Generic Robot Control - Interfaces Adafruit motor shield v2 to Android
 * App Inventor app via the Ardiono101's BluetoothLE
 */

#include <CurieBLE.h>
#include <Wire.h>
#include <Adafruit_MotorShield.h>
//#include "utility/Adafruit_PWMServoDriver.h"

#define NUM_SHIELDS 2

#define SHIELD_DC_MOTORS 4
#define SHIELD_STEPPER_MOTORS 2

#define NUM_DC_MOTORS NUM_SHIELDS*SHIELD_DC_MOTORS
#define NUM_STEPPER_MOTORS NUM_SHIELDS*SHIELD_STEPPER_MOTORS

// #define LOGGING

//
//   BLE Services Setup:
//
BLEPeripheral blePeripheral;  // BLE Peripheral Device (the board you're programming)
BLEService RobotService("19B20000-E8F2-537E-4F6C-D104768A1214"); // BLE robot Service
                       
// BLE slider Characteristics - custom 128-bit UUID, read and writable by central
BLEIntCharacteristic dcMotorCharacteristics[NUM_DC_MOTORS] = {
  BLEIntCharacteristic("0001", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("0002", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("0003", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("0004", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("0011", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("0012", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("0013", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("0014", BLERead | BLEWrite | BLENotify),
};
BLEIntCharacteristic stepperMotorSettingsCharacteristics[NUM_STEPPER_MOTORS] = {
  BLEIntCharacteristic("0005", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("0006", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("0015", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("0016", BLERead | BLEWrite | BLENotify),
};
BLEIntCharacteristic stepperMotorPositionCharacteristics[NUM_STEPPER_MOTORS] = {
  BLEIntCharacteristic("0007", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("0008", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("0017", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("0018", BLERead | BLEWrite | BLENotify),
};
BLEIntCharacteristic stepperMotorSpeedCharacteristics[NUM_STEPPER_MOTORS] = {
  BLEIntCharacteristic("0009", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("000A", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("0019", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("001A", BLERead | BLEWrite | BLENotify),
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
  #ifdef LOGGING
  while (!Serial) ;  // wait for serial connection to open
  Serial.begin(9600);
  Serial.println(F("ARDROID"));
  #endif

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

  #ifdef LOGGING
  Serial.println(F("BLE LED Peripheral Initialized"));
  #endif
}


void setupMotors() {
  // TODO: allow multiple shields
  for(int i=0; i<NUM_DC_MOTORS; i++)
    DCMotors[i] = AFMS.getMotor(i%SHIELD_DC_MOTORS + 1); // motor port
  for(int i=0; i<NUM_STEPPER_MOTORS; i++) {
    StepperMotors[i] = AFMS.getStepper(200, i%SHIELD_STEPPER_MOTORS + 1); // steps per rotation, motor port
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

  #ifdef LOGGING
  Serial.println(F("Motors Initialized"));
  #endif
}



void loop() {
  // listen for BLE peripherals to connect:
  BLECentral central = blePeripheral.central();

  // if a central is connected to peripheral:
  if (central) {
    #ifdef LOGGING
    Serial.print(F("Connected to central (android device): "));
    // print the central's MAC address:
    Serial.println(central.address());
    #endif

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

      int value; // reused in multiple places for getting characteristic values
      
      for(int i=0; i<NUM_DC_MOTORS; i++) {
        if(dcMotorCharacteristics[i].written()) {
          updateDCMotors = true;
          value = dcMotorCharacteristics[i].value();
          dcMotorSpeeds[i] = value;
          #ifdef LOGGING
          Serial.print(F("Set DC motor "));
          Serial.print(i + 1);
          Serial.print(F(" to "));
          Serial.println(value);
          #endif
        }
      }

      for(int i=0; i<NUM_STEPPER_MOTORS; i++) {
        if(stepperMotorSettingsCharacteristics[i].written()) {
          value = stepperMotorSettingsCharacteristics[i].value();
          if(value > 0) {
            // set max speed in RPM
            StepperMotors[i]->setSpeed(value);
            stepperMotorSettingsCharacteristics[i].setValue(0);
            #ifdef LOGGING
            Serial.print(F("Set stepper "));
            Serial.print(i + 1);
            Serial.print(F(" max speed to "));
            Serial.println(value);
            #endif
          } else if(value < 0) {
            // set steps per rotation
            // requires creating a new Adafruit_StepperMotor object
            StepperMotors[i] = AFMS.getStepper(-value, i%SHIELD_STEPPER_MOTORS + 1); // TODO: update for multiple shields!
            stepperMotorSettingsCharacteristics[i].setValue(0);
            #ifdef LOGGING
            Serial.print(F("Set stepper "));
            Serial.print(i + 1);
            Serial.print(F(" steps per rotation to "));
            Serial.println(-value);
            #endif
          }
        }

        if(stepperMotorPositionCharacteristics[i].written()) {
          value = stepperMotorPositionCharacteristics[i].value();
          stepperMotorTargetPositions[i] = value;
          #ifdef LOGGING
          Serial.print(F("Set stepper "));
          Serial.print(i + 1);
          Serial.print(F(" position to "));
          Serial.println(value);
          #endif
        }

        if(stepperMotorSpeedCharacteristics[i].written()) {
          value = stepperMotorSpeedCharacteristics[i].value();
          stepperMotorSpeeds[i] = value;
          #ifdef LOGGING
          Serial.print(F("Set stepper "));
          Serial.print(i + 1);
          Serial.print(F(" speed to "));
          Serial.println(value);
          #endif
        }
      }
      
      if (dcMotorSpeeds[0] > 255) {   //handle special case of stop command (slider1>255)
        #ifdef LOGGING
        Serial.println(F("Recieved all-off command"));
        #endif
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
    #ifdef LOGGING
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
    #endif
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

