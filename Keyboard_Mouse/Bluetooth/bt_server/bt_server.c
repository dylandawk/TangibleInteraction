#include <bt_cms.h>

#define LED 2
#define SBAUD 115200


BLEDevice peripheral;
BLECharacteristic accelVals;
int waiting;

void setup() {
  waiting = 0;
  pinMode(LED, OUTPUT);
  Serial.begin(SBAUD);

  //Waits for a serial connection to be established
  while(!Serial);
  
  Serial.println("Connected!");

  //Begin the bluetooth service
  BLE.begin();

  //Defined in cms/bt_cms.h
  BLE.scanForUuid(FLSID);
  Serial.println("Waiting for periph!");
}

void loop() {

  peripheral = BLE.available();

  if(peripheral){

    Serial.println("CONNECTED TO PERIPHERAL\n");
    digitalWrite(LED, HIGH);

    BLE.stopScan();

    handleData(peripheral);

    BLE.scanForUuid(FLSID);
  }
}



void handleData(BLEDevice peripheral){


  if(peripheral.connect()){
    Serial.println("Connected");
    
  }

  if(peripheral.discoverAttributes()){

    Serial.println("Attributes Discovered!");

  }

  accelVals = peripheral.characteristic(FLSID);

  if(!accelVals){
    Serial.println("Not working!!!");
    return;
  }

  if(!accelVals.canWrite()){
    Serial.println("No writing!\n");
    return;
  }

  while(peripheral.connected()){
    
    if(!waiting){
      strcpy(STRBUF, ONSTR);
      accelVals.writeValue(STRBUF);
      waiting = 1;
      digitalWrite(LED, LOW);
      delay(2000);
    }

    if(waiting == 1){
      
      accelVals.readValue(STRBUF, FLSTRBUFLEN);
      if(!strcmp(STRBUF, ACKSTR)){
	digitalWrite(LED, HIGH);
	Serial.println("TURNED ON!");
	waiting = 2;
      }
    }
  }

  digitalWrite(LED, LOW);
  Serial.println("Turned off!");
  waiting = 0;
  Serial.println("Waiting for periph!");
}
