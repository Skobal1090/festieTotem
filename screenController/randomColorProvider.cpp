#include "randomColorProvider.h"
#include "Arduino.h"

uint8_t randomColor[3];
RandomColorProvider::RandomColorProvider(){
};

static uint8_t t = 0;
uint8_t* RandomColorProvider::getColor(){

  t += 1;
  if(t >= 20){
    randomColor[0] = random(0,256);
    randomColor[1] = random(0,256);
    randomColor[2] = random(0,256);
    t = 0;
  }
  return randomColor;
}