#include <bt_cms.h>
#include <Arduino_LSM6DS3.h>
#include <Adafruit_NeoPixel.h>

#define LIGHT 3
#define PUSH_PIN 8
#define PIXEL_PIN 2
#define NUMPIXELS 1

//NEOPIXEL DEFINITIONS
//Define Neopixel pixel
Adafruit_NeoPixel pixels(NUMPIXELS, PIXEL_PIN, NEO_GRB + NEO_KHZ800);
int pixel_hue = 0;
int r, g, b;


//BLUETOOTH DEFINITIONS
//Advertise a new service with a unique ID
BLEService flService(FLSID);
BLEDevice central;
//Defines a characterisitc (feature) for a certain service
BLECharacteristic accelVals(FLSID, BLERead | BLEWrite, FLTXBUFLEN, true);
//BLEStringCharacteristic respVals(FLSID, BLERead | BLEWrite, FLTXBUFLEN);


//FLASHLIGHT DEFINITIONS
//define button states
int currentState;
int previousState = LOW;
bool isPushPressed, hasMouseBegun;
//define debounce consts
unsigned long lastDebounceTime = 0;
const unsigned long debounceDelay = 50;//


void btSetup(){
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
}


void flSetup(){
  //Initialize Accelerometer/Gyroscope
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  //Initialize button
  pinMode(PUSH_PIN, INPUT_PULLUP);
  //Initialize pixel
  pixels.begin();
  

}

void setup() {

  pinMode(LIGHT, OUTPUT);
  Serial.begin(115200);
  //while(!Serial);
  Serial.println("Connected!");

  btSetup();

  Serial.println("BT SETUP!");
  flSetup();
  Serial.println("FL SETUP!");
  Serial.println("Ready to begin!");
}

void viewData(){
  int i;
  for(i = AX; i <= GZ; ++i){
    Serial.print(vals[i]);Serial.print('\t');
  }

  Serial.println();
}


void readSensors(){
  readGyro();
  readAccel();
  readPushButton();
}

void loop() {

  //Attempt too connect with the central device
  //Polls until connection
  do {
    central = BLE.central();
  } while (!central);


  
  if (central) {
    Serial.print("Connected to central: ");
    Serial.println(central.address());
  }


  while (central.connected()) {

    readSensors();
    handleLED();

    //Write values to be sent over bluetooth
    accelVals.writeValue((void *)vals, FLTXBUFLEN);

    //Conform to an acceptable transmisison rate
    delay(15);
  }


  Serial.print("Disconnected from: ");
  Serial.println(central.address());

}
