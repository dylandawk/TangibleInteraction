#include <bt_cms.h>
#include <Arduino_LSM6DS3.h>
#include <Adafruit_NeoPixel.h>

#define LIGHT 3
#define PUSH_PIN 8
#define PIXEL_PIN 2
#define NUMPIXELS 1

//Define Neopixel pixel
Adafruit_NeoPixel pixels(NUMPIXELS, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

//Advertise a new service with a unique ID
BLEService flService(FLSID);
BLEDevice central;
//Defines a characterisitc (feature) for a certain service
BLEStringCharacteristic accelVals(FLSID, BLERead | BLEWrite, FLSTRBUFLEN);
//BLEStringCharacteristic respVals(FLSID, BLERead | BLEWrite, FLSTRBUFLEN);

//define button states
int currentState;
int previousState = LOW;
bool isPushPressed, hasMouseBegun;
//define debounce consts
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;//

const int  offset_gx = 1; //IMU at resting for gyroscope gives value of -1 for x (this isn't used in code)
const int  offset_gy = 4; //IMU at resting for gyroscope gives value of -4 for y
const int offset_gz = 2; //IMU at resting for gyroscope gives value of -2 for z

//define
int pixel_hue = 0;
int r, g, b;


void setup() {

  pinMode(LIGHT, OUTPUT);
  Serial.begin(115200);
  //while(!Serial);
  Serial.println("Connected!");

  //If the bluetooth service is not beginning, hang
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }

  //Set a local name for the device
  BLE.setLocalName("FLASH");

  //Select the service to advertise
  BLE.setAdvertisedService(flService);


  Serial.println("Addeding");
  //Attach features to the advertised service
  flService.addCharacteristic(accelVals);//accelerometer values
  //commSertice.addCharacteristic(respVals);//response from central

  //Add the service to the device
  BLE.addService(flService);

  //Write a start message to the central device
  accelVals.writeValue("READY TO GO ");

  //Advertise the service
  BLE.advertise();

  //Initialize Accelerometer/Gyroscope
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  //Initialize button
  pinMode(PUSH_PIN, INPUT_PULLUP);
  //Initialize pixel
  pixels.begin();

  Serial.println("Ready to begin!");
}

void loop() {

  //Attempt too connect with the central device
  //Hangs until connection
  do {
    central = BLE.central();
  } while (!central);

  digitalWrite(LIGHT, LOW);

  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());
  }

  while (central.connected()) {

    //If a message has been written
    if (accelVals.written()) {
      accelVals.readValue(STRBUF, FLSTRBUFLEN);

      //adds code that activates when connected
      readGyro();
      readAccel();
      readPushButton();
      handleLED();

      //Is the ON String
      if (!strcmp(STRBUF, ONSTR)) {
        Serial.println("IN!");
        digitalWrite(LIGHT, HIGH);
        strcpy(STRBUF, ACKSTR);
        accelVals.writeValue(STRBUF);
        continue;
      }


    }

  }

  digitalWrite(LIGHT, LOW);
  Serial.print("Disconnected from: ");
  Serial.println(central.address());

}
