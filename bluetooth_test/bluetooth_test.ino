/*
 * Log writes to BLE characteristics
 */

#include <CurieBLE.h>

#define NUM_BLE_CHARACTERISTICS 20

//
//   BLE Services Setup:
//
BLEPeripheral blePeripheral;  // BLE Peripheral Device (the board you're programming)
BLEService RobotService("19B20000-E8F2-537E-4F6C-D104768A1214"); // BLE robot Service
                       
// BLE Characteristics - custom 128-bit UUID, read and writable by central
BLEIntCharacteristic bleCharacteristics[NUM_BLE_CHARACTERISTICS] = {
  BLEIntCharacteristic("0001", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("0002", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("0003", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("0004", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("0005", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("0006", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("0007", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("0008", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("0009", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("000A", BLERead | BLEWrite | BLENotify),

  BLEIntCharacteristic("0011", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("0012", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("0013", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("0014", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("0015", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("0016", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("0017", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("0018", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("0019", BLERead | BLEWrite | BLENotify),
  BLEIntCharacteristic("001A", BLERead | BLEWrite | BLENotify)
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

    // check initial values
    for(int i=0; i<NUM_BLE_CHARACTERISTICS; i++) {
      if(bleCharacteristics[i].value() != 0) {
        Serial.print(i + 1);
        Serial.print(F(" is "));
        Serial.println(bleCharacteristics[i].value());
      }
    }

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

