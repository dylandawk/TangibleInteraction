#include <bt_cms.h>

#define LIGHT 3

//Advertise a new service with a unique ID
BLEService flService(FLSID);
BLEDevice central;
//Defines a characterisitc (feature) for a certain service
BLEStringCharacteristic accelVals(FLSID, BLERead | BLEWrite, FLSTRBUFLEN);
//BLEStringCharacteristic respVals(FLSID, BLERead | BLEWrite, FLSTRBUFLEN);

void setup() {
  
  pinMode(LIGHT, OUTPUT);
  Serial.begin(115200);
  //while(!Serial);
  Serial.println("Connected!");

  //If the bluetooth service is not beginning, hang
  if(!BLE.begin()){
    Serial.println("starting BLE failed!");

    while(1);
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

  Serial.println("Ready to begin!");
}

void loop() {

  //Attempt too connect with the central device
  //Hangs until connection
  do{
    central = BLE.central();
  }while(!central);

  digitalWrite(LIGHT, LOW);

  if(central){
    Serial.print("Connected to central: ");
    Serial.println(central.address());
  }

  while(central.connected()){

    //If a message has been written
    if(accelVals.written()){
      accelVals.readValue(STRBUF, FLSTRBUFLEN);
      
      //Is the ON String
      if(!strcmp(STRBUF, ONSTR)){
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
