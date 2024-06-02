#include "twoColorSwapProvider.h"

TwoColorSwapProvider::TwoColorSwapProvider(uint8_t* color1, uint8_t* color2){
  uint8_t* colorArray[2] = {color1, color2}; //contains both colors
}

bool selectedColor = 0;
uint8_t tcst; //two color swap time

uint8_t* TwoColorSwapProvider::getColor(){

  tcst += 1;
  if (tcst >= 20){
    selectedColor = !selectedColor;
    tcst = 0;
  }
  return colorArray[selectedColor];
}
