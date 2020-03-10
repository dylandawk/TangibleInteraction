//turns on LED when button is pressed, turns of when released
void handleLED() {
  if (isPushPressed) {
    for (int i = 0; i < NUMPIXELS; i++) { // For each pixel...
      pixels.setPixelColor(i, r, g, b);
    }
  } else {
    pixels.fill(0,0,0);
  }
  pixels.show();
}


//control pixel color with accelerometer values
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

//read gyroscope values
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

//read accelerometer values
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
