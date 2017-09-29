/*
 * Log writes to BLE characteristics
 */

#include <CurieBLE.h>

// largest possible value without running out of SRAM
#define NUM_BLE_CHARACTERISTICS 19

//
//   BLE Services Setup:
//
BLEPeripheral blePeripheral;  // BLE Peripheral Device (the board you're programming)
BLEService RobotService("19B20000-E8F2-537E-4F6C-D104768A1214"); // BLE robot Service
                       
// BLE Characteristics - custom 128-bit UUID, read and writable by central
BLEIntCharacteristic bleCharacteristics[NUM_BLE_CHARACTERISTICS] = {
  BLEIntCharacteristic("19B20001-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("19B20002-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("19B20003-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("19B20004-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("19B20005-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("19B20006-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("19B20007-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("19B20008-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("19B20009-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("19B2000A-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite | BLENotify),

  BLEIntCharacteristic("19B20011-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("19B20012-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("19B20013-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("19B20014-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("19B20015-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("19B20016-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("19B20017-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("19B20018-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("19B20019-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite | BLENotify)
};

void setup() {
  while (!Serial) ; // only works when connected
  Serial.begin(9600);

  Serial.println(F("BLE Test"));

  //****BLE Setup:*******
  // set advertised local name and service UUID:
  blePeripheral.setLocalName("ARDROID");
  blePeripheral.setAdvertisedServiceUuid(RobotService.uuid());

  // add service and characteristic:
  blePeripheral.addAttribute(RobotService);

  for(int i=0; i<NUM_BLE_CHARACTERISTICS; i++) {
    blePeripheral.addAttribute(bleCharacteristics[i]);
    //set initial value
    bleCharacteristics[i].setValue(0);
  }

  Serial.println(F("Start advertising..."));
  Serial.println(F("(if nothing prints after this it didn't work)"));
  // begin advertising BLE service:
  blePeripheral.begin();

  Serial.println(F("Ready"));
}


void loop() {
  // listen for BLE peripherals to connect:
  BLECentral central = blePeripheral.central();

  // if a central is connected to peripheral:
  if (central) {
    Serial.print(F("Connected: "));
    // print the central's MAC address:
    Serial.println(central.address());

    // while the central is still connected to peripheral:
    while (central.connected()) {
      for(int i=0; i<NUM_BLE_CHARACTERISTICS; i++) {
        if(bleCharacteristics[i].written()) {
          Serial.print(i + 1);
          Serial.print(F(" is "));
          Serial.println(bleCharacteristics[i].value());
        }
      }
      delay(100);
    }

    Serial.print(F("Disconnected: "));
    Serial.println(central.address());
  }
}

