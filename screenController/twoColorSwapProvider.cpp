#include "twoColorSwapProvider.h"
#include "Arduino.h"

static uint8_t* colors[2];
TwoColorSwapProvider::TwoColorSwapProvider(uint8_t* color1, uint8_t* color2){
  colors[0] = color1;
  colors[1] = color2;
}

int selectedColor = 0;
static uint8_t t;

uint8_t* TwoColorSwapProvider::getColor(){

  t += 1;
  if (t >= 20){
    selectedColor = (selectedColor + 1) % 2;
    t = 0;
  }
  

  return colors[selectedColor];
}
