#include <Arduino_LSM6DS3.h>
#include <Mouse.h>
#include <ArduinoBLE.h>

#define RED_PIN 3
#define GREEN_PIN 5
#define BLUE_PIN 6
#define PUSH_PIN 8

int currentState;
int previousState = LOW;
bool isPushPressed, hasMouseBegun;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;//

const int  recalib_gx = 1; //IMU at resting for gyroscope gives value of -1 for x (this isn't used in code)
const int  recalib_gy = 4; //IMU at resting for gyroscope gives value of -4 for y
const int recalib_gz = 2; //IMU at resting for gyroscope gives value of -2 for z


void setup() {
  Serial.begin(9600);
  while (!Serial);

  //Initialize Accelerometer/Gyroscope
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  Serial.print("Gyro sample rate = "); Serial.print(IMU.gyroscopeSampleRate()); Serial.println(" Hz");
  Serial.println();
  Serial.println("Gyroscope in degrees/second");
  Serial.println("X\tY\tZ");

  //Initialize pins
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(PUSH_PIN, INPUT_PULLUP);
}

void loop() {
  readGyro();
  //readAccel();
  readPushButton();
  handleLED();

}

//turns on LED when button is pressed, turns of when released
void handleLED() {
  if (isPushPressed) {
    RGB_color(0, 0, 255);
  } else RGB_color(0, 0, 0);
}

//Changes LED color based on values passed
void RGB_color(int red_light_value, int green_light_value, int blue_light_value)
{
  analogWrite(RED_PIN, red_light_value);
  analogWrite(GREEN_PIN, green_light_value);
  analogWrite(BLUE_PIN, blue_light_value);
}

void readGyro() {
  float x, y, z;

  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(x, y, z);

    int x_avg = round(x);
    int y_avg = round(y);
    int z_avg = round(z);

    if (hasMouseBegun) {
      moveMouse(x_avg, y_avg, z_avg);
    }
    Serial.print(x_avg); Serial.print('\t');
    Serial.print(y_avg); Serial.print('\t'); 
    Serial.println(z_avg);
  }
}

void readAccel() {

  float x, y, z;

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);

    Serial.print(x); Serial.print('\t'); 
    Serial.print(y); Serial.print('\t'); 
    Serial.println(z);
  }
}

//Debounce code from https://www.arduino.cc/en/tutorial/debounce
//Essentially prevents accidental multi-registering of button clicks
void readPushButton() {
  int reading = digitalRead(PUSH_PIN);
  if (reading != previousState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != currentState) {
      currentState = reading;
      if (currentState == LOW) {
        Serial.println("Button Pressed");
        Mouse.begin();
        hasMouseBegun = true;
      }
      else if (currentState == HIGH) {
        Serial.println("Button Released");
        Mouse.end();
        hasMouseBegun = false;
      }
    }
  }
  if (currentState == 0) isPushPressed = true;
  else isPushPressed = false;
  previousState = reading;
}

//handles changing mouse movement to 
void moveMouse(int gx, int gy, int gz) {
  float scale_y = -0.5;//flip axis and scale movement
  float scale_z = -0.5;//flip axis and scale movement

  float vy = scale_y * (gy + recalib_gy);
  float vx = scale_z * (gz + recalib_gz); //uses gyroscope z as x-axis
  Mouse.move(vx, vy);
}
