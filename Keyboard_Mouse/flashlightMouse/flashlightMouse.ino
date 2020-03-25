#include <Arduino_LSM6DS3.h>
#include <Mouse.h>
#include <Adafruit_NeoPixel.h>
#include <bt_cms.h>

#define RED_PIN 3
#define GREEN_PIN 5
#define BLUE_PIN 6
#define PUSH_PIN 8
#define PIXEL_PIN 2
#define NUMPIXELS 1

Adafruit_NeoPixel pixels(NUMPIXELS, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

int currentState;
int previousState = LOW;
bool isPushPressed, hasMouseBegun;

//checkPressesDef
int previousPressed = 0;
bool hasTimeoutStarted;

unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;//

//Sensor DEFINITIONS
float gx, gy, gz, ax, ay, az;

const int  offset_gx = 1; //IMU at resting for gyroscope gives value of -1 for x (this isn't used in code)
const int  offset_gy = 3; //IMU at resting for gyroscope gives value of -4 for y
const int offset_gz = 0; //IMU at resting for gyroscope gives value of -2 for z

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
  readGyro();
  readAccel();
  readPushButton();
  checkPresses();
  handleLED();
  //viewData();

}

//turns on LED when button is pressed, turns of when released
void handleLED() {
  if (isPushPressed) {
    RGB_color(0, 0, 255); // if using RGB led
    for (int i = 0; i < NUMPIXELS; i++) { // For each pixel...
      //pixels.setPixelColor(i, r, g, b);
      pixels.setPixelColor(i, 0, 0, 100);
    }
  } else {
    RGB_color(0, 0, 0);
    pixels.fill(0, 0, 0);
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
    IMU.readGyroscope(gx, gy, gz);

    //Formatting for bluetooth transmission
    vals[GX] = (int16_t)(round(gx) + offset_gx);
    vals[GY] = (int16_t)(round(gy) + offset_gy);
    vals[GZ] = (int16_t)(round(gz) + offset_gz);

  }
  if (hasMouseBegun) {
    moveMouse(vals[GX], vals[GY], vals[GZ]);
  }
}

void viewData() {
  int i;
  for (i = AX; i < VALEND; ++i) {
    Serial.print(vals[i]); Serial.print('\t');
  }

  Serial.println();

}


void readAccel() {

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(ax, ay, az);

    vals[AX] = (int16_t)map((int)(ax * 100), -100, 100, 0, 255);
    vals[AY] = (int16_t)map((int)(ay * 100), -100, 100, 0, 255);
    vals[AZ] = (int16_t)map((int)(az * 100), -100, 100, 0, 255);

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
        vals[BS] = (int16_t)ON;
        pixels.show();   // Send the updated pixel colors to the hardware.
      }
      else if (currentState == HIGH) {
        /*Execute code when button released here*/
        Serial.println("OFF");
        vals[BS] = (int16_t)OFF;
        pixels.show();   // Send the updated pixel colors to the hardware.
      }
    }
  }
  if (currentState == 0) isPushPressed = true;
  else isPushPressed = false;
  previousState = reading;
}

int startTime, counter;
void checkPresses() {
  int currentPressed = vals[BS];
  int buttonDelay = 500;
  if (currentPressed != previousPressed) {
    Serial.println("mousePressed");
    if (hasMouseBegun) {
      Mouse.click();
    }
    if (!hasTimeoutStarted) {
      startTime = millis();
      counter = 0;
      hasTimeoutStarted = true;
    } else if (millis() - startTime <= buttonDelay) {
      if (hasMouseBegun) {
        Mouse.end();
        hasMouseBegun = false;
        Serial.println("Mouse ended");
        Serial.println(millis() - startTime);
      } else {
        Mouse.begin();
        hasMouseBegun = true;
        Serial.println("Mouse Begun");
        Serial.println(millis() - startTime);
      }
      hasTimeoutStarted = false;
      //Serial.println(millis()-startTime);
    }
  }
  if (millis() - startTime >= buttonDelay) {
    hasTimeoutStarted = false;
    //    Serial.println(counter++);
  }

  previousPressed = currentPressed;
}

//handles changing mouse movement, this is to be moved to CENTRAL BLE
void moveMouse(int gx, int gy, int gz) {
  float scaled_y = gy * (-0.25);
  float scaled_x = gz * (-0.25);
  Mouse.move(scaled_x, scaled_y);
}
