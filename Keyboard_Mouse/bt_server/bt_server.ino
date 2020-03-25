#include <bt_cms.h>
#include <Mouse.h>

#define LED 2
#define SBAUD 115200

#define RED_PIN 5
#define GREEN_PIN 6
#define BLUE_PIN 3


BLEDevice peripheral;
BLECharacteristic imuVals;

int previousPressed = 0;
bool hasTimeoutStarted, hasMouseBegun;


void setup() {
  pinMode(LED, OUTPUT);
  
  //Initialize RGB pins
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  
  //Serial.begin(SBAUD);

  //Waits for a serial connection to be established
  //while(!Serial);
  
  //Serial.println("Connected!");

  //Begin the bluetooth service
  BLE.begin();

  //Defined in cms/bt_cms.h
  BLE.scanForUuid(FLSID);
}

void loop() {
  
  //Serial.println("Waiting for periph!");
  do{
    peripheral = BLE.available();
    RGB_Blink();
  }while(!peripheral);


  //Serial.println("CONNECTED TO PERIPHERAL\n");
  //digitalWrite(LED, HIGH);
  RGB_Blue();
  BLE.stopScan();
    
  //If we can not discover attributes, scan again
  if(!discoverBTAttribs()){
    receiveMessages(peripheral);
  }

  BLE.scanForUuid(FLSID);
}

int discoverBTAttribs(){
 if(peripheral.connect()){
//    Serial.println("Connected");    
  }

  if(peripheral.discoverAttributes()){
//    Serial.println("Attributes Discovered!");
  }

  imuVals = peripheral.characteristic(FLSID);

  if(!imuVals){
//    Serial.println("Not working!!!");
    return 1;
  }

  if(!imuVals.canRead()){
//    Serial.println("No READING!\n");
    return 1;
  }

  return 0;
}

void viewData(){

  int i;
  for(i = AX; i < VALEND; ++i){
//    Serial.print(vals[i]);Serial.print('\t');
  }

//  Serial.println();
    
}

void receiveMessages(BLEDevice peripheral){

//  Serial.println("Beginning reading!");

  while(peripheral.connected()){

    //read BT values
    imuVals.readValue((void *)vals, FLTXBUFLEN);

    RGB_Blue();

    //Serial.println(vals[BS]);
    
    //display BT values
    viewData();

    //check Button Presses
    checkPresses();

    //GZ == YAW
    //GY == PITCH
    //GX == ROLL
    if(hasMouseBegun){
      Mouse.move(-vals[GZ], -vals[GY], 0);
    }

    //Conform to an acceptable transmission rate
    delay(15);

  }

  digitalWrite(LED, LOW);
//  Serial.println("Turned off!");

}

//Changes LED color based on values passed
void RGB_Color(int r, int g, int b)
{
  analogWrite(RED_PIN, r);
  analogWrite(GREEN_PIN, g);
  analogWrite(BLUE_PIN, b);
}


//Blinks red To blue
void RGB_Blink(){
  int currentTime = millis();
  if(currentTime % 500 > 250) { //blink every half second
     RGB_Color(0,0,100);
  } else RGB_Color(100,0,0);
}


//stay blue
void RGB_Blue(){
  RGB_Color(0,0,100);
}

//double click enter or exit Mouse Mode

void checkPresses(){
  int currentPressed = vals[BS];
  int startTime;
  int buttonDelay = 500;
  if (currentPressed != previousPressed){
//    Serial.println("mousePressed");
    if(!hasTimeoutStarted){
      startTime = millis();
      hasTimeoutStarted = true;
    } else if (millis() - startTime >= buttonDelay){
      if(hasMouseBegun){
        Mouse.end();
        hasMouseBegun = false;
//        Serial.println("Mouse ended");
      } else {
        Mouse.begin();
        hasMouseBegun = true;
//        Serial.println("Mouse Begun");
      }
      hasTimeoutStarted = false;
    }
    if(hasMouseBegun){
      Mouse.click();
    }
  }
  previousPressed = currentPressed;
}
