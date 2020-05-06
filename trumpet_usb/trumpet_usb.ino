/*
  Simple MIDI player for MIDUSB
  Generates a series of MIDI notes.
   The melody is Steve Reich's "Piano phase"
  Uses MIDIUSB for MIDI, so will work on any
  32U4-based board (e.g. Uno, Leonardo, Micro, Yún)
  Circuit:
    none
    
  created 13 Feb 2017
  modified 13 Dec 2018
  by Tom Igoe
*/
#include <MIDIUSB.h>
#include <Arduino_LSM6DS3.h>

#define PUSH_01 2
#define PUSH_02 3
#define PUSH_03 4
#define PUSH_04 5

#define BREATH_SENSOR A0
#define BUTT_01 A3
#define BUTT_02 A2
#define BUTT_03 A1

#define LOWER_OCT 2
#define MIDDLE_OCT 3
#define HIGHER_OCT 4

int buttonState_01, buttonState_02, buttonState_03, buttonState_PLAY;


int bpm = 72;  // beats per minute
// duration of a beat in ms
float beatDuration = 60.0 / bpm * 1000;

// the melody sequence:
int melody[] = {64, 66, 71, 73, 74, 66, 64, 73, 71, 66, 74, 73};
// which note of the melody to play:
int noteCounter = 0;

void setup() {

  Serial.begin(9600);
  while(!Serial);
  // Initialize pins
  pinMode(PUSH_01, INPUT_PULLUP);
  pinMode(PUSH_02, INPUT_PULLUP);
  pinMode(PUSH_03, INPUT_PULLUP);
  pinMode(PUSH_04, INPUT_PULLUP);

  //Initialize IMU
  if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }


  // Initialize Instrument key
  setKey(0);
  // Initialize Instrument octave
  setOctave(MIDDLE_OCT);

}

void loop() {
  detectNote();
  detectPress();
  detectBreath();
  detectOctave();
}

// send a 3-byte midi message
void midiCommand(byte cmd, byte data1, byte  data2) {
  // First parameter is the event type (top 4 bits of the command byte).
  // Second parameter is command byte combined with the channel.
  // Third parameter is the first data byte
  // Fourth parameter second data byte

  midiEventPacket_t midiMsg = {cmd >> 4, cmd, data1, data2};
  MidiUSB.sendMIDI(midiMsg);
 // Serial.println("Message Sent");
  
}
