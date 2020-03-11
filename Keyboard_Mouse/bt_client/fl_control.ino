//SENSOR DEFINITIONS
float gx,gy,gz,ax,ay,az;

//Initial gyro values
const int  offset_gx = 1; //IMU at resting for gyroscope gives value of -1 for x (this isn't used in code)
const int  offset_gy = 4; //IMU at resting for gyroscope gives value of -4 for y
const int offset_gz = 2; //IMU at resting for gyroscope gives value of -2 for z

//turns on LED when button is pressed, turns of when released
void handleLED() {
  if (isPushPressed) {
    for (int i = 0; i < NUMPIXELS; i++) { // For each pixel...
      pixels.setPixelColor(i, vals[AX], vals[AY], vals[AZ]);
    }
  } else {
    pixels.fill(0,0,0);
  }
  pixels.show();
}


//read gyroscope values
void readGyro() {

  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(gx, gy, gz);

    //recalibrate incoming data;
    vals[GX] = (int16_t)(round(gx) + offset_gx);
    vals[GY] = (int16_t)(round(gy) + offset_gy);
    vals[GZ] = (int16_t)(round(gz) + offset_gz);

  }
  
}

//read accelerometer values
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
        //Mouse.begin(); //move to bt_server;
        hasMouseBegun = true;
        pixels.show();   // Send the updated pixel colors to the hardware.
      }
      else if (currentState == HIGH) {
        /*Execute code when button released here*/
        Serial.println("OFF");
        //Mouse.end();
        hasMouseBegun = false;
        pixels.show();   // Send the updated pixel colors to the hardware.
      }
    }
  }
  if (currentState == 0) isPushPressed = true;
  else isPushPressed = false;
  previousState = reading;
}
