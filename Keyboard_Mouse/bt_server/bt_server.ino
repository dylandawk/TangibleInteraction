#include <bt_cms.h>

#define LED 2
#define SBAUD 115200


BLEDevice peripheral;
BLECharacteristic accelVals;

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(SBAUD);

  //Waits for a serial connection to be established
  while(!Serial);
  
  Serial.println("Connected!");

  //Begin the bluetooth service
  BLE.begin();

  //Defined in cms/bt_cms.h
  BLE.scanForUuid(FLSID);
}

void loop() {
  
  Serial.println("Waiting for periph!");
  do{
    peripheral = BLE.available();
  }while(!peripheral);


  Serial.println("CONNECTED TO PERIPHERAL\n");
  digitalWrite(LED, HIGH);
  BLE.stopScan();
    
  //If we can not discover attributes, scan again
  if(!discoverBTAttribs()){
    receiveMessages(peripheral);
  }

  BLE.scanForUuid(FLSID);
}

int discoverBTAttribs(){
 if(peripheral.connect()){
    Serial.println("Connected");    
  }

  if(peripheral.discoverAttributes()){
    Serial.println("Attributes Discovered!");
  }

  accelVals = peripheral.characteristic(FLSID);

  if(!accelVals){
    Serial.println("Not working!!!");
    return 1;
  }

  if(!accelVals.canRead()){
    Serial.println("No READING!\n");
    return 1;
  }

  return 0;
}

void viewData(){

  int i;
  for(i = AX; i <= GZ; ++i){
    Serial.print(vals[i]);Serial.print('\t');
  }

  Serial.println();
    
}

void receiveMessages(BLEDevice peripheral){


  Serial.println("Beginning reading!");

  while(peripheral.connected()){

    //read BT values
    accelVals.readValue((void *)vals, FLTXBUFLEN);
    
    //display BT values
    viewData();
    
    //Conform to an acceptable transmission rate
    delay(15);
  }

  digitalWrite(LED, LOW);
  Serial.println("Turned off!");

}
