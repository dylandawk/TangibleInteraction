

void displayLED()
{
  int ledHour, ledMinute, ledSecond;

  if (clockMode == 1) //use Edit mode time
  {
    ledHour = (newHours % 12) * 5;
    ledMinute = newMinutes;
    ledSecond = newSeconds;
  }
  else //use display time
  {
    ledHour = (hours % 12) * 5;
    ledMinute = minutes;
    ledSecond = seconds;
  }


  if (ledHour == ledMinute && (ledHour != ledSecond))
  {
    strip.setPixelColor(ledHour, 255, 255 , 0);
    strip.setPixelColor(ledSecond, 255, 0 , 0);
  }

  else if (ledHour == ledSecond && (ledHour != ledMinute))
  {
    strip.setPixelColor(ledHour, 255, 0 , 255);
    strip.setPixelColor(ledMinute, 0, 255, 0);
  }
  else if (ledMinute == ledSecond && (ledMinute != ledHour))
  {
    strip.setPixelColor(ledMinute, 0, 255, 255);
    strip.setPixelColor(ledHour, 255, 0, 0);
  }
  else if (ledHour == ledMinute && ledHour == ledSecond)
  {
    strip.setPixelColor(ledHour, 255, 255 , 255);
  }
  else
  {
    strip.setPixelColor(ledHour, 255, 0 , 0);
    strip.setPixelColor(ledMinute, 0, 255 , 0);
    strip.setPixelColor(ledSecond, 0, 0, 255);
  }

  /*Turn off any unused pixels*/
  for (int i = 0; i < NUMPIXELS; i++)
  {
    if (i != ledHour && i != ledMinute && i != ledSecond)
    {
      strip.setPixelColor(i, 0, 0, 0);
    }
  }
  strip.show();
}

void displayEditModeLED()
{
  if (millis() % 500 > 250)// every half second blink
  {
    displayLED();
  }
  else
  {
    switch (editMode)
    {
      case 0:
        strip.setPixelColor((newHours % 12) * 5, 0, 0 , 0);
        break;
      case 1:
        strip.setPixelColor(newMinutes, 0, 0 , 0);
        break;
      case 2:
        strip.setPixelColor(newSeconds, 0, 0 , 0);
        break;
    }
  }
}

void displayTimerModeLED()
{
  int currSecond = rtc.getSeconds();
  for (int i = 0; i < NUMPIXELS; i++)
  {
    if (i <=  timerSetTime)
    {
      if (isTimerSet && currSecond % 2 == 1 && i == timerSetTime)
      {
        strip.setPixelColor(i, 0, 0, 0);
      } else strip.setPixelColor(i, 0, 255, 255);
    }
    else
    {
      strip.setPixelColor(i, 0, 0, 0);
    }
  }
  strip.show();
}

//blink LED red every other second;
void flashLED()
{
  int currSecond = rtc.getSeconds();
  if (currSecond % 2 == 1)
  {
    for (int i = 0; i < NUMPIXELS; i++)
    {
      strip.setPixelColor(i, 255, 0 , 0);
    }
  } else
  {
    for (int i = 0; i < NUMPIXELS; i++)
    {
      strip.setPixelColor(i, 0, 0 , 0);
    }
  }
  strip.show();
}
