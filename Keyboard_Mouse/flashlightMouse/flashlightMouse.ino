#include <Arduino_LSM6DS3.h>
#include <Mouse.h>
#include <ArduinoBLE.h>
#include <Adafruit_NeoPixel.h>
//#include <SerialCommand.h>

#define RED_PIN 3
#define GREEN_PIN 5
#define BLUE_PIN 6
#define PUSH_PIN 8
#define PIXEL_PIN 2
#define NUMPIXELS 1

//SerialCommand scmd;

Adafruit_NeoPixel pixels(NUMPIXELS, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

int currentState;
int previousState = LOW;
bool isPushPressed, hasMouseBegun;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;//

const int  offset_gx = 1; //IMU at resting for gyroscope gives value of -1 for x (this isn't used in code)
const int  offset_gy = 4; //IMU at resting for gyroscope gives value of -4 for y
const int offset_gz = 2; //IMU at resting for gyroscope gives value of -2 for z

int pixel_hue = 0;
int r, g, b;

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

  //Initialize pixel
  pixels.begin();
}

void loop() {
  //pixels.clear();
  readGyro();
  readAccel();
  readPushButton();
  handleLED();

}

//turns on LED when button is pressed, turns of when released
void handleLED() {
  if (isPushPressed) {
    RGB_color(0, 0, 255); // if using RGB led
    for (int i = 0; i < NUMPIXELS; i++) { // For each pixel...
      pixels.setPixelColor(i, r, g, b);
    }
  } else {
    RGB_color(0, 0, 0);
    for (int i = 0; i < NUMPIXELS; i++) { // For each pixel...
      pixels.setPixelColor(i, pixels.Color(0, 0, 0));
    }
  }
  pixels.show();
}

void handlePixel(float x, float y, float z) {
  int x_scaled = x * 100;
  int y_scaled = y * 100;
  int z_scaled = z * 100;
  r = map(x_scaled, -100, 100, 0, 255);
  g = map(y_scaled, -100, 100, 0, 255);
  b = map(z_scaled, -100, 100, 0, 255);
  //  Serial.print(x); Serial.print('\t');
  //  Serial.print(y); Serial.print('\t');
  //  Serial.println(z);
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

    //recalibrate incoming data;
    int x_avg = round(x) + offset_gx;
    int y_avg = round(y) + offset_gy;
    int z_avg = round(z) + offset_gz;

    if (hasMouseBegun) {
      moveMouse(x_avg, y_avg, z_avg);
    }
    //        Serial.print(x_avg); Serial.print('\t');
    //        Serial.print(y_avg); Serial.print('\t');
    //        Serial.println(z_avg);
  }
}

void readAccel() {

  float x, y, z;

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);
    handlePixel(x, y, z);

    //    Serial.print(x); Serial.print('\t');
    //    Serial.print(y); Serial.print('\t');
    //    Serial.println(z);
  }
}

//original debounce code from https://www.arduino.cc/en/tutorial/debounce
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
        /*Execute code when button pressed here*/
        Serial.println("ON");
        Mouse.begin();
        hasMouseBegun = true;
        pixels.show();   // Send the updated pixel colors to the hardware.
      }
      else if (currentState == HIGH) {
        /*Execute code when button released here*/
        Serial.println("OFF");
        Mouse.end();
        hasMouseBegun = false;
        pixels.show();   // Send the updated pixel colors to the hardware.
      }
    }
  }
  if (currentState == 0) isPushPressed = true;
  else isPushPressed = false;
  previousState = reading;
}

//handles changing mouse movement, this is to be moved to CENTRAL BLE
void moveMouse(int gx, int gy, int gz) {
  float scale_y = -0.5;//flip axis and scale movement
  float scale_z = -0.5;//flip axis and scale movement

  float vy = scale_y * gy;
  float vx = scale_z * gz; //uses gyroscope z as mouse x-axis
  Mouse.move(vx, vy);
}