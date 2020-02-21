#include<RTCZero.h>
#include<Encoder.h>
#include<Adafruit_NeoPixel.h>

//Encoder Pins
#define EPIN1 2
#define EPIN2 3
#define EPIN3 5

//NeoPixel Variables
#define NPIN 6
#define NUMPIXELS 60

//Create rtc
RTCZero rtc;

//Create encoder
Encoder encoder(EPIN1, EPIN2);

//Create Neopixels
Adafruit_NeoPixel strip(NUMPIXELS, NPIN, NEO_GRB + NEO_KHZ800);

long encoderPosition = -1;
int seconds = 0, minutes = 0, hours = 0;
float currentSecond;
float previousSecond;
float startSecond;
int currentState = 1, previousState = 1;
int buttonState = 0;
int clockMode = 0; // 0 = run normally, 1 = edit mode, 2 = timeout mode
int editMode = 0;  //0 = hours, 1 = minutes, 2 = seconds, 3 = exit
float timeout = 0;
bool isStartTimeout;
bool isTimerSet = false;
const float buttonDelay = 1;
int timerSetTime;

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

  Serial.println("Clock Initialized!");

}

void loop() {
  currentState = digitalRead(EPIN3);
  currentSecond = rtc.getSeconds();

  if(clockMode == 0)
  {
    displayTime();
    displayLED();
    enterMode();
  }
  else if(clockMode ==1)
  {
    runEditMode();
    displayEditModeLED();
  }
  else if(clockMode == 2)
  {
    runTimerMode();
    displayTimerModeLED();
  }
  
  previousState = currentState;
}

void displayTime()
{
  int currentSecondInt = currentSecond;
  int previousSecondInt = previousSecond;
  if(currentSecondInt != previousSecondInt)
  {
    hours = constrain(rtc.getHours() % 13, 1, 12);
    minutes = rtc.getMinutes();
    seconds = rtc.getSeconds();
    Serial.print(hours); 
    Serial.print(":"); 
    Serial.print(minutes); 
    Serial.print(":");
    Serial.println(seconds);
    previousSecond = currentSecondInt;
  }
}
