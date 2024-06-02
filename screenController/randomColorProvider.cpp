#include "randomColorProvider.h"
#include "Arduino.h"

uint8_t randomColor[3];
RandomColorProvider::RandomColorProvider(){
};

uint8_t rct; // random color time TROUBLESHOOT VAR NAME ISSUES LATER
uint8_t* RandomColorProvider::getColor(){

  rct += 1;
  if(rct >= 20){
    randomColor[0] = random(0,256);
    randomColor[1] = random(0,256);
    randomColor[2] = random(0,256);
    rct = 0;
  }
  return randomColor;
}