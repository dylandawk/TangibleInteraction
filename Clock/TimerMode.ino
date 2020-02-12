int timerTime = 0;
int currentMinutes = 0;
int prevMinutes = 0;

void runTimerMode()
{
  currentMinutes = rtc.getMinutes();
  if (!isTimerSet) {
    setTimer();
  } else
  {
    //decrease timer when a minute has passed
    if (prevMinutes != currentMinutes)
    {
      timerSetTime --;
      prevMinutes = currentMinutes;
    }

    //run stop timer function when time runs out
    if (timerSetTime < 0)
    {
      stopTimer();
    }

    enterModeTimer();
  }
}

void setTimer()
{
  long currentPosition = encoder.read();
  int difference = currentPosition - encoderPosition;
  if (abs(difference) >= 4)
  {
    timerSetTime += (abs(difference) / difference);
    timerSetTime = constrain(timerSetTime, 0, 59);
    Serial.print("timerSetTime: "); Serial.println(timerSetTime);
    encoderPosition = currentPosition;
  }

  if (currentState == 0 && previousState == 1) {
    Serial.println("Timer Set");
    isTimerSet = true;
    prevMinutes = currentMinutes;
  }
}

void stopTimer()
{
  flashLED();

  // return to clock mode when button is pressed
  if (currentState == 0 && previousState == 1)
  {
    clockMode = 0;
    isTimerSet = false;
  }
}

//Variation on enterMode for timer
void enterModeTimer()
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
        isTimerSet = false;
        clockMode = 2;
        Serial.println("Timer Mode Entered");
        break;
      case 2: //Enter Clock mode if button is pressed twice
        isTimerSet = false;
        clockMode = 0;
        Serial.println("Clock Mode Entered");
        break;
      default: break;
    }
    buttonState = 0;
  }
}
