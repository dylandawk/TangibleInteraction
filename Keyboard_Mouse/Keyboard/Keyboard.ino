#include <Keyboard.h> 

/*Double tap reset button to enter bootloader mode (subtle flashing). make sure you are in the right port 
itp.nyu.edu/classes/tangible-interaction/resources/class-notes*/
//make variable bool keyboardControl = true/false

void setup() {
  Keyboard.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  Keyboard.print('Hey, I'm Typing");
  delay(1000);
}
