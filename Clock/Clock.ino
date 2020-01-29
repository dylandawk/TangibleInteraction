#include<RTCZero.h>
#include<Encoder.h>
#include<Adafruit_NeoPixel.h>

//Encoder Pins
#define EPIN1 4
#define EPIN2 5
#define EPIN3 6

//NeoPixel Variables
#define NPIN 3
#define NUMPIXELS 60

//Create rtc
RTCZero rtc;

//Create encoder
Encoder encoder(EPIN1, EPIN2);

//Create Neopixels
Adafruit_NeoPixel strip(NUMPIXELS, NPIN, NEO_GRB + NEO_KHZ800);

long encoderPosition = -1;
int seconds = 0, minutes = 0, hours = 0;
int currentSecond;
int currentState = 1;
int previousState =1;
int buttonMode = 0;

void setup() {
  //Initialize Serial
  Serial.begin(9600);

  //Intitialize rtc
  rtc.begin();
  pinMode(EPIN3, INPUT_PULLUP);

  //Initialize Neopixels
  strip.begin();
  strip.show();
  strip.setBrightness(50);

  //Set time
  rtc.setHours(hours);
  rtc.setMinutes(minutes);
  rtc.setSeconds(seconds);  

}

void loop() {

  currentState = digitalRead(EPIN3);
  //If button is pressed twice within .5 seconds, begin mode to change time and back
  if(currentState == 0 && previousState == 1){
    Serial.println("Button Pressed");
  }

  previousState = currentState;

  //If button is pressed once switch from Hours to Minutes to Seconds

  //Test button
//  buttonState = digitalRead(EPIN3);
//  Serial.println(buttonState);



  
}
