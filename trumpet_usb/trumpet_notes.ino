
int KEY_C[] = {60, 62, 64, 65, 67, 69, 71, 72};
int middleKey[8];
int currKey[8];
int currNote;
int prevNote = 0;
int isNotePlayed;
int currOctave;
int prevOctave = 3;
int minThreshold = 20;
int peakValue = 0;
int volume =  0;

void setKey(int i) {
  switch (i) {
    case (0):
      for (int j = 0; j < 8; j++) {
        middleKey[j] = KEY_C[j];
      }
      break;
    default:
      break;
  }
}

void setOctave(int octave) {
  switch (octave) {
    case LOWER_OCT:
      for (int i = 0; i < 8; i++) {
        currKey[i] = middleKey[i] - 12;
      }
      break;
    case MIDDLE_OCT:
      for (int i = 0; i < 8; i++) currKey[i] = middleKey[i];
      break;
    case HIGHER_OCT:
      for (int i = 0; i < 8; i++) {
        currKey[i] = middleKey[i] + 12;
      }
      break;
  }
}


void detectNote() {

  // nothing pressed
  if (!buttonState_01 && !buttonState_02 && !buttonState_03) {
    currNote = currKey[0];
  }
  // first button pressed
  else if (buttonState_01 && !buttonState_02 && !buttonState_03) {
    currNote = currKey[1];
  }
  // second button pressed
  else if (!buttonState_01 && buttonState_02 && !buttonState_03) {
    currNote = currKey[2];
  }
  // third button pressed
  else if (!buttonState_01 && !buttonState_02 && buttonState_03) {
    currNote = currKey[3];
  }
  // first and second button pressed
  else if (buttonState_01 && buttonState_02 && !buttonState_03) {
    currNote = currKey[4];
  }
  // first and third button pressed
  else if (buttonState_01 && !buttonState_02 && buttonState_03) {
    currNote = currKey[5];
  }
  // second and third button pressed
  else if (!buttonState_01 && buttonState_02 && buttonState_03) {
    currNote = currKey[6];
  }
  // all buttons pressed
  else if (buttonState_01 && buttonState_02 && buttonState_03) {
    currNote = currKey[7];
  }

  //if note is not being played, play current note
  if (!isNotePlayed && buttonState_PLAY) {
    midiCommand(0x90, currNote, 127);
    isNotePlayed = true;
    //Serial.print("Note Played: "); Serial.println(currNote);
  }
  // stop notes playing
  else if (isNotePlayed && !buttonState_PLAY) {
    for (int i = 0; i < 127; i ++) {
      midiCommand(0x80, i, 0);
      //Serial.print("Turned off: "); Serial.println(currKey[i]);
    }
    //Serial.println("Note Stopped!");
    isNotePlayed = false;
  }
  // change note playing
  else if (currNote != prevNote && buttonState_PLAY) {
    midiCommand(0x80, prevNote, 0);
    midiCommand(0x90, currNote, volume);
    Serial.print("Note Changed: "); Serial.println(currNote);
  }

  prevNote = currNote;

}

void detectOctave() {

  float x, y, z;
  float minThreshold = -0.4;
  float maxThreshold = 0.3;

  if (IMU.accelerationAvailable()) {
    IMU.readAcceleration(x, y, z);
    //Serial.print("X: "); Serial.println(x);

    //check octave
    if (x > maxThreshold) currOctave = HIGHER_OCT;
    else if (x < minThreshold) currOctave = LOWER_OCT;
    else currOctave = MIDDLE_OCT;

    // change octave
    if (currOctave != prevOctave) setOctave(currOctave);


    prevOctave = currOctave;

  }
}

void detectPress() {
  int noteThreshold = 500;
  int butt01 = analogRead(BUTT_01);
  int butt02 = analogRead(BUTT_02);
  int butt03 = analogRead(BUTT_03);
  
  // change button state
  buttonState_01 = (butt01 > noteThreshold) ? HIGH : LOW;
  buttonState_02 = (butt02 > noteThreshold) ? HIGH : LOW;
  buttonState_03 = (butt03 > noteThreshold) ? HIGH : LOW;
}

void detectBreath() {
  int breathThreshold = 40;
  int noise = 5;
  
  int val = analogRead(BREATH_SENSOR);
  int mappedVal = map(val, 300, 1024, 0, 127);
  Serial.println(mappedVal);
  
  // signal that note is played
  if (mappedVal > breathThreshold) {
    volume = mappedVal;
    buttonState_PLAY = true;
    
  // signal that note is no longer played
  } else if (mappedVal < -20) {
    volume = 0;
    buttonState_PLAY = false;
  }
–
}
