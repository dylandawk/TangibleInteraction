int newHours, newMinutes, newSeconds;

//Checks to see if user has entered edit mode (tapping button twice)
void enterMode()
{
  //If button is pressed once, start timer
  //Serial.print("CurrentState: "); Serial.print(currentState); Serial.print(", PreviousState: "); Serial.println(previousState);
  if (currentState == 0 && previousState == 1) {
    Serial.println("Button Pressed");
    buttonState++;
    isStartTimeout = true;
    startSecond = rtc.getSeconds();
  }
  //Keep track of time passed since button first pressed
  if (isStartTimeout)
  {
    timeout = abs(currentSecond - startSecond);
  }
  //checks how many times button is pressed, resets when a second has passed
  if (timeout > buttonDelay)
  {
    isStartTimeout = false;
    timeout = 0;
    Serial.println("isStartTimeout = false");
    switch (buttonState)
    {
      case 0:
      case 1: //Reset is button is only pressed once
        Serial.println("Clock Mode Entered");
        break;
      case 2: //Enter edit mode if button is only pressed once
        clockMode = 1;
        Serial.println("Edit Mode Entered");
        break;
      case 3 : // Enter Timer mode if Button is pressed three times
        clockMode = 2;
        Serial.println("Timer Mode Entered");
        break;
      default: break;
    }
    buttonState = 0;
  }
}

bool hasGottenTime;

void runEditMode()
{
  //preserve current information for editing
  if (!hasGottenTime)
  {
    hours = rtc.getHours() % 13; // keep hours constrained to 12 hour cycle
    minutes = rtc.getMinutes();
    seconds = rtc.getSeconds();
    encoderPosition = encoder.read();
    hasGottenTime = true;
  }
  //Detect button changes
  if (currentState == 0 && previousState == 1)
  {
    Serial.println("Button Pressed");
    editMode++;
  }

  //Change whether editing hours/minutes/seconds
  if (editMode != 3)
  {
    changeTime(editMode);
  }
  else
  {
    Serial.println("EditMode: Reset");
    clockMode = 0;
    editMode = 0;
    hasGottenTime = false;
    setClock();
  }

  showEditTime();

}


//Adjust the time based on mode (hours, minutes, seconds)
int prevMode = 0;
void changeTime(int mode)
{
  long currentPosition = encoder.read();
  int difference = currentPosition - encoderPosition;

  if (abs(difference) >= 4)
  {
    switch (mode) {
      case 0:
        hours += (abs(difference) / difference);
        newHours = constrain(hours, 1, 12);
        break;
      case 1:
        minutes += (abs(difference) / difference);
        newMinutes = constrain(minutes, 0, 59);
        break;
      case 2:
        seconds += (abs(difference) / difference);
        newSeconds = constrain(seconds, 0, 59);
        break;
    }
    encoderPosition = currentPosition;
    prevMode = mode;
  }
}

void setClock()
{
  rtc.setHours(newHours); rtc.setMinutes(newMinutes); rtc.setSeconds(newSeconds);
}

void showEditTime()
{
  Serial.print(newHours);
  Serial.print(":");
  Serial.print(newMinutes);
  Serial.print(":");
  Serial.println(newSeconds);
}
