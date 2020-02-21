#include <Mouse.h>


void setup() {
  // put your setup code here, to run once:
  Mouse.begin();

}

void loop() {
  int x =1;
  int y = -1;
  Mouse.move(x,y);

}
